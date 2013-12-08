/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QDebug>
#include <QMap>
#include <QFile>
#include <QDomDocument>
#include <QApplication>
#include <QCleanlooksStyle>

#include "libBlueSky/Application.hpp"
#include "libBlueSky/Windows.hpp"
#include "libBlueSky/ColorSchema.hpp"
#include "libBlueSky/Internal/XmlState.hpp"

#include "libBlueSky/Style.hpp"

namespace BlueSky {

    class Application::Private {
    public:
        Private(Application* owner);

    public:
        static Application* sInstance;

        QVector<Window*> mWindows;
        QHash<QByteArray, Mode*> mModes;
        ViewDescriptor* mModelessView;
        Mode* mActiveMode;
        QStyle* mDialogStyle;

        bool mModeReorderQueued;

        Internal::XmlStateRoot::Ptr mModelessFakeState;
    };

    Application::Private::Private(Application* owner)
        : mModelessView(NULL)
        , mActiveMode(NULL)
        , mDialogStyle(NULL)
        , mModeReorderQueued(false)
    {
        Q_ASSERT(sInstance == NULL);
        Private::sInstance = owner;

        mDialogStyle = qApp->style();
        mDialogStyle->setParent(sInstance);

        qApp->setStyle(new Style(new QCleanlooksStyle));
    }

    Application::Application()
    {
        d = new Private(this);
        ColorSchema::instance();
    }

    Application::~Application() {
        delete d;
        Private::sInstance = NULL;
    }

    Application* Application::Private::sInstance = NULL;

    Application* Application::instance() {
        Q_ASSERT(Private::sInstance);
        return Private::sInstance;
    }

    void Application::setModelessView(ViewDescriptor* view) {
        d->mModelessView = view;

        d->mModelessFakeState = Internal::XmlState::createFake(view);
    }

    ViewDescriptor* Application::modelessView() {
        return d->mModelessView;
    }

    void Application::addMode(Mode* mode) {
        Q_ASSERT(mode);
        Q_ASSERT(!d->mModes.contains(mode->identifier()));

        mode->setDisplayOrder(d->mModes.count() + 1);

        d->mModes.insert(mode->identifier(), mode);
        connect(mode, SIGNAL(orderChanged()), SLOT(queueModeReorder()));

        emit modeAdded(mode);
    }

    void Application::removeMode(Mode* mode) {
        if (mode) {
            QByteArray identifier = mode->identifier();

            if (d->mModes.contains(identifier)) {
                emit modeAboutToRemove(mode);
                mode->disconnect(this);
                d->mModes.remove(identifier);
            }
        }
    }

    void Application::removeMode(const QByteArray& identifier) {
        Mode* mode = findMode(identifier);
        if (mode) {
            removeMode(mode);
        }
    }

    Mode* Application::findMode(const QByteArray& identifier) const {
        return d->mModes.value(identifier, NULL);
    }

    Mode::List Application::allModes() const {
        Mode::List l;
        foreach (Mode* mode, d->mModes.values()) {
            l.append(mode);
        }
        return l;
    }

    Mode::List Application::allModesOrdered() const {
        QMap<int, Mode*> order;
        Mode::List l;

        foreach (Mode* mode, d->mModes.values()) {
            order.insert(mode->displayOrder(), mode);
        }

        foreach (Mode* mode, order) {
            l.append(mode);
        }

        return l;
    }

    Mode* Application::activeMode() {
        return d->mActiveMode;
    }

    void Application::setActiveMode(const QByteArray& identifier) {
        Mode* mode = findMode(identifier);
        setActiveMode(mode);
    }

    void Application::setActiveMode(Mode* mode) {
        if (d->mActiveMode != mode) {
            d->mActiveMode = mode;
            forceModeSwitch();
            emit activeModeChanged(mode);
        }
    }

    void Application::forceModeSwitch() {

        Internal::XmlStateRoot::Ptr state = d->mModelessFakeState;

        if (d->mActiveMode) {
            state = d->mActiveMode->currentState();
        }

        Internal::ModeSwitcher s(state);
        s.run();
    }

    PrimaryWindow* Application::primaryWindow() {

        Window* w = window(PrimaryWindow::idPrimaryWindow());
        return qobject_cast<PrimaryWindow*>(w);
    }

    void Application::emitModesReordered() {
        d->mModeReorderQueued = false;
        emit modesReordered();
    }

    void Application::queueModeReorder() {
        if (!d->mModeReorderQueued) {
            d->mModeReorderQueued = true;
            QMetaObject::invokeMethod(this, "emitModesReordered", Qt::QueuedConnection);
        }
    }

    QVector<Window*> Application::allWindows() const {
        return d->mWindows;
    }

    Window* Application::window(const ViewIdentifier& id, bool create) {
        foreach (Window* win, d->mWindows) {
            if (win->identifier() == id) {
                return win;
            }
        }

        if (id == PrimaryWindow::idPrimaryWindow()) {
            PrimaryWindow* w = newPrimaryWindow();
            d->mWindows.append(w);
            forceModeSwitch();
            return w;
        }

        if (create) {
            //return createSecondaryWindow();
        }

        return NULL;
    }

    bool Application::loadState(const QString& fileName, bool isDefault) {

        QFile file(fileName);
        if (!file.open(QFile::ReadOnly)) {
            return false;
        }

        QDomDocument doc(QLatin1String("Sky:State"));
        doc.setContent(&file);
        return loadState(doc, isDefault);
    }

    bool Application::loadState(const QDomDocument& doc, bool isDefault) {

        QByteArray modeToActivate;

        QDomNodeList nl = doc.documentElement().childNodes();
        if (!nl.count()) {
            return true;
        }

        for (int i = 0; i < nl.count(); i++) {
            QDomNode node = nl.at(i);

            if (!node.isElement()) {
                continue;
            }

            QDomElement el = node.toElement();
            QString tn = el.tagName();
            if (tn == QLatin1String("Sky:Mode")) {

                QDomElement elId = el.firstChildElement(QLatin1String("Sky:Id"));
                if (elId.isNull()) {
                    return false;
                }

                QByteArray identifier = elId.text().toUtf8();
                Mode* mode = findMode(identifier);
                if (!mode) {
                    mode = new Mode(identifier);
                    addMode(mode);
                }

                mode->readFromState(el, isDefault);
            }
            else if (tn == QLatin1String("Sky:CurrentMode")) {
                modeToActivate = el.text().toUtf8();
            }
            else {
                qDebug() << "Unknown tag:" << tn;
                return false;
            }
        }

        if (!modeToActivate.isEmpty()) {
            Mode* mode = findMode(modeToActivate);
            if (!mode) {
                qDebug() << "Cannot find current mode.";
                return false;
            }
            setActiveMode(mode);
        }

        return true;
    }

    PrimaryWindow* Application::newPrimaryWindow() {
        return new PrimaryWindow;
    }

    Window* Application::newSecondaryWindow() {
        return NULL; //new Window;
    }

    QStyle* Application::dialogStyle() const {
        return d->mDialogStyle;
    }

}

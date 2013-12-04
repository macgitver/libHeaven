
#include <QDebug>
#include <QMap>
#include <QFile>
#include <QDomDocument>
#include <QApplication>

#include "libBlueSky/Application.hpp"
#include "libBlueSky/Windows.hpp"
#include "libBlueSky/ColorSchema.hpp"
#include "libBlueSky/Internal/XmlState.hpp"

#include "libBlueSky/Style.hpp"

namespace BlueSky {

    Application::Application() {
        Q_ASSERT(sInstance == NULL);
        sInstance = this;
        ColorSchema::init();

        qApp->setStyle(new Style(qApp->style()));
    }

    Application::~Application() {
        sInstance = NULL;
    }

    Application* Application::sInstance = NULL;

    Application* Application::instance() {
        Q_ASSERT(sInstance);
        return sInstance;
    }

    void Application::addMode(Mode* mode) {
        Q_ASSERT(mode);
        Q_ASSERT(!mModes.contains(mode->identifier()));

        mode->setDisplayOrder(mModes.count() + 1);

        mModes.insert(mode->identifier(), mode);
        connect(mode, SIGNAL(orderChanged()), SLOT(queueModeReorder()));

        emit modeAdded(mode);
    }

    void Application::removeMode(Mode* mode) {
        if (mode) {
            QByteArray identifier = mode->identifier();

            if (mModes.contains(identifier)) {
                emit modeAboutToRemove(mode);
                mode->disconnect(this);
                mModes.remove(identifier);
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
        return mModes.value(identifier, NULL);
    }

    Mode::List Application::allModes() const {
        Mode::List l;
        foreach (Mode* mode, mModes.values()) {
            l.append(mode);
        }
        return l;
    }

    Mode::List Application::allModesOrdered() const {
        QMap<int, Mode*> order;
        Mode::List l;

        foreach (Mode* mode, mModes.values()) {
            order.insert(mode->displayOrder(), mode);
        }

        foreach (Mode* mode, order) {
            l.append(mode);
        }
        return l;
    }

    Mode* Application::activeMode() {
        return mActiveMode;
    }

    void Application::setActiveMode(const QByteArray& identifier) {
        Mode* mode = findMode(identifier);
        setActiveMode(mode);
    }

    void Application::setActiveMode(Mode* mode) {
        if (mActiveMode != mode) {

            Internal::ModeSwitcher s(mode ? mode->currentState() : Internal::XmlStateRoot::Ptr());
            s.run();

            mActiveMode = mode;
            emit activeModeChanged(mode);
        }
    }

    PrimaryWindow* Application::primaryWindow() {

        Window* w = window(PrimaryWindow::idPrimaryWindow());
        return qobject_cast<PrimaryWindow*>(w);
    }

    void Application::emitModesReordered() {
        mModeReorderQueued = false;
        emit modesReordered();
    }

    void Application::queueModeReorder() {
        if (!mModeReorderQueued) {
            mModeReorderQueued = true;
            QMetaObject::invokeMethod(this, "emitModesReordered", Qt::QueuedConnection);
        }
    }

    QVector<Window*> Application::allWindows() const {
        return mWindows;
    }

    Window* Application::window(const ViewIdentifier& id, bool create) {
        foreach (Window* win, mWindows) {
            if (win->identifier() == id) {
                return win;
            }
        }

        if (id == PrimaryWindow::idPrimaryWindow()) {
            PrimaryWindow* w = newPrimaryWindow();
            mWindows.append(w);
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

}

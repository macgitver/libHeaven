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

#include <QChildEvent>
#include <QResizeEvent>
#include <QStyle>

#include "libBlueSky/Application.hpp"
#include "libBlueSky/Windows.hpp"
#include "libBlueSky/Dialog.hpp"
#include "libBlueSky/SizeGrip.hpp"

namespace BlueSky {

#ifndef Q_OS_MAC
    class Dialog::SheetEventFilter : public QObject {
    public:
        SheetEventFilter(QWidget* topLevelWidget, Dialog* parent)
            : QObject(parent)
            , mParentDlg(parent)
        {
            Q_ASSERT(parent);
            Q_ASSERT(topLevelWidget && topLevelWidget->isWindow());
            topLevelWidget->installEventFilter(this);
        }


    protected:
        bool eventFilter(QObject* obj, QEvent* event)
        {
            Q_ASSERT(obj == mParentDlg);

            switch (event->type()) {
            default: break;

            case QEvent::Move:
            case QEvent::Resize:
                mParentDlg->updatePinnedPos();
                break;
            }

            return QObject::eventFilter(obj, event);
        }

    private:
        Dialog*     mParentDlg;
    };
#endif

    class Dialog::WatchDog : public QObject {
    public:
        WatchDog();

        bool eventFilter(QObject* watched, QEvent* ev);
        void applyStyle(QWidget* w);

    public:
        void bark();

    public:
        bool mQueued;
        QList<QPointer<QObject> > mChildren;
        Dialog* mOwner;
    };

    Dialog::WatchDog::WatchDog() {
        mQueued = false;
    }

    bool Dialog::WatchDog::eventFilter(QObject* watched, QEvent* ev) {

        if (ev->type() == QEvent::ChildAdded) {
            QChildEvent* cev = static_cast<QChildEvent*>(ev);

            QObject* o = cev->child();
            if (!mChildren.contains(o)) {
                mChildren.append(o);
                if (!mQueued) {
                    mQueued = true;
                    QMetaObject::invokeMethod(mOwner, "watchDogBark", Qt::QueuedConnection);
                }
            }
            o->installEventFilter(this);
        }
        else if (ev->type() == QEvent::ChildRemoved) {
            QChildEvent* cev = static_cast<QChildEvent*>(ev);

            QObject* o = cev->child();
            if (mChildren.contains(o)) {
                mChildren.removeAll(o);
            }
        }

        return QObject::eventFilter(watched, ev);
    }

    void Dialog::WatchDog::bark() {

        QSet<QObject*> visited;
        QList<QPointer<QObject> > all = mChildren;
        mQueued = false;
        mChildren.clear();

        while (!all.isEmpty()) {
            QPointer<QObject> kid1 = all.takeFirst();

            if (kid1.isNull()) {
                continue;
            }

            if (visited.contains(kid1)) {
                continue;
            }
            visited.insert(kid1);

            if (!kid1->isWidgetType()) {
                continue;
            }

            QWidget* kid2 = static_cast<QWidget*>(kid1.data());

            /*
            qDebug() << "A"
                     << kid2->metaObject()->className()
                     << "named"
                     << kid2->objectName()
                     << "is looking over the fence. Preparing to bark at it.";
            */

            applyStyle(kid2);

            foreach (QObject* o, kid2->children()) {
                all.append(o);
            }
        }
    }

    void Dialog::WatchDog::applyStyle(QWidget *w) {

        QStyle* style = BlueSky::Application::instance()->dialogStyle();
        w->setStyle(style);
    }

    Dialog::Dialog()
        : QDialog(BlueSky::Application::instance()->primaryWindow())
        , w(new WatchDog)
        , mSizeGripEnabled(false)
        , mResizer(0)
    {
        w->mOwner = this;
        installEventFilter(w);
    }

    Dialog::~Dialog() {
        delete w;
    }

    bool Dialog::isResizerEnabled() const
    {
        return mSizeGripEnabled;
    }

    void Dialog::setResizerEnabled(bool enabled)
    {
        mSizeGripEnabled = enabled;
    }

    void Dialog::open()
    {
#ifndef Q_OS_MAC
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Sheet);
        new SheetEventFilter(parentWidget(), this);
#endif

        QDialog::open();
    }

#ifndef Q_OS_MAC
    void Dialog::moveEvent(QMoveEvent* ev)
    {
        QDialog::moveEvent(ev);
        if (isSheet()) {
            updatePinnedPos();
        }
    }

    void Dialog::resizeEvent(QResizeEvent* ev)
    {
        if (mResizer) {
            mResizer->updatePos();
            mResizer->raise();
        }

        QDialog::resizeEvent(ev);
    }
#endif

    void Dialog::showEvent(QShowEvent* ev) {
        w->applyStyle(this);
        QDialog::showEvent(ev);

        if (mSizeGripEnabled) {
            if (!mResizer) {
                mResizer = new SizeGrip(this);
            }
#ifndef Q_OS_MAC
            if (isSheet()) {
                mResizer->setResizeBehaviour(SizeGrip::ResizeMirrorHorizontal);
            }
#endif
            mResizer->updatePos();
            mResizer->raise();
            mResizer->show();
        }
    }

    void Dialog::watchDogBark() {
        w->bark();
    }

}

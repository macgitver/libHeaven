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
#include <QStyle>
#include <QApplication>
#include <QChildEvent>

#include "libBlueSky/Application.hpp"
#include "libBlueSky/Windows.hpp"
#include "libBlueSky/Dialog.hpp"

namespace BlueSky {

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
    {
        w->mOwner = this;
        installEventFilter(w);
    }

    Dialog::~Dialog() {
        delete w;
    }

    void Dialog::showEvent(QShowEvent* ev) {
        w->applyStyle(this);
        QDialog::showEvent(ev);
    }

    void Dialog::watchDogBark() {
        w->bark();
    }

}

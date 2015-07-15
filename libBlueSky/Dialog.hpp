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

#pragma once

#include <QDialog>

#include "libBlueSky/libBlueSkyAPI.hpp"

namespace BlueSky {

    class SizeGrip;

    /**
     * @brief A Dialog
     *
     * This class automatically gives the Heaven::primaryWindow() as parent to the dialog.
     *
     */
    class HEAVEN_BLUESKY_API Dialog
            : public QDialog
    {
        Q_OBJECT

        class SheetEventFilter;
        class WatchDog;

        friend class SheetEventFilter;

    public:
        Dialog();
        ~Dialog();

    public:
        bool isResizerEnabled() const;
        void setResizerEnabled(bool enabled);

    public slots:
        virtual void open();

    protected:
#ifndef Q_OS_MAC
        void moveEvent(QMoveEvent* ev);
        void resizeEvent(QResizeEvent* ev);
#endif
        void showEvent(QShowEvent* ev);

#ifndef Q_OS_MAC
    private:
        bool isSheet() const;
        void updatePinnedPos();
#endif

    private:
        bool isSizeGripEnabled() const { return false; /* delete method */ }
        void setSizeGripEnabled(bool enabled) const { /* delete method */ }

    private:
        WatchDog*   w;

        bool        mSizeGripEnabled;
        SizeGrip*   mResizer;

    private slots:
        void watchDogBark();
    };

#ifndef Q_OS_MAC
    inline bool Dialog::isSheet() const
    {
        Qt::WindowFlags wf = windowFlags();
        return wf.testFlag(Qt::Sheet) && !wf.testFlag(Qt::ToolTip);
    }

    inline void Dialog::updatePinnedPos()
    {
        QWidget* pw = parentWidget();
        Q_ASSERT(pw);

        // TODO: implement all alignment positions
        const QRect pr = pw->geometry();

        //move to top center
        QPoint fixedPos(pr.center().x() - (width() / 2), pr.top());
        if (pos() != fixedPos) {
            move(fixedPos);
        }
    }
#endif

}

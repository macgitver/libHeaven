/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2015 Cunz RAD Ltd.
 *
 * (C) Nils Fenner <nils@macgitver.org>
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

#include "libBlueSky/libBlueSkyAPI.hpp"

#include <QSizeGrip>

namespace BlueSky {

    class HEAVEN_BLUESKY_API SizeGrip
            : public QSizeGrip
    {
    public:
        enum ResizeBehaviour {
            ResizeDefault             = 0x00,
            ResizeMirrorHorizontal    = 0x01,
            ResizeMirrorVertical      = 0x02
        };
        Q_DECLARE_FLAGS(ResizeDirections, ResizeBehaviour);

    public:
        explicit SizeGrip(QWidget* parent);

    public:
        ResizeDirections resizeBehaviour() const;
        void setResizeBehaviour(int behaviour);

    public:
        inline bool atBottom() const
        {
            return mCorner == Qt::BottomRightCorner || mCorner == Qt::BottomLeftCorner;
        }

        inline bool atLeft() const
        {
            return mCorner == Qt::BottomLeftCorner || mCorner == Qt::TopLeftCorner;
        }

        inline void updatePos()
        {
            move(mParentWindow->isLeftToRight() ? mParentWindow->rect().bottomRight() - rect().bottomRight()
                                             : mParentWindow->rect().bottomLeft() - rect().bottomLeft());
        }

    private:
        void mousePressEvent(QMouseEvent* ev) Q_DECL_OVERRIDE;
        void mouseReleaseEvent(QMouseEvent* ev) Q_DECL_OVERRIDE;
        void mouseMoveEvent(QMouseEvent * ev) Q_DECL_OVERRIDE;

    private:
        ResizeDirections    mResizeDirections;

        QWidget*    mParentWindow;
        bool        mGotMousePress;
        QPoint      mStartPos;
        int         dxMax;
        int         dyMax;
        QRect       mDlgRect;
        Qt::Corner  mCorner;
    };

}

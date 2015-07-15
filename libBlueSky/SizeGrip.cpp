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

#include "SizeGrip.hpp"

#include <QAbstractScrollArea>
#include <QApplication>
#include <QDesktopWidget>
#include <QLayout>
#include <QMouseEvent>

namespace BlueSky {

SizeGrip::SizeGrip(QWidget* parent)
    : QSizeGrip(parent)
    , mResizeDirections(ResizeDefault)
    , mParentWindow(parent)
    , mGotMousePress(false)
    , dxMax(0)
    , dyMax(0)
    , mCorner(mParentWindow->isLeftToRight() ? Qt::BottomRightCorner : Qt::BottomLeftCorner)
{
    Q_ASSERT(parent);
    resize(sizeHint());
}

SizeGrip::ResizeDirections SizeGrip::resizeBehaviour() const
{
    return mResizeDirections;
}

void SizeGrip::setResizeBehaviour(int behaviour)
{
    mResizeDirections = static_cast<ResizeDirections>(behaviour);
}

void SizeGrip::mousePressEvent(QMouseEvent* ev)
{
    if (ev->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(ev);
        return;
    }

    QWidget *tlw = mParentWindow;
    mGotMousePress = true;
    mStartPos = ev->globalPos();
    mDlgRect = mParentWindow->geometry();

    // TODO: The following requires QPA stuff and we don't have access here
    // Does the platform provide size grip support?
//    m_platformSizeGrip = false;
//    if (tlw->isWindow()
//        && tlw->windowHandle()
//        && !(tlw->windowFlags() & Qt::X11BypassWindowManagerHint)
//        && !tlw->testAttribute(Qt::WA_DontShowOnScreen)
//        && !tlw->hasHeightForWidth()) {
//        QPlatformWindow *platformWindow = tlw->windowHandle()->handle();
//        const QPoint topLevelPos = mapTo(tlw, ev->pos());
//        m_platformSizeGrip = platformWindow && platformWindow->startSystemResize(topLevelPos, mCorner);
//    }

//    if (m_platformSizeGrip)
//        return;

    // Find available desktop/workspace geometry.
    QRect availableGeometry;
    bool hasVerticalSizeConstraint = true;
    bool hasHorizontalSizeConstraint = true;
    if (tlw->isWindow())
        availableGeometry = QApplication::desktop()->availableGeometry(tlw);
    else {
        const QWidget *tlwParent = tlw->parentWidget();
        // Check if tlw is inside QAbstractScrollArea/QScrollArea.
        // If that's the case tlw->parentWidget() will return the viewport
        // and tlw->parentWidget()->parentWidget() will return the scroll area.
#ifndef QT_NO_SCROLLAREA
        QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea *>(tlwParent->parentWidget());
        if (scrollArea) {
            hasHorizontalSizeConstraint = scrollArea->horizontalScrollBarPolicy() == Qt::ScrollBarAlwaysOff;
            hasVerticalSizeConstraint = scrollArea->verticalScrollBarPolicy() == Qt::ScrollBarAlwaysOff;
        }
#endif
        availableGeometry = tlwParent->contentsRect();
    }

    // Find frame geometries, title bar height, and decoration sizes.
    const QRect frameGeometry = tlw->frameGeometry();
    const int titleBarHeight = qMax(tlw->geometry().y() - frameGeometry.y(), 0);
    const int bottomDecoration = qMax(frameGeometry.height() - tlw->height() - titleBarHeight, 0);
    const int leftRightDecoration = qMax((frameGeometry.width() - tlw->width()) / 2, 0);

    // Determine dyMax depending on whether the sizegrip is at the bottom
    // of the widget or not.
    if (atBottom()) {
        if (hasVerticalSizeConstraint)
            dyMax = availableGeometry.bottom() - mDlgRect.bottom() - bottomDecoration;
        else
            dyMax = INT_MAX;
    } else {
        if (hasVerticalSizeConstraint)
            dyMax = availableGeometry.y() - mDlgRect.y() + titleBarHeight;
        else
            dyMax = -INT_MAX;
    }

    // In RTL mode, the size grip is to the left; find dxMax from the desktop/workspace
    // geometry, the size grip geometry and the width of the decoration.
    if (atLeft()) {
        if (hasHorizontalSizeConstraint)
            dxMax = availableGeometry.x() - mDlgRect.x() + leftRightDecoration;
        else
            dxMax = -INT_MAX;
    } else {
        if (hasHorizontalSizeConstraint)
            dxMax = availableGeometry.right() - mDlgRect.right() - leftRightDecoration;
        else
            dxMax = INT_MAX;
    }
}

void SizeGrip::mouseReleaseEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton) {
        mGotMousePress = false;
        mStartPos = QPoint();
    }

    QSizeGrip::mouseReleaseEvent(ev);
}

void SizeGrip::mouseMoveEvent(QMouseEvent* ev)
{
    if (ev->buttons() != Qt::LeftButton) {
        QWidget::mouseMoveEvent(ev);
        return;
    }

    QWidget* tlw = mParentWindow;

    if (!mGotMousePress || tlw->testAttribute(Qt::WA_WState_ConfigPending)) {
        return;
    }


    QPoint np(ev->globalPos());

    const int dx        = np.x() - mStartPos.x();
    const int dx2       = mResizeDirections.testFlag(ResizeMirrorHorizontal) ? dx * 2    : dx;
    const int dxMax2    = mResizeDirections.testFlag(ResizeMirrorHorizontal) ? dxMax * 2 : dxMax;
    const int dy        = np.y() - mStartPos.y();
    const int dy2       = mResizeDirections.testFlag(ResizeMirrorVertical) ? dy * 2    : dy;
    const int dyMax2    = mResizeDirections.testFlag(ResizeMirrorVertical) ? dyMax * 2 : dyMax;

    // Don't extend beyond the available geometry; bound to dyMax and dxMax.
    QSize ns(atLeft()   ? mDlgRect.width() - qMax(dx2, dxMax2)
                        : mDlgRect.width() + qMin(dx2, dxMax2),
             atBottom() ? mDlgRect.height() + qMin(dy2, dyMax2)
                        : mDlgRect.height() - qMax(dy2, dyMax2));

    ns = QLayout::closestAcceptableSize(tlw, ns);

    QPoint p;
    QRect nr(p, ns);
    if (atBottom()) {
        nr.moveTop(mResizeDirections.testFlag(ResizeMirrorVertical)
                   ? qMin(mDlgRect.top(), mDlgRect.top() - qMin(dy, dyMax))
                   : mDlgRect.top());
    }
    else {
        nr.moveBottom(mResizeDirections.testFlag(ResizeMirrorVertical)
                      ? qMax(mDlgRect.bottom(), mDlgRect.bottom() - qMax(dy, dyMax))
                      : mDlgRect.bottom());
    }

    if (atLeft()) {
        nr.moveRight(mResizeDirections.testFlag(ResizeMirrorHorizontal)
                     ? qMax(mDlgRect.right(), mDlgRect.right() - qMax(dx, dxMax))
                     : mDlgRect.right());
    }
    else {
        nr.moveLeft(mResizeDirections.testFlag(ResizeMirrorHorizontal)
                    ? qMin(mDlgRect.left(), mDlgRect.left() - qMin(dx, dxMax))
                    : mDlgRect.left());
    }

    tlw->setGeometry(nr);
}

}

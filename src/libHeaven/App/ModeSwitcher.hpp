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

#ifndef HEAVEN_APP_MODE_SWITCHER_HPP
#define HEAVEN_APP_MODE_SWITCHER_HPP

#include <QHash>
#include <QPointer>
#include <QStringList>

#include "CentralUI/States/WindowStateRoot.hpp"

namespace Heaven
{
    class Application;
    class WindowStateRoot;
    class HeavenWindow;

    class AbstractViewWidget;
    class ContainerWidget;
    class View;

    class WindowStateView;
    class WindowStateSplitter;
    class WindowStateTab;
    class WindowStateWindow;

    class ModeSwitcher
    {
    public:
        ModeSwitcher( WindowStateRoot::Ptr state );

    public:
        void run();

    private:
        void findOpened();
        void killUnused();

        void synchronizeWindows();
        void synchronizeWindow( HeavenWindow* window, WindowStateWindow* state );
        void synchronizeContainer( ContainerWidget* container, WindowState* state );

        AbstractViewWidget* grabView( WindowStateView* view );
        AbstractViewWidget* grabSplitter( WindowStateSplitter* splitter );
        AbstractViewWidget* grabTab( WindowStateTab* tab );

        void cleanUpContainer( ContainerTypes unlessType, ContainerWidget*& cw );

        void associateViewContainer(AbstractViewWidget* view, WindowState* wsView);

    private:
        WindowStateRoot::Ptr    mState;

        QHash< ViewIdentifier, QPointer<View> > mExistingViews;
        QHash< ViewIdentifier, QPointer<ContainerWidget> > mExistingContainers;
        QHash< ViewIdentifier, QPointer<HeavenWindow> > mExistingWindows;
    };

}

#endif

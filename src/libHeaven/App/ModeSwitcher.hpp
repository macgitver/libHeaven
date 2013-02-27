
#ifndef HEAVEN_APP_MODE_SWITCHER_HPP
#define HEAVEN_APP_MODE_SWITCHER_HPP

#include <QHash>
#include <QStringList>

#include "CentralUI/WindowState.hpp"

namespace Heaven
{
    class Application;
    class WindowStateRoot;
    class HeavenWindow;

    class AbstractViewWidget;
    class ContainerWidget;

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
        void synchronizeContainer( ContainerWidget* container, WindowStateBase* state );

        AbstractViewWidget* grabView( WindowStateView* view );
        AbstractViewWidget* grabSplitter( WindowStateSplitter* splitter );
        AbstractViewWidget* grabTab( WindowStateTab* tab );

        void cleanUpContainer( ContainerTypes unlessType, ContainerWidget*& cw );

        void associateViewContainer( AbstractViewWidget* view, WindowStateBase* ws );

    private:
        WindowStateRoot::Ptr    mState;

        QHash< QString, View* > mExistingViews;
        QHash< QString, ContainerWidget* > mExistingContainers;
        QHash< QString, HeavenWindow* > mExistingWindows;
    };

}

#endif

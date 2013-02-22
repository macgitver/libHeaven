
#ifndef HEAVEN_APP_MODE_SWITCHER_HPP
#define HEAVEN_APP_MODE_SWITCHER_HPP

#include <QHash>
#include <QStringList>

#include "Views/WindowState.hpp"

namespace Heaven
{
    class Application;
    class WindowStateRoot;
    class HeavenWindow;

    class ModeSwitcher
    {
    public:
        ModeSwitcher( Application* app, WindowStateRoot::Ptr state );

    public:
        void run();

    private:
        void findOpened();
        void killUnused();

        void synchronizeWindows();
        void synchronizeWindow( HeavenWindow* window, WindowStateWindow* state );
        void synchronizeContainer( ViewContainer* container, WindowStateBase* state );

        ViewContainerContent* grabView( WindowStateView* view );
        ViewContainerContent* grabSplitter( WindowStateSplitter* splitter );
        ViewContainerContent* grabTab( WindowStateTab* tab );

    private:
        Application*            mApp;
        WindowStateRoot::Ptr    mState;

        QStringList             mOpened;
        QStringList             mReused;
        QHash< QString, View* > mExistingViews;
        QHash< QString, ViewContainerContent* > mExistingContainers;
        QHash< QString, HeavenWindow* > mExistingWindows;
    };

}

#endif

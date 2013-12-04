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

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

#include "libHeavenActions/UiContainer.hpp"
#include "libHeavenActions/UiManager.hpp"
#include "libHeavenActions/ActionContainerPrivate.hpp"
#include "libHeavenActions/MenuPrivate.hpp"
#include "libHeavenActions/ActionPrivate.hpp"
#include "libHeavenActions/ActionGroupPrivate.hpp"
#include "libHeavenActions/WidgetActionPrivate.hpp"
#include "libHeavenActions/MergePlacePrivate.hpp"
#include "libHeavenActions/Separator.hpp"
#include "libHeavenActions/MergesManager.hpp"
#include "libHeavenActions/WidgetActionWrapper.hpp"
#include "libHeavenActions/DynamicActionMergerPrivate.hpp"

namespace Heaven
{

    UiContainer::UiContainer( QObject* owner )
        : UiObjectPrivate( owner )
        , mDirty( false )
    {
    }

    UiContainer::~UiContainer()
    {
    }

    void UiContainer::add( UiObject* uio )
    {
        add( uio->mPrivate );
    }

    void UiContainer::add( UiObjectPrivate* uio )
    {
        mContent.append( uio );
        setContainerDirty();
    }

    void UiContainer::remove( UiObjectPrivate* uio )
    {
        for( int i = 0; i < mContent.count(); i++ )
        {
            if( mContent[ i ] == uio )
            {
                mContent.removeAt( i );
                i--;
                setContainerDirty();
            }
        }
    }

    int UiContainer::numObjects() const
    {
        return mContent.count();
    }

    UiObjectPrivate* UiContainer::objectAt( int index )
    {
        return mContent[ index ];
    }

    QList< UiObjectPrivate* > UiContainer::allObjects() const
    {
        return mContent;
    }

    bool UiContainer::isContainerDirty() const
    {
        return mDirty;
    }

    void UiContainer::setContainerDirty( bool value )
    {
        mDirty = value;
    }

    int UiContainer::priority() const
    {
        return -1;
    }

    bool UiContainer::mergeInto( QMenu* menu )
    {
        MenuPrivate* menuPriv;
        ActionPrivate* actionPriv;
        ActionGroupPrivate* actgrpPriv;
        WidgetActionPrivate* widgetActPriv;
        DynamicActionMergerPrivate* damPriv;
        //ActionContainerPrivate* containerPriv;
        //MergePlacePrivate* mergePlacePriv;

        foreach( UiObjectPrivate* uio, allObjects() )
        {
            QAction* action;

            switch( uio->type() )
            {
            case MenuType:
                menuPriv = qobject_cast< MenuPrivate* >( uio );
                Q_ASSERT( menuPriv );
                action = menuPriv->getOrCreateQMenu( menu )->menuAction();
                menu->addAction( action );
                break;

            case ActionType:
                actionPriv = qobject_cast< ActionPrivate* >( uio );
                Q_ASSERT( actionPriv );
                action = actionPriv->getOrCreateQAction( menu );
                menu->addAction( action );
                break;

            case ActionGroupType:
                actgrpPriv = qobject_cast< ActionGroupPrivate* >(uio);
                Q_ASSERT(actgrpPriv);
                menu->addActions(actgrpPriv->groupForParent(menu)->actions());
                break;

            case SeparatorType:
                menu->addSeparator();
                break;

            case WidgetActionType:
                widgetActPriv = qobject_cast< WidgetActionPrivate* >( uio );
                Q_ASSERT( widgetActPriv );
                // We don't have to create several widget actions
                menu->addAction( widgetActPriv->wrapper() );
                break;

            case ContainerType:
                ((UiContainer*)uio)->mergeInto( menu );
                break;

            case DynamicActionMergerType:
                damPriv = qobject_cast< DynamicActionMergerPrivate* >( uio );
                Q_ASSERT( damPriv );
                damPriv->addActionsTo( menu );
                break;

            case MergePlaceType:
                Q_ASSERT_X( false, "UiContainer", "MergePlace into Menu is unsupported!" );
                break;

            case MenuBarType:
            case ToolBarType:
                Q_ASSERT_X( false, "UiContainer", "Cannot merge bars into Menus!" );
                break;
            }
        }
        return true;
    }


    bool UiContainer::mergeInto( QMenuBar* menuBar )
    {
        MenuPrivate* menuPriv;
        ActionPrivate* actionPriv;
        ActionGroupPrivate* actgrpPriv;
        WidgetActionPrivate* widgetActPriv;
        //ActionContainerPrivate* containerPriv;
        //MergePlacePrivate* mergePlacePriv;

        foreach( UiObjectPrivate* uio, allObjects() )
        {
            QAction* action;

            switch( uio->type() )
            {
            case MenuType:
                menuPriv = qobject_cast< MenuPrivate* >( uio );
                Q_ASSERT( menuPriv );
                action = menuPriv->getOrCreateQMenu( menuBar )->menuAction();
                menuBar->addAction( action );
                break;

            case ActionType:
                actionPriv = qobject_cast< ActionPrivate* >( uio );
                Q_ASSERT( actionPriv );
                action = actionPriv->getOrCreateQAction( menuBar );
                menuBar->addAction( action );
                break;

            case ActionGroupType:
                actgrpPriv = qobject_cast< ActionGroupPrivate* >(uio);
                Q_ASSERT(actgrpPriv);
                menuBar->addActions(actgrpPriv->groupForParent(menuBar)->actions());
                break;

            case SeparatorType:
                menuBar->addSeparator();
                break;

            case WidgetActionType:
                widgetActPriv = qobject_cast< WidgetActionPrivate* >( uio );
                Q_ASSERT( widgetActPriv );
                // We don't have to create several widget actions
                menuBar->addAction( widgetActPriv->wrapper() );
                break;

            case ContainerType:
                ((UiContainer*)uio)->mergeInto( menuBar );
                break;

            case DynamicActionMergerType:
                Q_ASSERT_X( false, "UiContainer", "Merging DAMs into Menubar is unsupported!" );
                break;

            case MergePlaceType:
                Q_ASSERT_X( false, "UiContainer", "MergePlace into Toolbar is unsupported!" );
                break;

            case MenuBarType:
            case ToolBarType:
                Q_ASSERT_X( false, "UiContainer", "Cannot merge bars into bars!" );
                break;
            }

        }
        return true;
    }

    bool UiContainer::mergeInto( QToolBar* toolBar )
    {
        MenuPrivate* menuPriv;
        ActionPrivate* actionPriv;
        ActionGroupPrivate* actgrpPriv;
        WidgetActionPrivate* widgetActPriv;
//		ActionContainerPrivate* containerPriv;
//		MergePlacePrivate* mergePlacePriv;

        foreach( UiObjectPrivate* uio, allObjects() )
        {
            QAction* action;

            switch( uio->type() )
            {
            case MenuType:
                menuPriv = qobject_cast< MenuPrivate* >( uio );
                Q_ASSERT( menuPriv );
                action = menuPriv->getOrCreateQMenu( toolBar )->menuAction();
                toolBar->addAction( action );
                break;

            case ActionType:
                actionPriv = qobject_cast< ActionPrivate* >( uio );
                Q_ASSERT( actionPriv );
                action = actionPriv->getOrCreateQAction( toolBar );
                toolBar->addAction( action );
                break;

            case ActionGroupType:
                actgrpPriv = qobject_cast< ActionGroupPrivate* >(uio);
                Q_ASSERT(actgrpPriv);
                toolBar->addActions(actgrpPriv->groupForParent(toolBar)->actions());
                break;

            case WidgetActionType:
                widgetActPriv = qobject_cast< WidgetActionPrivate* >( uio );
                Q_ASSERT( widgetActPriv );
                // We don't have to create several widget actions
                toolBar->addAction( widgetActPriv->wrapper() );
                break;

            case SeparatorType:
                toolBar->addSeparator();
                break;

            case ContainerType:
                ((UiContainer*)uio)->mergeInto( toolBar );
                break;

            case DynamicActionMergerType:
                Q_ASSERT_X( false, "UiContainer", "Merging DAMs into Toolbar is unsupported!" );
                break;

            case MergePlaceType:
                Q_ASSERT_X( false, "UiContainer", "MergePlace into Toolbar is unsupported!" );
                break;

            case MenuBarType:
            case ToolBarType:
                Q_ASSERT_X( false, "UiContainer", "Cannot merge bars into bars!" );
                break;
            }

        }
        return true;
    }

    bool UiContainer::hasDynamicContent() const
    {
        foreach( UiObjectPrivate* uiop, allObjects() )
        {
            if( uiop->type() == DynamicActionMergerType )
            {
                return true;
            }
        }

        return false;
    }

    QList< UiContainer* > UiContainer::pathTo( UiObjectPrivate* child )
    {
        QList< UiContainer* > result;

        if( this != child )
        {
            foreach( UiObjectPrivate* cur, mContent )
            {
                if( cur == child )
                {
                    // direct child, return empty
                    return result;
                }

                UiObjectTypes type = cur->type();
                if( type == ToolBarType || type == MenuType || type == MenuBarType )
                {
                    UiContainer* sub = static_cast< UiContainer* >( cur );
                    QList< UiContainer* > subPath = sub->pathTo( child );
                    if( !subPath.isEmpty() )
                    {
                        result.append( this );
                        result.append( subPath );
                        break;
                    }
                }
            }
        }

        return result;
    }

}

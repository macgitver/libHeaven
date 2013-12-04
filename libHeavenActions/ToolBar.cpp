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

#include <QToolBar>
#include <QMenu>

#include "libHeavenActions/ToolBarPrivate.hpp"
#include "libHeavenActions/MenuPrivate.hpp"
#include "libHeavenActions/ActionPrivate.hpp"
#include "libHeavenActions/ActionGroupPrivate.hpp"
#include "libHeavenActions/ActionContainerPrivate.hpp"
#include "libHeavenActions/MergePlacePrivate.hpp"
#include "libHeavenActions/Separator.hpp"
#include "libHeavenActions/UiManager.hpp"
#include "libHeavenActions/MergesManager.hpp"

namespace Heaven
{

    ToolBarPrivate::ToolBarPrivate( ToolBar* owner )
        : UiContainer( owner )
        , mRebuildQueued( false )
    {
    }

    ToolBarPrivate::~ToolBarPrivate()
    {
    }

    QToolBar* ToolBarPrivate::createQToolBar( QWidget* forParent )
    {
        QToolBar* bar = new QToolBar( forParent );

        connect( bar, SIGNAL(destroyed()), this, SLOT(qtoolbarDestroyed()) );

        setContainerDirty();

        mToolBars.insert( bar );

        #if 0
        qDebug( "TB(%p) - Created QToolBar(%p) for QWidget(%p)", owner(), bar, forParent );
        #endif

        UiManager::self()->addCreatedObject( bar, this );
        return bar;
    }

    QToolBar* ToolBarPrivate::getOrCreateQToolBar( QWidget* forParent )
    {
        foreach( QToolBar* bar, mToolBars )
        {
            if( bar->parent() == forParent )
            {
                return bar;
            }
        }

        return createQToolBar( forParent );
    }

    void ToolBarPrivate::qtoolbarDestroyed()
    {
        QToolBar* t = static_cast< QToolBar* >( sender() );
        if( mToolBars.remove( t ) )
        {
            #if 0
            qDebug( "TB(%p) - QToolBar (%p) was removed", owner(), t );
            #endif
        }
        else
        {
            #if 0
            qDebug( "TB(%p) - QToolBar (%p) should have been removed; but was not found",
                    owner(), t );
            #endif
        }

        UiManager::self()->removeCreatedObject( t );
    }

    void ToolBarPrivate::setContainerDirty( bool value )
    {
        UiContainer::setContainerDirty( value );
        if( value && !mRebuildQueued )
        {
            mRebuildQueued = true;
            QMetaObject::invokeMethod( this, "reemergeGuiElement", Qt::QueuedConnection );
        }
    }

    UiObjectTypes ToolBarPrivate::type() const
    {
        return ToolBarType;
    }

    void ToolBarPrivate::reemergeGuiElement()
    {
        MenuPrivate* menuPriv;
        ActionPrivate* actionPriv;
        ActionGroupPrivate* actgrpPriv;
        ActionContainerPrivate* containerPriv;
        MergePlacePrivate* mergePlacePriv;
        mRebuildQueued = false;

        foreach( QToolBar* myBar, mToolBars )
        {
            myBar->clear();

            foreach( UiObjectPrivate* uio, allObjects() )
            {
                QList< QAction* > actions;

                // TODO: Do this the other way round. ContainerType will be much easier that way.
                switch( uio->type() )
                {
                case MenuType:
                    menuPriv = qobject_cast< MenuPrivate* >( uio );
                    Q_ASSERT( menuPriv );
                    actions << menuPriv->getOrCreateQMenu( myBar )->menuAction();
                    myBar->addActions( actions );
                    break;

                case ActionType:
                    actionPriv = qobject_cast< ActionPrivate* >( uio );
                    Q_ASSERT( actionPriv );
                    actions << actionPriv->getOrCreateQAction( myBar );
                    myBar->addActions( actions );
                    break;

                case ActionGroupType:
                    actgrpPriv = qobject_cast< ActionGroupPrivate* >(uio);
                    Q_ASSERT(actgrpPriv);
                    myBar->addActions(actgrpPriv->groupForParent(myBar)->actions());
                    break;

                case SeparatorType:
                    myBar->addSeparator();
                    break;

                case ContainerType:
                    containerPriv = qobject_cast< ActionContainerPrivate* >( uio );
                    Q_ASSERT( containerPriv );
                    if( containerPriv->isContainerDirty() )
                    {
                        // TODO: Force reemerge
                    }
                    break;

                case MergePlaceType:
                    mergePlacePriv = qobject_cast< MergePlacePrivate* >( uio );
                    Q_ASSERT( mergePlacePriv );
                    MergesManager::self()->emerge( mergePlacePriv->mName, myBar );
                    break;

                case MenuBarType:
                case ToolBarType:
                    // Cannot merge Bars into Toolbars
                    Q_ASSERT( false );
                    break;

                case DynamicActionMergerType:
                    Q_ASSERT_X( false, "ToolBar", "Can't merge DAMs directly into bars" );
                    break;

                case WidgetActionType:
                    qDebug( "WidgetAction not supported!" );
                    Q_ASSERT( false );
                    break;
                }
            }
        }
    }

    ToolBar::ToolBar( QObject* parent )
        : UiObject( parent, new ToolBarPrivate( this ) )
    {
    }

    QToolBar* ToolBar::toolBarFor( QWidget* forParent )
    {
        UIOD(ToolBar);
        return d->getOrCreateQToolBar( forParent );
    }

    void ToolBar::add( UiObject* uio )
    {
        UIOD(ToolBar);
        d->add( uio );
    }

    void ToolBar::addSeparator()
    {
        UIOD(ToolBar);
        d->add( new Separator( this ) );
    }

}

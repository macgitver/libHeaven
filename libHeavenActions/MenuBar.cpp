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

#include <QMenuBar>

#include "libHeavenActions/MenuBarPrivate.hpp"
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

    MenuBarPrivate::MenuBarPrivate( MenuBar* owner )
        : UiContainer( owner )
        , mRebuildQueued( false )
    {
    }

    MenuBarPrivate::~MenuBarPrivate()
    {
    }

    QMenuBar* MenuBarPrivate::createQMenuBar( QWidget* forParent )
    {
        QMenuBar* bar = new QMenuBar( forParent );

        connect( bar, SIGNAL(destroyed()), this, SLOT(qmenubarDestroyed()) );

        setContainerDirty();

        mMenuBars.insert( bar );

        UiManager::self()->addCreatedObject( bar, this );
        return bar;
    }

    QMenuBar* MenuBarPrivate::getOrCreateQMenuBar( QWidget* forParent )
    {
        foreach( QMenuBar* bar, mMenuBars )
        {
            if( bar->parent() == forParent )
            {
                return bar;
            }
        }

        return createQMenuBar( forParent );
    }

    void MenuBarPrivate::qmenubarDestroyed()
    {
        mMenuBars.remove( static_cast< QMenuBar* >( sender() ) );

        UiManager::self()->removeCreatedObject( sender() );
    }

    void MenuBarPrivate::setContainerDirty( bool value )
    {
        UiContainer::setContainerDirty( value );
        if( value && !mRebuildQueued )
        {
            mRebuildQueued = true;
            QMetaObject::invokeMethod( this, "reemergeGuiElement", Qt::QueuedConnection );
        }
    }

    UiObjectTypes MenuBarPrivate::type() const
    {
        return MenuBarType;
    }

    void MenuBarPrivate::reemergeGuiElement()
    {
        MenuPrivate* menuPriv;
        ActionPrivate* actionPriv;
        ActionGroupPrivate* actgrpPriv;
        ActionContainerPrivate* containerPriv;
        MergePlacePrivate* mergePlacePriv;
        mRebuildQueued = false;

        foreach( QMenuBar* myBar, mMenuBars )
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

                case WidgetActionType:
                    // not implemented
                    Q_ASSERT( false );
                    break;

                case DynamicActionMergerType:
                    Q_ASSERT_X( false, "MenuBar", "Can't merge DAMs directly into bars" );
                    break;

                case MenuBarType:
                case ToolBarType:
                    // cannot merge these into a menubar
                    Q_ASSERT( false );
                    break;
                }
            }
        }
    }

    MenuBar::MenuBar( QObject* parent )
        : UiObject( parent, new MenuBarPrivate( this ) )
    {
    }

    QMenuBar* MenuBar::menuBarFor( QWidget* forParent )
    {
        UIOD(MenuBar);
        return d->getOrCreateQMenuBar( forParent );
    }

    void MenuBar::add( UiObject* uio )
    {
        UIOD(MenuBar);
        d->add( uio );
    }

    void MenuBar::addSeparator()
    {
        UIOD(MenuBar);
        d->add( new Separator( this ) );
    }

}

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
#include <QQueue>
#include <QAction>

#include "libHeavenActions/MenuPrivate.hpp"
#include "libHeavenActions/ActionPrivate.hpp"
#include "libHeavenActions/ActionGroupPrivate.hpp"
#include "libHeavenActions/DynamicActionMerger.hpp"
#include "libHeavenActions/DynamicActionMergerPrivate.hpp"
#include "libHeavenActions/ActionContainerPrivate.hpp"
#include "libHeavenActions/MergePlacePrivate.hpp"
#include "libHeavenActions/Separator.hpp"
#include "libHeavenActions/UiManager.hpp"
#include "libHeavenActions/MergesManager.hpp"

namespace Heaven
{

    MenuPrivate::MenuPrivate( Menu* owner )
        : UiContainer( owner )
        , mEnabled( true )
        , mRebuildQueued( false )
    {
    }

    MenuPrivate::~MenuPrivate()
    {
    }

    QMenu* MenuPrivate::createQMenu( QWidget* forParent )
    {
        QMenu* menu = new QMenu( forParent );

        menu->setTitle( mText );
        menu->setEnabled( mEnabled );

        QAction* action = menu->menuAction();
        action->setToolTip( mToolTip );
        action->setStatusTip( mStatusTip );

        setContainerDirty();

        connect( menu, SIGNAL(destroyed()), this, SLOT(menuDestroyed()) );
        connect( menu, SIGNAL(aboutToShow()), this, SLOT(menuAboutToShow()) );

        mQMenus.insert( menu );

        #if 0
        qDebug( "MU(%p) - Created QMenu(%p) for QWidget(%p)", owner(), menu, forParent );
        #endif

        UiManager::self()->addCreatedObject( menu, this );
        return menu;
    }

    QMenu* MenuPrivate::getOrCreateQMenu( QWidget* forParent )
    {
        foreach( QMenu* menu, mQMenus )
        {
            if( menu->parent() == forParent )
            {
                return menu;
            }
        }

        return createQMenu( forParent );
    }

    void MenuPrivate::setText( const QString& text )
    {
        mText = text;

        foreach( QMenu* menu, mQMenus )
        {
            menu->setTitle( text );
        }
    }

    void MenuPrivate::setStatusTip( const QString& text )
    {
        mStatusTip = text;

        foreach( QMenu* menu, mQMenus )
        {
            menu->menuAction()->setStatusTip( text );
        }
    }

    void MenuPrivate::setToolTip( const QString& text )
    {
        mToolTip = text;

        foreach( QMenu* menu, mQMenus )
        {
            menu->menuAction()->setToolTip( text );
        }
    }

    void MenuPrivate::setEnabled( bool v )
    {
        mEnabled = v;

        foreach( QMenu* menu, mQMenus )
        {
            menu->setEnabled( v );
        }
    }

    void MenuPrivate::menuAboutToShow()
    {
        if( hasDynamicContent() )
        {
            reemergeGuiElement();
        }
    }

    void MenuPrivate::menuDestroyed()
    {
        QMenu* menu = static_cast< QMenu* >( sender() );
        if( mQMenus.remove( menu ) )
        {
            #if 0
            qDebug( "MU(%p) - QMenu (%p) was removed", owner(), menu );
            #endif
        }
        else
        {
            #if 0
            qDebug( "MU(%p) - QMenu (%p) should have been removed; but was not found",
                    owner(), menu );
            #endif
        }

        UiManager::self()->removeCreatedObject( menu );
    }

    void MenuPrivate::reemergeGuiElement()
    {
        MenuPrivate* menuPriv;
        ActionPrivate* actionPriv;
        ActionGroupPrivate* actgrpPriv;
        ActionContainerPrivate* containerPriv;
        MergePlacePrivate* mergePlacePriv;
        DynamicActionMergerPrivate* damPriv;

        mRebuildQueued = false;

        foreach( QMenu* myMenu, mQMenus )
        {
            #if 0
            qDebug( "MU(%p) - Reemerge QMenu(%p)", owner(), myMenu );
            #endif
            myMenu->clear();

            QQueue< UiObjectPrivate* > todos;
            foreach (UiObjectPrivate* uio, allObjects()) {
                todos.enqueue(uio);
                if (uio->type() == ContainerType) {
                    UiContainer* cc = static_cast< UiContainer* >(uio);
                    foreach (UiObjectPrivate* uio2, cc->allObjects()) {
                        todos.enqueue(uio2);
                    }
                }
            }

            while( !todos.isEmpty() )
            {
                UiObjectPrivate* uio = todos.dequeue();
                QList< QAction* > actions;

                // TODO: Do this the other way round. ContainerType will be much easier that way.
                switch( uio->type() )
                {
                case MenuType:
                    menuPriv = qobject_cast< MenuPrivate* >( uio );
                    Q_ASSERT( menuPriv );
                    actions << menuPriv->getOrCreateQMenu( myMenu )->menuAction();
                    myMenu->addActions( actions );
                    break;

                case ActionType:
                    actionPriv = qobject_cast< ActionPrivate* >( uio );
                    Q_ASSERT( actionPriv );
                    actions << actionPriv->getOrCreateQAction( myMenu );
                    myMenu->addActions( actions );
                    break;

                case ActionGroupType:
                    actgrpPriv = qobject_cast< ActionGroupPrivate* >(uio);
                    Q_ASSERT(actgrpPriv);
                    myMenu->addActions(actgrpPriv->groupForParent(myMenu)->actions());
                    break;

                case SeparatorType:
                    myMenu->addSeparator();
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
                    MergesManager::self()->emerge( mergePlacePriv->mName, myMenu );
                    break;

                case WidgetActionType:
                    qDebug( "WidgetAction not supported!" );
                    Q_ASSERT( false );
                    break;

                case DynamicActionMergerType:
                    damPriv = qobject_cast< DynamicActionMergerPrivate* >( uio );
                    Q_ASSERT( damPriv );
                    damPriv->addActionsTo( myMenu );
                    break;

                case ToolBarType:
                case MenuBarType:
                    // Cannot merge bars into a menu
                    Q_ASSERT( false );
                    break;
                }
            }
        }
    }

    void MenuPrivate::setContainerDirty( bool value )
    {
        UiContainer::setContainerDirty( value );
        if( value && !mRebuildQueued )
        {
            mRebuildQueued = true;
            QMetaObject::invokeMethod( this, "reemergeGuiElement", Qt::QueuedConnection );
        }
    }

    UiObjectTypes MenuPrivate::type() const
    {
        return MenuType;
    }

    Menu::Menu( QObject* parent )
        : UiObject( parent, new MenuPrivate( this ) )
    {
    }

    QString Menu::text() const
    {
        UIOD(const Menu);
        return d->mText;
    }

    QString Menu::statusTip() const
    {
        UIOD(const Menu);
        return d->mStatusTip;
    }

    QString Menu::toolTip() const
    {
        UIOD(const Menu);
        return d->mToolTip;
    }

    bool Menu::isEnabled() const
    {
        UIOD(const Menu);
        return d->mEnabled;
    }

    void Menu::setText( const QString& text )
    {
        UIOD(Menu);
        if( text != d->mText )
        {
            d->setText( text );
        }
    }

    void Menu::setStatusTip( const QString& text )
    {
        UIOD(Menu);
        if( text != d->mStatusTip )
        {
            d->setStatusTip( text );
        }
    }

    void Menu::setToolTip( const QString& text )
    {
        UIOD(Menu);
        if( text != d->mToolTip )
        {
            d->setToolTip( text );
        }
    }

    void Menu::setStatusToolTip( const QString& text )
    {
        setStatusTip( text );
        setToolTip( text );
    }

    void Menu::setEnabled( bool v )
    {
        UIOD(Menu);
        d->setEnabled( v );
    }

    void Menu::setDisabled( bool v )
    {
        setEnabled( !v );
    }

    void Menu::add( UiObject* uio )
    {
        UIOD(Menu);
        d->add( uio );
    }

    void Menu::addSeparator()
    {
        UIOD(Menu);
        d->add( new Separator( this ) );
    }

    void Menu::showPopup( const QPoint& globalPos )
    {
        UIOD(Menu);
        QMenu* m = d->getOrCreateQMenu( NULL );
        m->exec( globalPos );
        m->deleteLater();
    }

}

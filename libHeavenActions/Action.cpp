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

#include <QAction>

#include "libHeavenActions/ActionPrivate.hpp"
#include "libHeavenActions/ActionGroup.hpp"
#include "libHeavenActions/UiManager.hpp"

namespace Heaven
{

    ActionPrivate::ActionPrivate( Action* owner )
        : UiObjectPrivate( owner )
        , mVisible( true )
        , mEnabled( true )
        , mCheckable( false )
        , mChecked( false )
        , mShortcutContext( Qt::WindowShortcut )
        , mMenuRole( QAction::TextHeuristicRole )
        , mGroup(NULL)
    {
    }

    ActionPrivate::~ActionPrivate()
    {
    }

    void ActionPrivate::setText( const QString& text )
    {
        mText = text;

        foreach( QAction* act, mQActions )
        {
            act->setText( text );
        }
    }

    void ActionPrivate::setStatusTip( const QString& text )
    {
        mStatusTip = text;

        foreach( QAction* act, mQActions )
        {
            act->setStatusTip( text );
        }
    }

    void ActionPrivate::setToolTip( const QString& text )
    {
        mToolTip = text;

        foreach( QAction* act, mQActions )
        {
            act->setToolTip( text );
        }
    }

    void ActionPrivate::setEnabled( bool v )
    {
        mEnabled = v;

        foreach( QAction* act, mQActions )
        {
            act->setEnabled( v );
        }
    }

    void ActionPrivate::setChecked( bool v )
    {
        mChecked = v;

        foreach( QAction* act, mQActions )
        {
            act->setChecked( v );
        }
    }

    void ActionPrivate::setCheckable( bool v )
    {
        mCheckable = v;

        foreach( QAction* act, mQActions )
        {
            act->setCheckable( v );
        }
    }

    void ActionPrivate::setVisible( bool v )
    {
        mVisible = v;

        foreach( QAction* act, mQActions )
        {
            act->setVisible( v );
        }
    }

    void ActionPrivate::setIconRef( const IconRef& ref )
    {
        mIconRef = ref;
        mIcon = QIcon();
        updateIcons();
    }

    void ActionPrivate::setShortcut(const QString &shortcut)
    {
        mShortcut = QKeySequence::fromString(shortcut);

        foreach( QAction *act, mQActions )
        {
            act->setShortcut( mShortcut );
        }
    }

    void ActionPrivate::setShortcutContext(Qt::ShortcutContext context)
    {
        mShortcutContext = context;

        foreach( QAction *act, mQActions )
        {
            act->setShortcutContext( context );
        }
    }

    void ActionPrivate::setMenuRole( QAction::MenuRole role )
    {
        mMenuRole = role;

        foreach( QAction* act, mQActions )
        {
            // Will this actually move things around?
            act->setMenuRole( role );
        }
    }

    void ActionPrivate::qactionDestroyed()
    {
        QAction* act = static_cast< QAction* >( sender() );
        if( mQActions.remove( act ) )
        {
            #if 0
            qDebug( "AC(%p) - QAction (%p) was removed", owner(), act );
            #endif
        }
        else
        {
            #if 0
            qDebug( "AC(%p) - QAction (%p) should have been removed; but was not found",
                    owner(), act );
            #endif
        }

        UiManager::self()->removeCreatedObject( act );
    }

    void ActionPrivate::qactionTriggered()
    {
        findActivationContext( sender() );

        emit triggered();
    }

    void ActionPrivate::qactionToggled( bool checked )
    {
        QAction* from = qobject_cast< QAction* >( sender() );

        foreach( QAction* act, mQActions )
        {
            if( act != from )
            {
                act->setChecked( checked );
            }
        }

        mChecked = checked;

        findActivationContext( from );

        emit toggled( checked );
    }

    QAction* ActionPrivate::createQAction( QObject* forParent )
    {
        QAction* a = new QAction( forParent );

        a->setText( mText );
        a->setStatusTip( mStatusTip );
        a->setToolTip( mToolTip );
        a->setEnabled( mEnabled );
        a->setCheckable( mCheckable );
        a->setChecked( mChecked );
        a->setVisible( mVisible );
        a->setShortcut( mShortcut );
        a->setShortcutContext( mShortcutContext );
        a->setMenuRole( mMenuRole );

        if( mIconRef.isValid() )
        {
            createIcon();
            a->setIcon( mIcon );
        }

        connect( a, SIGNAL(destroyed()), this, SLOT(qactionDestroyed()) );
        connect( a, SIGNAL(triggered()), this, SLOT(qactionTriggered()) );
        connect( a, SIGNAL(toggled(bool)), this, SLOT(qactionToggled(bool)) );

        mQActions.insert( a );
        #if 0
        qDebug( "AC(%p) - Created QAction(%p) for QWidget(%p)", owner(), a, forParent );
        #endif

        if (mGroup) {
            mGroup->groupForParent(forParent);
        }

        UiManager::self()->addCreatedObject( a, this );
        return a;
    }

    QAction* ActionPrivate::getOrCreateQAction( QObject* forParent )
    {
        foreach( QAction* act, mQActions )
        {
            if( act->parent() == forParent )
            {
                return act;
            }
        }

        return createQAction( forParent );
    }

    void ActionPrivate::createIcon()
    {
        if( mIcon.isNull() && mIconRef.isValid() )
        {
            mIcon = QIcon( mIconRef.icon().pixmap() );
        }
    }

    void ActionPrivate::updateIcons()
    {
        createIcon();
        foreach( QAction* act, mQActions )
        {
            act->setIcon( mIcon );
        }
    }

    UiObjectTypes ActionPrivate::type() const
    {
        return ActionType;
    }

    Action::Action( QObject* parent )
        : UiObject( parent, new ActionPrivate( this ) )
    {
        UIOD(Action);

        connect( d, SIGNAL(triggered()), this, SIGNAL(triggered()) );
        connect( d, SIGNAL(toggled(bool)), this, SIGNAL(toggled(bool)) );
    }

    QAction* Action::actionFor( QObject* parent )
    {
        UIOD(Action);

        return d->getOrCreateQAction( parent );
    }


    QString Action::text() const
    {
        UIOD(const Action);
        return d->mText;
    }

    QString Action::statusTip() const
    {
        UIOD(const Action);
        return d->mStatusTip;
    }

    QString Action::toolTip() const
    {
        UIOD(const Action);
        return d->mToolTip;
    }

    IconRef Action::iconRef() const
    {
        UIOD(const Action);
        return d->mIconRef;
    }

    QKeySequence Action::shortcut() const
    {
        UIOD(const Action);
        return d->mShortcutContext;
    }

    Qt::ShortcutContext Action::shortcutContext() const
    {
        UIOD(const Action);
        return d->mShortcutContext;
    }

    QAction::MenuRole Action::menuRole() const
    {
        UIOD(const Action);
        return d->mMenuRole;
    }

    bool Action::isEnabled() const
    {
        UIOD(const Action);
        return d->mEnabled;
    }

    bool Action::isCheckable() const
    {
        UIOD(const Action);
        return d->mCheckable;
    }

    bool Action::isChecked() const
    {
        UIOD(const Action);
        return d->mChecked;
    }

    bool Action::isVisible() const
    {
        UIOD(const Action);
        return d->mVisible;
    }

    void Action::setText( const QString& text )
    {
        UIOD(Action);
        if( text != d->mText )
        {
            d->setText( text );
        }
    }

    void Action::setStatusTip( const QString& text )
    {
        UIOD(Action);
        if( text != d->mStatusTip )
        {
            d->setStatusTip( text );
        }
    }

    void Action::setToolTip( const QString& text )
    {
        UIOD(Action);
        if( text != d->mToolTip )
        {
            d->setToolTip( text );
        }
    }

    void Action::setStatusToolTip( const QString& text )
    {
        setStatusTip( text );
        setToolTip( text );
    }

    void Action::setEnabled( bool v )
    {
        UIOD(Action);
        d->setEnabled( v );
    }

    void Action::setDisabled( bool v )
    {
        setEnabled( !v );
    }

    void Action::setChecked( bool v )
    {
        UIOD(Action);
        d->setChecked( v );
    }

    void Action::setCheckable( bool v )
    {
        UIOD(Action);
        d->setCheckable( v );
    }

    void Action::setVisible( bool v )
    {
        UIOD(Action);
        d->setVisible( v );
    }

    void Action::setIconRef( const QString& text )
    {
        UIOD(Action);
        d->setIconRef( IconRef::fromString( text ) );
    }

    void Action::setIconRef( const IconRef& ref )
    {
        UIOD(Action);
        d->setIconRef( ref );
    }

    void Action::setShortcut(const QString &shortcut)
    {
        UIOD(Action);
        d->setShortcut( shortcut );
    }

    void Action::setShortcutContext(Qt::ShortcutContext context)
    {
        UIOD(Action);
        d->setShortcutContext( context );
    }

    void Action::setMenuRole( QAction::MenuRole role )
    {
        UIOD(Action);
        d->setMenuRole( role );
    }

    void Action::setGroup(ActionGroup *group)
    {
        UIOD(Action);
        d->mGroup = group;
    }

    ActionGroup* Action::group() const
    {
        UIOD(const Action);
        return d->mGroup;
    }
}

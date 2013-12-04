/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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
#include <QWidget>

#include "libHeavenActions/DynamicActionMergerPrivate.hpp"

namespace Heaven
{

    /**
     * @enum        MergerMode
     * @ingroup     Actions
     * @brief       Operation modes of a DynamicActionMerger
     *
     * @var         MergerMode::DAMergerCallback
     *              This is a _active_ mode. Every time that the framework internally rebuilds the
     *              actions, a call back slot is invoked. It's task is to setup a list of actions
     *              that the framework will use in the place of the DynamicActionMerger.
     *
     * @var         MergerMode::DAMergerAdvancedList
     *              The advanced mode is more _passive_. The owner installs a list of actions or
     *              key/value pairs into the DAM and this list will be used in the place of the
     *              DynamicActionMerger.
     *
     * @enum        MergerActionLifetime
     * @ingroup     Actions
     * @brief       Lifetime of an action inside a DynamicActionMerger
     *
     * @var         MergerActionLifetime::DAMergerActionUserControlled
     *              The lift itme of the action is defined by the consumer. The DynamicActionMerger
     *              will not delete this action.
     *
     * @var         MergerActionLifetime::DAMergerActionMergerControlled
     *              The lift time of this action is managed by the DynamicActionMerger. It will
     *              delete the action as the replacement list is repopulated.
     *
     * @class       DynamicActionMerger
     * @ingroup     Actions
     * @brief       Add dynamic actions to an action structure
     *
     * A DynamicActionMerger (DAM) is used to populate a structure with dynamic actions. It has two
     * modes of operation:
     *
     * -   __Callback__:
     *     This is a _active_ mode. Every time that the framework internally rebuilds the actions, a
     *     call back slot is invoked. It's task is to setup a list of actions that the framework
     *     will use in the place of the DynamicActionMerger.
     *
     * -   __Advanced__:
     *     The advanced mode is more _passive_. The owner installs a list of actions or key/value
     *     pairs into the DAM and this list will be used in the place of the DynamicActionMerger.
     *
     * The mode can be set with setMode(), the merger slot can be set via setMergerSlot().
     *
     * As with all UiObject classes, a DAM is typically not created directly but rather through the
     * help of a @ref md_HID "HID file".
     *
     * @fn          DynamicActionMerger::entryTriggered( QVariant value )
     * @brief       Emitted when an action got triggered
     *
     * @param[in]   value   The value that was used to add the action that has been triggered.
     *
     * This signal is emitted when an action got triggered.
     *
     */

    DynamicActionMergerPrivate::DynamicActionMergerPrivate( DynamicActionMerger* owner )
        : UiObjectPrivate( owner )
    {
        mMode = DAMergerCallback;
    }

    DynamicActionMergerPrivate::~DynamicActionMergerPrivate()
    {
        freeActionList();
    }

    void DynamicActionMergerPrivate::freeActionList()
    {
        foreach( ActionListEntry ale, mActions )
        {
            if( ale.mAction )
            {
                ale.mAction->disconnect( this );
            }

            if( ale.mLifetime == DAMergerActionMergerControlled )
            {
                delete ale.mAction;
            }
        }
        mActions.clear();
    }

    UiObjectTypes DynamicActionMergerPrivate::type() const
    {
        return DynamicActionMergerType;
    }

    void DynamicActionMergerPrivate::addActionsTo( QWidget* widget )
    {
        static_cast< DynamicActionMerger* >( mOwner )->triggerRebuild();

        foreach( ActionListEntry ale, mActions )
        {
            widget->addAction( ale.mAction );
        }
    }

    void DynamicActionMergerPrivate::onActionTriggered()
    {
        foreach( ActionListEntry le, mActions )
        {
            if( sender() == le.mAction )
            {
                DynamicActionMerger* dam = static_cast< DynamicActionMerger* >( mOwner );
                dam->entryTriggered( le.mValue );
                return;
            }
        }
    }

    /**
     * @brief       Constructor
     *
     * @param[in]   parent      The parent in QObject object hierarchy.
     *
     * Sets the mode to DAMergerCallback.
     *
     */
    DynamicActionMerger::DynamicActionMerger( QObject* parent )
        : UiObject( parent, new DynamicActionMergerPrivate( this ) )
    {
    }

    /**
     * @brief       Rebuild the action list
     *
     * This slot is either involked internally or can be explicitly called from consumer code.
     *
     * If the current mode() is DAMergerCallback, the internal action list is cleared and the merger
     * slot is invoked.
     *
     */
    void DynamicActionMerger::triggerRebuild()
    {
        UIOD(DynamicActionMerger);

        if( d->mMode == DAMergerCallback )
        {
            d->freeActionList();
            if( !d->mMergerSlot.isEmpty() )
            {
                QMetaObject::invokeMethod( parent(), d->mMergerSlot.constData(),
                                           Qt::DirectConnection,
                                           Q_ARG( Heaven::DynamicActionMerger*, this ) );
            }
        }
    }

    /**
     * @brief       Set the merger slots
     *
     * @param[in]   szSlot      An Qt-encoded slot signature. Use the `SLOT()` macro for encoding.
     *
     * The merger slot can also be set through the HID-Property `Merger`.
     *
     */
    void DynamicActionMerger::setMergerSlot( const char *szSlot )
    {
        UIOD(DynamicActionMerger);
        d->mMergerSlot = szSlot;
    }

    /**
     * @brief       Clear the internal action list
     *
     * For the mode() DAMergerCallback, this happens automatically. For DAMergerAdvancedList the
     * consumer code might want to clear out the list when it shall be regenerated from scratch.
     *
     */
    void DynamicActionMerger::clear()
    {
        UIOD(DynamicActionMerger);
        d->freeActionList();
    }

    /**
     * @brief       Add an action to the internal action list
     *
     * @param[in]   act         The QAction to add. If you specifiy `NULL` for this parameter, the
     *                          value will be converted into a string and an action will be created.
     *                          This also means that lifeTime will be implicitly set to
     *                          DAMergerActionMergerControlled.
     *
     * @param[in]   value       A `QVariant` that will be used as parameter to the entryTriggered()
     *                          signal when this action gets triggered.
     *
     * @param[in]   lifeTime    Describes the ownership of this action. The default value
     *                          (DAMergerActionUserControlled) means that the user is responsible
     *                          for handling destruction of the action.
     *                          DAMergerActionMergerControlled instead means that the merger will
     *                          take ownership of the action and delete it when the action list is
     *                          cleared the next time. This also causes the Merger to reparent the
     *                          action to itself.
     *
     */
    void DynamicActionMerger::addAction( QAction* act, const QVariant& value,
                                         MergerActionLifetime lifeTime )
    {
        UIOD(DynamicActionMerger);

        if( !act )
        {
            lifeTime = DAMergerActionMergerControlled;
            act = new QAction( value.toString(), this );
        }

        DynamicActionMergerPrivate::ActionListEntry le;
        le.mAction = act;
        le.mLifetime = lifeTime;
        le.mValue = value;

        connect( act, SIGNAL(triggered()), d, SLOT(onActionTriggered()),
                 Qt::UniqueConnection );

        if( lifeTime == DAMergerActionMergerControlled && act->parent() != this )
        {
            act->setParent( this );
        }

        d->mActions.append( le );
    }

    /**
     * @brief       Add the contents of a QStringList to the internal action list
     *
     * @param[in]   list    The QStringList to add.
     *
     * Each element of the string list will be used as display _and_ as value to be emitted from the
     * entryTriggered() singal.
     *
     */
    void DynamicActionMerger::addStringList( const QStringList& list )
    {
        foreach( QString s, list )
        {
            addAction( s, s );
        }
    }

    /**
     * @brief       Add an action to the merger.
     *
     * @param[in]   display     String to be used as display for the action.
     *
     * @param[in]   value       A `QVariant` that will be used as parameter to the entryTriggered()
     *                          signal when this action gets triggered.
     *
     * The action will be owned by the Merger.
     *
     */
    void DynamicActionMerger::addAction( const QString& display, const QVariant& value )
    {
        QAction* act = new QAction( display, this );
        addAction( act, value, DAMergerActionMergerControlled );
    }

    /**
     * @brief       Add a separator action to the internal action list
     *
     * The separator will be owned by the Merger and deleted as appropiate.
     *
     */
    void DynamicActionMerger::addSeparator()
    {
        UIOD(DynamicActionMerger);

        DynamicActionMergerPrivate::ActionListEntry le;
        le.mAction = new QAction( this );
        le.mAction->setSeparator( true );
        le.mLifetime = DAMergerActionMergerControlled;
        d->mActions.append( le );
    }

    /**
     * @brief       Set the merger mode
     *
     * @param[in]   mode    The new mode to set.
     *
     * After setting the merger mode to @a mode, the action list is cleared.
     */
    void DynamicActionMerger::setMode( MergerMode mode )
    {
        UIOD(DynamicActionMerger);
        d->mMode = mode;
        clear();
    }

    /**
     * @brief       Get current merger mode
     *
     * @return      The MergerMode that this DAM uses to populate the list of actions.
     *
     */
    MergerMode DynamicActionMerger::mode() const
    {
        UIOD(const DynamicActionMerger);
        return d->mMode;
    }

}

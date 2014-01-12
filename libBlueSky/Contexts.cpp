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

#include <QMap>
#include <QStringBuilder>

#include "libBlueSky/Contexts.hpp"
#include "libBlueSky/Internal/ContextManager.hpp"

namespace BlueSky
{

    /**
     * @class       ContextKeys
     * @brief       Keys identifiying a view-context
     *
     */

    class ContextKeys::Data : public QSharedData
    {
    public:
        Data( const ViewIdentifier& _viewId ) : viewId( _viewId ) {}

    public:
        QMap< QString, QString > keys;
        const ViewIdentifier     viewId;
    };

    /**
     * @brief       Constructor
     *
     * Constructs invalid context keys.
     */
    ContextKeys::ContextKeys()
    {
    }

    /**
     * @brief       Constructor
     *
     * @param[in]   viewId      ViewIdentifier for which this keys should be created.
     *
     * Creates a valid context keys object for the ViewIdentifier @a viewId.
     *
     */
    ContextKeys::ContextKeys( const ViewIdentifier& viewId )
    {
        d = new Data( viewId );
    }

    /**
     * @brief       Constructor ( Copy )
     *
     * @param[in]   other       ContextKeys to copy.
     *
     * Creates a context keys object from another one.
     *
     */
    ContextKeys::ContextKeys( const ContextKeys& other )
        : d( other.d )
    {
    }

    /**
     * @brief       Destructor
     *
     */
    ContextKeys::~ContextKeys()
    {
    }

    /**
     * @brief       Assignment Operator
     *
     * @param[in]   other   ContextKeys to assgin
     *
     * @return      A reference to this.
     *
     * Copys the @a other ContextKeys into this object.
     *
     */
    ContextKeys& ContextKeys::operator=( const ContextKeys& other )
    {
        d = other.d;
        return *this;
    }

    /**
     * @brief       Compare Equality Operator
     *
     * @param[in]   other   ContextKeys to compare against
     *
     * @return      `true` if this Context Keys object is equal to the @a other context keys object,
     *              `false` otherwise. Two context keys objects are considered equal, if they are
     *              actually the same object or thier viewName and all key/values are identical.
     */
    bool ContextKeys::operator==( const ContextKeys& other ) const
    {
        return (d == other.d ) ||
                ( d->viewId == other.d->viewId &&
                  d->keys == other.d->keys );
    }

    /**
     * @brief       Is this context keys object valid
     *
     * @return      `true`, if this context keys objcect is valid, `false` otherwise.
     *
     */
    bool ContextKeys::isValid() const
    {
        return !!d; // = bool::operator!( QExpli...::operator!() )
    }

    /**
     * @brief       Create a readable and hashable string
     *
     * @return      A string that uniquely describes this context keys object
     *
     * An empty string for invalid context keys objects; for valid context key objects: the view
     * identifier followed by a colon followed by an optional list of key value pairs. Each key
     * value pair consists of the key, an equal sign, the value and a semicolon. No escaping is
     * performed. The key value list is in arbitrary, but stable order.
     *
     */
    QString ContextKeys::toString() const
    {
        if( !isValid() )
        {
            return QString();
        }

        QString s = d->viewId.toString() % QChar( L':' );

        foreach( QString keyName, d->keys.keys() )
        {
            s = s % keyName % QChar( L'=' ) % d->keys[ keyName ] % QChar( L';' );
        }

        return s;
    }

    /**
     * @brief       Get the View Identifier
     *
     * @return      View Identifer to which this context keys object applies.
     *
     */
    ViewIdentifier ContextKeys::viewId() const
    {
        return isValid() ? d->viewId : ViewIdentifier();
    }

    /**
     * @brief       Index operator
     *
     * @param[in]   pszKey      Ascii key to fetch
     *
     * @return      The value stored under the key @a pszKey.
     *
     */
    QString ContextKeys::operator[]( const char* pszKey ) const
    {
        return get( pszKey );
    }

    /**
     * @brief       Index operator
     *
     * @param[in]   key     The key to fetch
     *
     * @return      The value stored under the key @a key.
     *
     */
    QString ContextKeys::operator[]( const QString& key ) const
    {
        return get( key );
    }

    /**
     * @brief       Get a value
     *
     * @param[in]   pszKey      Ascii key to fetch
     *
     * @return      The value stored under the key @a pszKey.
     *
     */
    QString ContextKeys::get( const char* pszKey ) const
    {
        return get( QString::fromLatin1( pszKey ) );
    }

    /**
     * @brief       Get a value
     *
     * @param[in]   key     The key to fetch
     *
     * @return      The value stored under the key @a key.
     *
     */
    QString ContextKeys::get( const QString& key ) const
    {
        if( isValid() )
        {
            return d->keys.value( key, QString() );
        }
        return QString();
    }

    /**
     * @brief[in]   Change a value
     *
     * @param[in]   pszKey      Ascii key to change
     *
     * @param[in]   value       The new value to store for the key @a pszKey.
     *
     */
    void ContextKeys::set( const char* pszKey, const QString& value )
    {
        set( QString::fromLatin1( pszKey ), value );
    }

    /**
     * @brief[in]   Change a value
     *
     * @param[in]   key         Key to change
     *
     * @param[in]   value       The new value to store for the key @a key.
     *
     */
    void ContextKeys::set( const QString& key, const QString& value )
    {
        Q_ASSERT( isValid() );
        d->keys[ key ] = value;
    }

    /**
     * @class   ViewContext
     * @ingroup CentralUI
     * @brief   A context that a view provides to other views
     *
     * A ContextView is able to provide and to consume contexts. A context represents the current
     * state of inter-dependent views.
     *
     * A master view is said to _provide_ a context and this context will automatically be attached
     * to slave views that have associated themselves with the master. They _consume_ the context.
     *
     * A _context consumer_ has the ability to store additional data into the context. This can be
     * achieved through ViewContextData objects.
     *
     * When a context and/or associated data is no longer in active use, it will be dropped after
     * a grace period.
     *
     * Contexts are identified by unique keys (ContextKeys objects). These keys always contain the
     * identifier of the ContextView to which they belong and optional as many key-value pairs as
     * required to make them unique.
     *
     * @par Providing contexts
     *
     * To create a ContextView provide contexts, a derivat of ContextView must set the flag
     * ProvidesContexts. Each such view that wants to provide a context must also implement a
     * derivat of ViewContext and return a new instance of it in the overloaded
     * ContextView::createContextObject() method.
     *
     * A pattern that can be followed for creation of new contexts is shown in the
     * ContextView::createContextObject() method documentation.
     *
     * @note    There are rare cases where it is not required to create a class inheriting from
     *          ViewContext. This is the case when all data can be stored into ViewContextData
     *          objects. However, in this scenario there is no direct call back for context
     *          switching. You can gain knowledge of the context change indirectly through the
     *          ViewContextData events, though.
     *
     * @par Consuming contexts
     *
     * To consume a context, one has to set the flag ConsumesContexts on the derivat of the
     * ContextView. The view must then know which contexts to attach to. This is done by giving the
     * identifier() of the providing view to setContextProvider().
     *
     * libHeaven will then internally call to ContextView::attachedContext() and
     * ContextView::detachedContext() as it is required. These method should be overloaded to
     * implement reactions to the context change in the providing view.
     *
     * Typical implementations are:
     * - For detachedContext(): Return the view to a state displaying nothing (i.e. like when the
     *   application was just started).
     * - For attachedContext(): Bring the view to display the state that is demanded by the context.
     *
     * @par Providing and consuming contexts at the same time
     *
     * It is possible to be consuming contexts and at the same time providing context to other
     * views. This way, it is possible to create a hierarchical data dependance.
     *
     *
     * @class   ViewContextPrivate
     * @internal
     * @ingroup CentralUI
     * @brief   Internal bridge between ViewContextManager and ViewContext
     *
     * Most of the implementation of ViewContext goes here. This class befriends the Manager and the
     * Context.
     *
     */

    ViewContextPrivate::ViewContextPrivate( ViewContext* owner )
        : mOwner( owner )
        , mOwningView( NULL )
    {
        if( gDebugContexts && gDebugContextsVerbose )
        {
            qDebug( "VCP %p: Constructor", this );
        }
    }

    /**
     * @brief       Destructor
     *
     * When we're comming here, we already know that everything is over. All grace periods have
     * passed. We need to just get the damn thing gone...
     *
     */
    ViewContextPrivate::~ViewContextPrivate()
    {
        if( gDebugContexts && gDebugContextsVerbose )
        {
            qDebug( "VCP %p: Destructor", this );
        }

        foreach( ViewContextData* vcd, mDepData )
        {
            vcd->setAttachedContext( NULL );
        }
        qDeleteAll( mDepData );

        ViewContextManager::self().delContext( this );

        if( gDebugContexts && gDebugContextsVerbose )
        {
            qDebug( "VCP %p: deleted", this );
        }
    }

    void ViewContextPrivate::setOwnerShip( ContextView* view )
    {
        if( view != mOwningView )
        {
            if( gDebugContexts )
            {
                qDebug( "VCP %p: Change owner from %p to %p", this, mOwningView, view );
            }

            mOwningView = view;

            if( !mOwningView )
            {
                ViewContextManager::self().updateExpireTime( this );
            }
        }
    }

    ContextView* ViewContextPrivate::ownerShip() const
    {
        return mOwningView;
    }

    void ViewContextPrivate::setKeys( const ContextKeys& keys )
    {
        mKeys = keys;

        if( gDebugContexts )
        {
            qDebug( "VCP %p: Keys => %s", this,
                    qPrintable( mKeys.toString() ) );
        }
    }

    const ContextKeys& ViewContextPrivate::keys() const
    {
        return mKeys;
    }

    ViewContext* ViewContextPrivate::owner()
    {
        return mOwner;
    }

    QDateTime ViewContextPrivate::expiresAt() const
    {
        return mExpiresAt;
    }

    void ViewContextPrivate::setExpireAt( const QDateTime& dt )
    {
        mExpiresAt = dt;
    }

    void ViewContextPrivate::expired()
    {
        if( gDebugContexts )
        {
            qDebug( "VCP %p: About to be expelled", this );
        }

        mOwner->deleteLater();
    }

    void ViewContextPrivate::setDataFor( const ViewIdentifier& id, ViewContextData* data )
    {
        ViewContextData* data2 = mDepData.take( id );

        if( data && data == data2 )
        {
            mDepData.insert( id, data );
            return;
        }

        if( data2 )
        {
            data2->setAttachedContext( NULL );
        }

        if( data )
        {
            data->setAttachedContext( this );
            mDepData.insert( id, data );
        }
    }

    ViewContextData* ViewContextPrivate::dataFor( const ViewIdentifier& id ) const
    {
        return mDepData.value( id, NULL );
    }

    void ViewContextPrivate::detach()
    {
        foreach( ContextView* view, ViewContextManager::self().dependantViews( mKeys.viewId() ) )
        {
            view->detachContext();
        }

        mOwner->afterDetached();
    }

    void ViewContextPrivate::attach()
    {
        mOwner->beforeAttach();

        foreach( ContextView* view, ViewContextManager::self().dependantViews( mKeys.viewId() ) )
        {
            view->attachContext( mOwner );
        }
    }


    /**
     * @brief       Constructor
     *
     * Internally creates the ViewContext.
     *
     */
    ViewContext::ViewContext()
        : d( new ViewContextPrivate( this ) )
    {
    }

    /**
     * @brief       Destructor
     */
    ViewContext::~ViewContext()
    {
        delete d;
    }

    /**
     * @brief       Get the owning view
     *
     * @return      A pointer to the owning ContextView.
     *
     * The owning view is the view that currently holds track of the context. It is set to `NULL`
     * in case this ContextView is closed.
     *
     * ViewContext objects without an owningView() don't cease to exist directly. A special grace
     * period for contexts that are both ownerless and unattached is granted.
     *
     * This is mainly useful in the case, the user shortly moves into another mode (i.e. Help) and
     * then returns to a mode that recreates the owningView().
     *
     * In common applications all view specific data would have been flushed out of memory in this
     * scenario and would have to be recreated just seconds later. libHeaven's grace periods make
     * such (silly) behaviour superfluous.
     *
     */
    ContextView* ViewContext::owningView()
    {
        return d->mOwningView;
    }

    /**
     * @brief       Callback to be invoked after detaching this context
     *
     * This virtual call back method is invoked directly after the context has internally been
     * detached from a ContextView (and thus is now in any grace period).
     *
     * This method should _only_ be used to drop memory with high cost that the implementation is
     * able to fastly regenerate.
     *
     */
    void ViewContext::afterDetached()
    {
    }

    /**
     * @brief       Callback to be invoked before attaching this context
     *
     * This virtual call back method is invoked before the context is internally attached to a view.
     *
     * It should _only_ be used to (re-)create local data that is fast to calculate but has high
     * memory cost.
     *
     */
    void ViewContext::beforeAttach()
    {
    }

    /**
     * @brief       Get a set of all attached data objects
     *
     * @return      A set of all ViewContextData objects that are attached to this context.
     *
     */
    QSet< ViewContextData* > ViewContext::attachedDataObjects() const
    {
        return d->mDepData.values().toSet();
    }

    /**
     * @brief       Return the keys that are associated with this context
     *
     * @return      The associated keys
     *
     */
    ContextKeys ViewContext::keys() const
    {
        return d->mKeys;
    }

    /**
     * @class       ContextView
     * @ingroup     CentralUI
     * @brief       Founding of contextual Views
     *
     * A ContextView is a View that provides or consumes contexts to or from other views.
     *
     * Three types of context views are possible:
     * - context views that provide contexts to other views.
     * - context views that consume context from other views.
     * - context views that do both of that at the same time.
     *
     * Each of these tasks has a different set of methods to support them.
     *
     * @enum        ContextView::Flag
     * @brief       A flag for Context Views
     *
     * @typedef     ContextView::Flags
     * @brief       A combination of Flag values
     *
     */

    /**
     * @brief       Constructor
     *
     * @param[in]   identifier  The identifier for this view.
     *
     */
    ContextView::ContextView( const ViewIdentifier& identifier )
        : View( identifier )
        , mCurrentContext( NULL )
        , mCtxData( NULL )
        , mAttachedContext( NULL )
    {
        ViewContextManager::self().viewOpened( this );
    }

    /**
     * @brief       Destructor
     *
     */
    ContextView::~ContextView()
    {
        ViewContextPrivateSet ctxs = ViewContextManager::self().contextsOwnedBy( this );

        foreach( ViewContextPrivate* vcp, ctxs )
        {
            vcp->setOwnerShip( NULL );
        }
        ViewContextManager::self().viewClosed( this );
    }

    /**
     * @brief       Set the current context of this view
     *
     * @param[in]   context     The context to switch to.
     *
     *
     *
     */
    void ContextView::setCurrentContext( ViewContext* context )
    {
        mCurrentContext = context;
        ViewContextManager::self().setCurrentContext( ViewContextPrivate::of( context ), this );
    }

    /**
     * @brief       Get the current context
     *
     * @return      The current context, or `NULL` if this view doesn't currently provide a context.
     *
     */
    ViewContext* ContextView::currentContext() const
    {
        return mCurrentContext;
    }

    /**
     * @brief       Get the attached context
     *
     * @return      The attached context, or `NULL` if the context provider does not have a
     *              current context.
     *
     */
    ViewContext* ContextView::attachedContext() const
    {
        return mAttachedContext;
    }

    /**
     * @brief       Get this view's flags
     *
     * @return      This view's current flags.
     *
     */
    ContextView::Flags ContextView::flags() const
    {
        return mFlags;
    }

    /**
     * @brief       Change this view's flags
     *
     * @param[in]   flags   The flags to change
     *
     * @param[in]   set     If `true`, the flags in @a flags will be set. If `false` the flags
     *                      given in @a flags will be unset.
     *
     */
    void ContextView::setFlags( ContextView::Flags flags, bool set )
    {
        if( set )
        {
            mFlags |= flags;
        }
        else
        {
            mFlags &= ~flags;
        }
    }

    /**
     * @brief       Create a default keys object for use by this view
     *
     * @return      A new, valid ContextKeys object associated with this view's identifier but no
     *              properties set.
     *
     */
    ContextKeys ContextView::mkKeys() const
    {
        ContextKeys keys( identifier() );
        return keys;
    }

    /**
     * @brief       Search or create a context for a given set of keys
     *
     * @param[in]       keys            The set of keys to look for.
     *
     * @param[in,out]   isNewContext    Pointer to a bool, which will receive whether the returned
     *                                  context was newly created and must be initialized or not.
     *                                  May be `NULL`.
     *
     * @return          A pointer to the found or newly created ViewContext. The owner of the
     *                  returned context is set to this View.
     *
     * If it is required to setup a new context, the virtual method createContextObject() is called
     * and the context is associated with the given @a keys.
     *
     * The returned context is not attached to a View, but ownership will still be given to the
     * creating View (`this`).
     *
     */
    ViewContext* ContextView::contextFor( const ContextKeys& keys, bool* isNewContext )
    {
        ViewContextPrivate* vcp = ViewContextManager::self().getContext( keys );

        if( !vcp )
        {
            if( isNewContext )
            {
                *isNewContext = true;
            }

            ViewContext* ctx = createContextObject();
            vcp = ViewContextPrivate::of( ctx );

            vcp->setKeys( keys );
            vcp->setOwnerShip( this );
            ViewContextManager::self().addContext( vcp, gGracePeriodContextShutdownUnattached );

            return ctx;
        }

        if( isNewContext )
        {
            *isNewContext = false;
        }

        vcp->setOwnerShip( this );

        return vcp->owner();
    }

    /**
     * @brief       Allocate a view specific ViewContext object
     *
     * @return      A ViewContext object suitable to store the context data of this view.
     *
     * Derivats must implement this method and return a valid, new ViewContext object. `NULL` must
     * _never_ be returned.
     *
     * Dependant views will get access to this object.
     *
     * The new object must not yet be initialized. If a View is able to store all it's data inside
     * the context keys, a simple `return new ViewContext;` is enough, which is what the default
     * implementation does.
     *
     * However, ViewContext objects can also be used as an anchor for hooking up interface
     * implementations based on the Q_INTERFACES() system.
     *
     */
    ViewContext* ContextView::createContextObject() const
    {
        return new ViewContext;
    }

    void ContextView::attachedToContext( ViewContext* ctx, ViewContextData* data )
    {
    }

    void ContextView::detachedFromContext( ViewContext* ctx )
    {
    }

    /**
     * @internal
     * @brief       Attach to a context
     *
     * @param[in]   ctx The context to attach to
     *
     * This method is internally called to attach to a context.
     *
     * Depending on the flag DataPerContext, it will be tried to fetch this view's specific
     * ViewContextData object from the context. If the flag it is set and such a ViewContextData
     * object does not yet exist, it will be created (through a call to createContextData()) and
     * registered with the context.
     *
     * updateAttachedContext() is then called, which will take care of invoking the required call
     * backs.
     *
     */
    void ContextView::attachContext( ViewContext* ctx )
    {
        if( !ctx )
        {
            updateAttachedContext( NULL, NULL );
            return;
        }

        if( mFlags & DataPerContext )
        {
            ViewContextData* data = ViewContextPrivate::of( ctx )->dataFor( identifier() );
            if( !data )
            {
                data = createContextData();
                ViewContextPrivate::of( ctx )->setDataFor( identifier(), data );
            }

            updateAttachedContext( ctx, data );
        }
        else
        {
            updateAttachedContext( ctx, NULL );
        }
    }

    /**
     * @internal
     * @brief       Switch from no attached context to the given context
     *
     * @param[in]   ctx     The context to switch to
     *
     * @param[in]   data    Our own data, associated with the context.
     *
     * This method is internally called to do the actual update of the context and invoke the
     * attachedToContext() call back.
     *
     */
    void ContextView::updateAttachedContext( ViewContext* ctx, ViewContextData* data )
    {
        mAttachedContext = ctx;
        mCtxData = data;
        attachedToContext( ctx, data );
    }

    void ContextView::detachContext()
    {
        if( mAttachedContext )
        {
            if( mCtxData )
            {
            }
            detachedFromContext( mAttachedContext );
            mAttachedContext = NULL;
        }
    }

    /**
     * @brief       Set the context provider for this view
     *
     * @param[in]   identifier  A ViewIdentifier that names the ContextView, which will provide a
     *                          context to this view.
     *
     */
    void ContextView::setContextProvider( const ViewIdentifier& identifier )
    {
        if( mProvider != identifier )
        {
            detachContext();
            mProvider = identifier;

            ContextView* provView = ViewContextManager::self().viewFor( identifier );
            if( provView )
            {
                ViewContext* ctx = provView->currentContext();
                if( ctx )
                {
                    attachContext( ctx );
                }
            }
        }
    }

    /**
     * @brief       Get the identifier of the view that provides context to this view
     *
     * @return      The identifier of the view that provides this view's context.
     *
     */
    ViewIdentifier ContextView::contextProvider() const
    {
        return mProvider;
    }

    /**
     * @brief       Call back to create contextsensitive data for this view
     *
     * @return      A new object of a derivat of the ViewContextData class.
     *
     * You must overwrite this method, if you set the flag DataPerContext and if you do, you must
     * not return `NULL` from this method.
     *
     */
    ViewContextData* ContextView::createContextData() const
    {
        return NULL;
    }


    /**
     * @class   ViewContextData
     * @ingroup CentralUI
     * @brief   Additional per cosumer data for a ViewContext
     *
     */

    ViewContextData::ViewContextData()
        : mAttachedContext( NULL )
    {
        if( gDebugContexts && gDebugContextsVerbose )
        {
            // I would really like to output the class name here, but metaObject() still will
            // point to ViewContextData::staitcMetaObject.

            qDebug( "VCD %p: Constructor", this );
        }
    }

    ViewContextData::~ViewContextData()
    {
        if( gDebugContexts && gDebugContextsVerbose )
        {
            qDebug( "VCD %p: Destructor", this );
        }

        // Assert that we're detached when being destructed.
        Q_ASSERT( !mAttachedContext );
    }

    void ViewContextData::setAttachedContext( ViewContextPrivate*  ctx )
    {
        if( mAttachedContext )
        {
            if( gDebugContexts )
            {
                qDebug( "VCD %p: detaching from context %p", this, mAttachedContext );
            }

            detachedFromContext();
            mAttachedContext = NULL;
        }

        if( ctx )
        {
            if( gDebugContexts )
            {
                qDebug( "VCD %p: attaching to context %p", this, ctx );
            }

            mAttachedContext = ctx;
            attachedToContext( ctx->owner() );
        }
    }

    /**
     * @brief       Get the attached context, if any
     *
     * @return      A pointer to the attached ViewContext or `NULL` if this ViewContextData is not
     *              attached to a ViewContext.
     *
     */
    ViewContext* ViewContextData::attachedContext() const
    {
        return mAttachedContext ? mAttachedContext->owner() : NULL;
    }

    /**
     * @brief       Callback on detaching from a context
     *
     * This virtual call back is called directly _before_ detaching this ViewContextData from a
     * ViewContext object. In theory, this should happen only once, right before the destruction.
     *
     * While this call back is invoked, attachedContext() still returns a reference to the context.
     * However, you should assume that it is a ViewContext object and _not a derivat of that_, since
     * this method might be implicitly called from the view context's destructor.
     *
     */
    void ViewContextData::detachedFromContext()
    {
    }

    /**
     * @brief       Callback on attaching to a context
     *
     * @param[in]   context     The context that this data was just attached to.
     *
     * This virtual call back is called directly after attaching this ViewContextData to a
     * ViewContext object. In theory, this should happen only once.
     *
     */
    void ViewContextData::attachedToContext( ViewContext* context )
    {
    }

}

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

#ifndef HEAVEN_VIEW_CONTEXT_HPP
#define HEAVEN_VIEW_CONTEXT_HPP

#include <QObject>
#include <QSet>

#include "libBlueSky/Views.hpp"

namespace BlueSky {

    class ViewContextPrivate;
    class ContextView;
    class ViewContext;
    class ViewContextData;

    class HEAVEN_BLUESKY_API ContextKeys
    {
    public:
        ContextKeys();
        explicit ContextKeys( const ViewIdentifier& viewId );
        ContextKeys( const ContextKeys& other );
        ~ContextKeys();
        ContextKeys& operator=( const ContextKeys& other );
        bool isValid() const;

    public:
        bool operator==( const ContextKeys& other ) const;

    public:
        QString toString() const;
        ViewIdentifier viewId() const;

        QString operator[]( const char* pszKey ) const;
        QString operator[]( const QString& key ) const;

        QString get( const char* pszKey ) const;
        QString get( const QString& key ) const;

        void set( const char* pszKey, const QString& value );
        void set( const QString& key, const QString& value );

    private:
        class Data;
        QExplicitlySharedDataPointer< Data > d;
    };

    inline uint qHash( const ContextKeys& keys )
    {
        return qHash( keys.toString() );
    }

    class HEAVEN_BLUESKY_API ViewContextData : public QObject
    {
        Q_OBJECT
        friend class ViewContextPrivate;

    public:
        ViewContextData();
        ~ViewContextData();

    protected:
        ViewContext* attachedContext() const;

        virtual void attachedToContext( ViewContext* context );
        virtual void detachedFromContext();

    private:
        void setAttachedContext( ViewContextPrivate* ctx );

    private:
        ViewContextPrivate* mAttachedContext;
    };

    class HEAVEN_BLUESKY_API ViewContext : public QObject
    {
        Q_OBJECT
        friend class ViewContextPrivate;
        friend class ContextView;

    protected:
        ViewContext();
    public:
        ~ViewContext();

    public:
        ContextView* owningView();

    public:
        template< class I >
        QSet< I* > interfaces() const;

    protected:
        virtual void afterDetached();
        virtual void beforeAttach();

    public:
        ContextKeys keys() const;
        QSet< ViewContextData* > attachedDataObjects() const;

    private:
        ViewContextPrivate* d;
    };

    /**
     * @brief       Query all attached interfaces
     *
     * @tparam  I   The interface to query for.
     *
     * @return  A set of interface pointers.
     *
     * Query all interfaces in this context and it's attached ViewContextData objects.
     *
     */
    template< class I >
    inline QSet< I* > ViewContext::interfaces() const
    {
        QSet< I* > ifaces;

        I* i = qobject_cast< I* >( this );
        if( i )
        {
            ifaces.insert( i );
        }

        foreach( ViewContextData* data, attachedDataObjects() )
        {
            i = qobject_cast< I* >( data );
            if( i )
            {
                ifaces.insert( i );
            }
        }

        return ifaces;
    }

    class HEAVEN_BLUESKY_API ContextView : public View
    {
        Q_OBJECT
        friend class ViewContextPrivate;
        friend class ViewContextManager;
    public:
        ContextView( const ViewIdentifier& identifier );
        ~ContextView();

    public:
        enum Flag
        {
            ProvidesContexts    = 1 << 0,
            ConsumesContexts    = 1 << 1,
            DataPerContext      = 1 << 2
        };
        typedef QFlags< Flag > Flags;

    public:
        Flags flags() const;
        ViewContext* currentContext() const;
        ViewContext* attachedContext() const;

        template< class I >
        QSet< I* > interfaces() const;

    protected:
        ContextKeys mkKeys() const;
        void setFlags( Flags flags, bool set = true );

    protected:  // for context consumers
        virtual ViewContextData* createContextData() const;
        virtual void attachedToContext( ViewContext* ctx, ViewContextData* data );
        virtual void detachedFromContext( ViewContext* ctx );

        void setContextProvider( const ViewIdentifier& identifier );
        ViewIdentifier contextProvider() const;

    protected:  // for context providers
        virtual ViewContext* createContextObject() const;
        ViewContext* contextFor( const ContextKeys& keys, bool* isNewContext = NULL );
        void setCurrentContext( ViewContext* context );

    private:
        void attachContext( ViewContext* ctx );
        void detachContext();
        void updateAttachedContext( ViewContext* ctx, ViewContextData* data );

    private:
        Flags               mFlags;             //!< flags for this view
        ViewContext*        mCurrentContext;    //!< our current context
        ViewIdentifier      mProvider;          //!< the provider we depend on
        ViewContextData*    mCtxData;           //!< our data in the context we depend on
        ViewContext*        mAttachedContext;   //!< the context we're attached to
    };


    /**
     * @brief   Query all attached interfaces
     *
     * @tparam  I   The interface to query for.
     *
     * @return  A set of interface pointers.
     *
     * Query all interfaces in context and context data that are currently attached to this view.
     *
     */
    template< class I >
    inline QSet< I* > ContextView::interfaces() const
    {
        if( mAttachedContext )
        {
            return mAttachedContext->interfaces< I >();
        }

        return QSet< I*>();
    }

}

#endif




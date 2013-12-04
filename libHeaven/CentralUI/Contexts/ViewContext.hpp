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

#include "libHeaven/HeavenApi.hpp"

namespace Heaven
{

    class ViewContextPrivate;
    class ContextView;
    class ContextKeys;
    class ViewContextData;

    class HEAVEN_API ViewContext : public QObject
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

}

#endif

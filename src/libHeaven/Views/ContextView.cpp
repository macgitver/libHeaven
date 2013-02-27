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

#include "libHeaven/Views/ContextView.hpp"

namespace Heaven
{

    /**
     * @brief       ContextView::createContextObject
     *
     * @return      A ViewContext object suitable to store the context data of this view.
     *
     * Derivats must implement this method and return a valid, new ViewContext object. `NULL` must
     * _never_ be returned.
     *
     * Dependant views will get access to this object.
     *
     */

    ContextView::ContextView( const QString& identifier, ViewTypes type )
        : View( identifier, type )
    {
    }

    ViewContext* ContextView::context()
    {
        return mContext;
    }

    void ContextView::setContext( ViewContext* context )
    {
        mContext = context;
    }

}

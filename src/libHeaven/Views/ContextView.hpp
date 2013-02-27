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

#ifndef HEAVEN_CONTEXT_VIEW_H
#define HEAVEN_CONTEXT_VIEW_H

#include <QWidget>

#include "libHeaven/Views/View.h"
#include "libHeaven/Views/Contexts/ContextKeys.hpp"
#include "libHeaven/Views/Contexts/ViewContext.hpp"

namespace Heaven
{

    /**
     * @class       ContextView
     * @brief       Founding of context sensitive Views
     *
     * A ContextView is a View that provides contexts to other views.
     */
    class HEAVEN_API ContextView : public View
    {
        Q_OBJECT
    public:
        ContextView( const QString& identifier, ViewTypes type );

    public:
        ViewContext* context();
        virtual void setContext( ViewContext* context );

    protected:
        virtual ViewContext* createContextObject() = 0;

    private:
        ViewContext* mContext;
    };

}

#endif

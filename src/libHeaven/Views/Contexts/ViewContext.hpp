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

#ifndef HEAVEN_VIEW_CONTEXT_H
#define HEAVEN_VIEW_CONTEXT_H

#include <QObject>
#include <QSharedData>

#include "libHeaven/HeavenApi.hpp"

namespace Heaven
{

    class ContextView;

    class HEAVEN_API ViewContext : public QObject
    {
        Q_OBJECT
    public:
        ViewContext( ContextView* owningView );
        ~ViewContext();

    public:
        ContextView* owningView();

    protected:
        virtual void afterDetached();
        virtual void beforeAttach();

    private:
        ContextView*    mOwningView;
    };

}

#endif

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

#ifndef HEAVEN_DEPENDANT_VIEW_H
#define HEAVEN_DEPENDANT_VIEW_H

#include "libHeaven/Views/ContextView.hpp"

namespace Heaven
{

    class DependantView : public ContextView
    {
        Q_OBJECT
    public:
        DependantView( const QString& identifier );

    public:
        void setDependency( const QString& identifier );
        QString dependency() const;

    private:
        QString mDependency;
    };

}

#endif

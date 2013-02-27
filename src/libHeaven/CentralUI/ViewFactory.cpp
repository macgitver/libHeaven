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

#include "CentralUI/ViewFactory.hpp"

namespace Heaven
{

    /**
     * @class   ViewFactory
     * @ingroup CentralUI
     * @brief   factory for views
     *
     * Your application has to create one ViewFactory and install this factory via a call to
     * Heaven::Application::setViewFactory().
     *
     * libHeaven will take ownership of the factory and delete it as appropiate (actually only on
     * shutdown, unless you call setViewFactory() again with another ViewFactory).
     *
     * The ViewFactory is internally used whenever a new View is required.
     *
     * @fn          ViewFactory::createView
     * @brief       create a new View
     *
     * @param[in]   identifier      The identifier of the view to create.
     *
     * @return      A pointer to a newly created derivat of View.
     *
     * @fn          ViewFactory::ViewFactory
     * @brief       Constructor
     * @param[in]   parent      Parent in QObject hierarchy
     *
     */
    ViewFactory::ViewFactory( QObject* parent )
        : QObject( parent )
    {
    }

}

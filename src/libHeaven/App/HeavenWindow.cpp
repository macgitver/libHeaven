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

#include "App/HeavenWindow.hpp"
#include "App/HeavenWindowPrivate.hpp"

namespace Heaven
{

    /**
     * @class       HeavenWindow
     * @brief       Base for primary- and secondary windows
     *
     * Contains one TopLevelWidget, which is layouted differently in PrimaryWindow and
     * SecondaryWindow.
     *
     * @fn          HeavenWindow::isPrimary() const
     * @brief       Is this a primary window?
     *
     * @return      `true` for PrimaryWindow, `false` for a SecondaryWindow.
     *
     */

    /**
     * @internal
     * @brief       Constructor
     *
     */
    HeavenWindowPrivate::HeavenWindowPrivate()
    {
        root = NULL;
    }

    /**
     * @brief       Constructor
     * @internal
     *
     * @param[in]   priv    Pointer to private data class
     *
     * @param[in]   handle  The ViewIdentifier that will be used as handle for this HeavenWindow.
     *                      It is immutable after construction.
     *
     * This constructor is only available to derivats of HeavenWindow.
     *
     */
    HeavenWindow::HeavenWindow( HeavenWindowPrivate* priv, const ViewIdentifier& handle )
        : mPrivate( priv )
    {
        priv->handle = handle;
    }

    /**
     * @brief       Destructor
     *
     */
    HeavenWindow::~HeavenWindow()
    {
        delete mPrivate;
    }

    /**
     * @brief       Get the root container
     *
     * @return      Pointer to the root Container of this widget.
     *
     */
    ContainerWidget* HeavenWindow::rootContainer() const
    {
        return mPrivate->root;
    }

    /**
     * @brief       Get this window's handle (identifier)
     *
     * @return      ViewIdentifier for this view.
     *
     * HeavenWindows have a unique identifier. It is build from a GUID. The GUID of the
     * PrimaryWindow is always fixed to `97302003-c519-43b6-aea1-26db56e99941`.
     *
     */
    ViewIdentifier HeavenWindow::handle() const
    {
        return mPrivate->handle;
    }

}

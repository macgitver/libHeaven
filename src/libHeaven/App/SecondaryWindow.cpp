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

#include <QUuid>
#include <QVBoxLayout>

#include "App/SecondaryWindow.hpp"
#include "App/SecondaryWindowPrivate.hpp"

#include "CentralUI/ContainerWidgets/SplitterContainerWidget.hpp"

namespace Heaven
{

    SecondaryWindowPrivate::SecondaryWindowPrivate()
    {
    }

    /**
     * @brief       Constructor
     *
     * @param[in]   handle  The ViewIdentifier used as handle for this HeavenWindow.
     *
     */
    SecondaryWindow::SecondaryWindow( const ViewIdentifier& handle )
        : HeavenWindow( new SecondaryWindowPrivate, handle )
    {
        HWPD( SecondaryWindow );

        ViewIdentifier id( QUuid::createUuid().toString() );
        d->root = new SplitterContainerWidget( id, false );

        QVBoxLayout* l = new QVBoxLayout;
        l->setMargin( 0 );
        l->setSpacing( 0 );
        l->addWidget( d->root );

        setLayout( l );
    }

    /**
     * @brief       Is this a primary window?
     *
     * @return      Always `false`.
     *
     */
    bool SecondaryWindow::isPrimary() const
    {
        return false;
    }

}

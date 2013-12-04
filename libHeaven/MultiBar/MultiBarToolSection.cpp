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

#include <QToolBar>
#include <QVariant>

#include "libHeavenActions/ToolBar.hpp"

#include "MultiBar/MultiBarToolSection.hpp"
#include "MultiBar/MultiBarSectionPrivate.hpp"

namespace Heaven
{

    class MultiBarToolSectionPrivate : public MultiBarSectionPrivate
    {
    public:
        MultiBarToolSectionPrivate()
        {
            toolBar = NULL;
            toolBarWidget = NULL;
        }

    public:
        ToolBar*            toolBar;
        QToolBar*           toolBarWidget;
    };

    MultiBarToolSection::MultiBarToolSection( QWidget* parent )
        : MultiBarSection( parent, new MultiBarToolSectionPrivate )
    {
        setFlag( IsToolBar );
        setToolBar( NULL ); // create an initial dummy toolbar
    }

    void MultiBarToolSection::setToolBar( ToolBar* tb )
    {
        MultiBarToolSectionPrivate* data = static_cast< MultiBarToolSectionPrivate* >( d );

        if( data->toolBar != tb )
        {

            if( data->toolBarWidget )
            {
                Q_ASSERT( data->toolBarWidget );
                takeWidget( data->toolBarWidget );
                data->toolBarWidget->deleteLater();
            }

            data->toolBar = tb;
            if( data->toolBar )
            {
                data->toolBarWidget = tb->toolBarFor( this );
            }
            else
            {
                data->toolBarWidget = new QToolBar( this );
            }
            data->toolBarWidget->setProperty( "heavenMultiBarTool", true );
            addWidget( data->toolBarWidget );

        }
    }

    ToolBar* MultiBarToolSection::toolBar() const
    {
        const MultiBarToolSectionPrivate* data = static_cast< const MultiBarToolSectionPrivate* >( d );
        return data->toolBar;
    }

}

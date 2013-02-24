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

#include "libHeaven/Views/View.h"
#include "libHeaven/Views/ViewContainer.h"

namespace Heaven
{

    View::View( const QString& identifier, ViewTypes type )
        : ViewContainerContent( identifier, NULL )
        , mWindowState( NULL )
        , mType( type )
        , mAction( NULL )
        , mToolBar( NULL )
    {
    }

    View::~View()
    {
    }

    bool View::isContainer() const
    {
        return false;
    }

    View* View::asView()
    {
        return this;
    }

    QString View::viewName() const
    {
        return mViewName;
    }

    void View::setViewName( const QString& name )
    {
        if( name != mViewName )
        {
            mViewName = name;
            emit nameChanged( mViewName );
        }
    }

    ViewTypes View::type() const
    {
        return mType;
    }

    void View::aboutToRemove()
    {
    }

    QWidget* View::widget()
    {
        return this;
    }

    void View::setToolBar( ToolBar* tb )
    {
        if( mToolBar != tb )
        {
            mToolBar = tb;
            emit toolBarChanged( mToolBar );
        }
    }

    ToolBar* View::toolBar() const
    {
        return mToolBar;
    }

    void View::closeView()
    {
        container()->take( this );
        deleteLater();
    }

    void View::setWindowState( WindowStateView* state )
    {
        mWindowState = state;
    }

    WindowStateView* View::currentWindowState()
    {
        return mWindowState;
    }

}

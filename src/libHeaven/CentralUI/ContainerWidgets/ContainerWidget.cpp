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

#include "CentralUI/ContainerWidgets/ContainerWidget.hpp"

namespace Heaven
{

    /**
     * @class       ContainerWidget
     * @ingroup     CentralUI
     * @brief       Base class for widgets that act as containers for others
     *
     * @fn          ContainerWidget::insert( int pos, AbstractViewWidget* widget )
     * @brief       Insert a widget into this container
     *
     * @param[in]   pos     Position where to insert the widget.
     * @param[in]   widget  The widget to insert.
     *
     * @fn          ContainerWidget::takeAt(int)
     * @brief       Remove a widget from this container and return it
     *
     * @param[in]   pos     The zero based position of the widget to remove.
     *
     * @return      A pointer to the widget.
     *
     * The widget must hidden and be reparented to `NULL`. If @a pos is `-1` nothing will happen.
     *
     * @fn          ContainerWidget::indexOf(AbstractViewWidget*)
     * @brief       Get the index (position) of a widget
     *
     * @param[in]   widget  The widget whose position is to be determined.
     *
     * @return      The zero based position of the widget or `-1` if the widget is not a direct
     *              child of this container.
     *
     * @fn          ContainerWidget::count()
     * @brief       Number of widgets in this container.
     *
     * @return      The number of widgets that are direct children of this container.
     *
     * @fn          ContainerWidget::widget(int)
     * @brief       Get the widget at a given index (position).
     *
     * @param[in]   index   The zero based position to look for the widget.
     *
     * @return      A pointer to the widget at position @a index or `NULL` if no such widget exists.
     *
     * @fn          ContainerWidget::containerType()
     * @brief       Get the type of this container
     *
     * @return      The type of this container
     *
     * @fn          ContainerWidget::ContainerWidget
     * @brief       Constructor
     *
     * @param[in]   identifier  The identifier used to identifiy this container widget. Immutable
     *                          after construction.
     */
    ContainerWidget::ContainerWidget( const QString& identifier )
        : AbstractViewWidget( identifier )
    {
    }

    /**
     * @brief       Add a widget to this container
     *
     * @param[in]   widget  The widget to add into this container.
     *
     */
    void ContainerWidget::add( AbstractViewWidget* widget )
    {
        insert( count(), widget );
    }

    /**
     * @brief       Is this widget a container
     *
     * @return      Always `true` since derivats of ContainerWidget are meant to be containers.
     *
     */
    bool ContainerWidget::isContainerWidget() const
    {
        return true;
    }

    /**
     * @brief       Remove a widget from this container
     *
     * @param[in]   widget  The widget to remove from this container.
     *
     * If the widget is not part of this container, nothing happens.
     *
     */
    void ContainerWidget::take( AbstractViewWidget* widget )
    {
        takeAt( indexOf( widget ) );
    }

}

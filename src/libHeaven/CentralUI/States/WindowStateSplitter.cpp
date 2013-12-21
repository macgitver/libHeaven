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

#include <QDomElement>

#include "libHeaven/CentralUI/States/WindowStateSplitter.hpp"
#include "libHeaven/CentralUI/ContainerWidgets/SplitterContainerWidget.hpp"

namespace Heaven
{

    static inline bool parseOrient( const QString& s )
    {
        return s == QLatin1String( "Vert" );
    }

    WindowStateSplitter::WindowStateSplitter(WindowState* parent)
        : WindowState( parent )
    {
    }

    WindowStateSplitter::WindowStateSplitter(WindowState* parent, WindowStateSplitter* cloneFrom)
        : WindowState(parent, cloneFrom)
    {
        mVertical = cloneFrom->mVertical;
    }

    WindowStateSplitter::WindowStateSplitter(WindowState* parent, QDomElement& el)
        : WindowState( parent )
    {
        mVertical = parseOrient(el.attribute(QLatin1String("Orient"), QLatin1String("Vert")));

        readOrCreateIdentifier(el);
        readOptions(el);
        readChildren(el, CTContainers | CTViews);
    }

    WindowStateSplitter* WindowStateSplitter::clone(WindowState* toParent)
    {
        return new WindowStateSplitter(toParent, this);
    }

    void WindowStateSplitter::save( QDomElement& elParent ) const
    {
        QDomElement elChild = elParent.ownerDocument().createElement( QLatin1String( "Split" ) );
        elParent.appendChild( elChild );

        elChild.setAttribute(QLatin1String("Orient"),
                             QLatin1String(mVertical ? "Vert" : "Horz"));

        saveIdentifier(elChild);
        saveOptions(elChild);
        saveChildren(elChild);
    }

    void WindowStateSplitter::updateConfig()
    {
        SplitterContainerWidget* scw = qobject_cast<SplitterContainerWidget*>(widget());

        if (scw) {

            QVariantList sizes;

            foreach (int i, scw->transformedSizes()) {
                sizes.append(i);
            }

            setOption(QLatin1String("Sizes"), sizes);
        }

        WindowState::updateConfig();
    }

    void WindowStateSplitter::applyConfig()
    {
        SplitterContainerWidget* scw = qobject_cast<SplitterContainerWidget*>(widget());

        if (scw) {
            QVariant v = option(QLatin1String("Sizes"));

            if (v.type() == QVariant::List) {

                QList<int> sizes;
                foreach (QVariant vEntry, v.toList()) {
                    sizes.append(v.toInt());
                }

                scw->setTransformedSizes(sizes);
            }
        }

        WindowState::applyConfig();
    }

    WindowState::Type WindowStateSplitter::type() const
    {
        return WSSplitter;
    }

    void WindowStateSplitter::setVertical( bool value )
    {
        mVertical = value;
    }

    bool WindowStateSplitter::isVertical() const
    {
        return mVertical;
}

}

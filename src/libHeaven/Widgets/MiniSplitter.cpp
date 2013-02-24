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

#include <QPaintEvent>
#include <QPainter>
#include <QDomElement>

#include "libHeaven/Widgets/MiniSplitter.h"

namespace Heaven
{

    class MiniSplitterHandle : public QSplitterHandle
    {
    public:
        MiniSplitterHandle( Qt::Orientation ori, QSplitter* parent )
            : QSplitterHandle( ori, parent )
        {
            setMask( QRegion( contentsRect() ) );
            setAttribute( Qt::WA_MouseNoMask, true );
        }

    protected:
        void resizeEvent( QResizeEvent* ev )
        {
            if (orientation() == Qt::Horizontal)
                setContentsMargins( 2, 0, 2, 0 );
            else
                setContentsMargins( 0, 2, 0, 2 );

            setMask( QRegion( contentsRect() ) );

            QSplitterHandle::resizeEvent( ev );
        }

        void paintEvent( QPaintEvent* ev )
        {
            QPainter p( this );
            p.fillRect( ev->rect(), Qt::black );
        }
    };

    MiniSplitter::MiniSplitter( QWidget* parent )
        : QSplitter( parent )
    {
        init();
    }

    MiniSplitter::MiniSplitter( Qt::Orientation ori )
        : QSplitter( ori )
    {
        init();
    }

    void MiniSplitter::init()
    {
        setHandleWidth( 1 );
        setChildrenCollapsible( false );
    }

    QSplitterHandle* MiniSplitter::createHandle()
    {
        return new MiniSplitterHandle( orientation(), this );
    }

    void MiniSplitter::saveState( QDomElement& elContainer ) const
    {
        QDomDocument doc = elContainer.ownerDocument();
        QDomElement elWidths = doc.createElement( QLatin1String( "Widths" ) );

        QString widthStr;
        foreach( int i, sizes() )
        {
            if( !widthStr.isEmpty() )
            {
                widthStr += QChar( L':' );
            }
            widthStr += QString::number( i );
        }
        elWidths.setAttribute( QLatin1String( "v" ), widthStr );

        for( int i = 0; i < count(); ++i )
        {
            QWidget* w = widget( i );
            if( w && !w->objectName().isEmpty() )
            {
                IStoredState* ss = qobject_cast< IStoredState* >( w );
                if( ss )
                {
                    QDomElement elChildContainer = doc.createElement( QLatin1String( "Child" ) );
                    elChildContainer.setAttribute(
                                QLatin1String( "Name" ),
                                w->objectName() );
                    elContainer.appendChild( elChildContainer );

                    ss->saveState( elChildContainer );
                }
            }
        }
    }

    void MiniSplitter::loadState( const QDomElement& elContainer )
    {
    }

}

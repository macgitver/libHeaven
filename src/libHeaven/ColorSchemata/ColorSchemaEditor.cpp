/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#include <QDebug>
#include <QQueue>
#include <QStringBuilder>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QPainter>

#include "libHeaven/ColorSchemata/ColorSchemaEditor.hpp"
#include "libHeaven/ColorSchemata/ColorManager.hpp"

#include "ui_ColorSchemaEditor.h"

namespace Heaven
{

    ColorSchemaDelegate::ColorSchemaDelegate( QObject* parent )
        : QItemDelegate( parent )
    {
    }

    void ColorSchemaDelegate::paint( QPainter* painter,
                                     const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const
    {
        QItemDelegate::paint( painter, option, index );

        if( index.column() > 0 )
        {
            QModelIndex nameIdx = index.model()->index( index.row(), 0, index.parent() );
            ColorId id = ColorManager::self().colorId( nameIdx.data( Qt::UserRole ).toByteArray() );

            QRect r( 2 + option.rect.left() + option.rect.width() / 2 - 20,
                     2 + option.rect.top(),
                     40, option.rect.height() - 5 );

            QColor clr = Qt::black;

            switch( index.column() )
            {
            case 1: clr = ColorManager::self().get( id, QPalette::Active ); break;
            case 2: clr = ColorManager::self().get( id, QPalette::Inactive ); break;
            case 3: clr = ColorManager::self().get( id, QPalette::Disabled ); break;
            }

            painter->fillRect( r, clr );
            painter->setPen( Qt::black );
            painter->drawRect( r );
        }

    }

    ColorSchemaEditor::ColorSchemaEditor( QWidget* parent )
        : QWidget( parent )
    {
        ui = new Ui::ColorSchemaEditor;
        ui->setupUi( this );

        QHeaderView* head = ui->twColorList->header();
        head->setResizeMode( 0, QHeaderView::ResizeToContents );

        ui->twColorList->setItemDelegate( new ColorSchemaDelegate );

        connect( ui->twColorTree->selectionModel(),
                 SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                 this,
                 SLOT(onTreeChanged()) );

        setupColorTree();
    }

    ColorSchemaEditor::~ColorSchemaEditor()
    {
        delete ui;
    }

    void ColorSchemaEditor::setupColorTree()
    {
        typedef QPair< QByteArray, QTreeWidgetItem* > QueueItem;
        QQueue< QueueItem > todo;

        todo.enqueue( QueueItem( QByteArray(), ui->twColorTree->invisibleRootItem() ) );

        while( !todo.isEmpty() )
        {
            QueueItem it = todo.dequeue();
            QList< QByteArray > groups = ColorManager::self().sortedChildren( it.first );
            foreach( QByteArray group, groups )
            {
                QByteArray path;

                if( it.first.isEmpty() )
                    path = group;
                else
                    path = it.first % '/' % group;

                QTreeWidgetItem* item = new QTreeWidgetItem( it.second );
                item->setText( 0, ColorManager::self().translatedPathName( path ) );
                item->setData( 0, Qt::UserRole, path );

                it.second->setExpanded( true );

                foreach( QByteArray subPath, ColorManager::self().sortedChildren( path ) )
                {
                    todo.enqueue( QueueItem( path % '/' % subPath, item ) );
                }
            }
        }
    }

    void ColorSchemaEditor::onTreeChanged()
    {
        QModelIndexList mil = ui->twColorTree->selectionModel()->selectedRows();

        if( mil.count() == 0 )
            return;

        if( mil.count() != 1 )
            return;

        QByteArray path = mil[0].data( Qt::UserRole ).toByteArray();
        setupColorList( path );
    }

    void ColorSchemaEditor::setupColorList( const QByteArray& path )
    {
        ui->twColorList->clear();

        foreach( QByteArray subPath, ColorManager::self().sortedColors( path ) )
        {
            QString name = ColorManager::self().translatedColorName( path, subPath );
            QTreeWidgetItem* it = new QTreeWidgetItem( ui->twColorList );
            it->setData( 0, Qt::DisplayRole, name );
            it->setData( 0, Qt::UserRole, QVariant( path % '/' % subPath ) );
        }
    }

}

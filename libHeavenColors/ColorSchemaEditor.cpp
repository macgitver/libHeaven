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

#include <QDebug>
#include <QQueue>
#include <QStringBuilder>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QPainter>

#include "libHeavenColors/ColorSchemaEditor.hpp"
#include "libHeavenColors/ColorManager.hpp"
#include "libHeavenColors/ColorSchema.hpp"

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
            QByteArray colorName = nameIdx.data( Qt::UserRole ).toByteArray();
            ColorId id = ColorManager::self().colorId( colorName );

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

        #if QT_VERSION < 0x050000
        QHeaderView* head = ui->twColorList->header();
        head->setResizeMode( 0, QHeaderView::ResizeToContents );
        #endif

        ui->twColorList->setItemDelegate( new ColorSchemaDelegate );

        connect( ui->twColorTree->selectionModel(),
                 SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                 this,
                 SLOT(onTreeChanged()) );

        setupSchemata();
        setupColorTree();
    }

    ColorSchemaEditor::~ColorSchemaEditor()
    {
        delete ui;
    }

    void ColorSchemaEditor::setupSchemata()
    {
        ColorSchema* curSchema = ColorManager::self().activeSchema();
        QString current;
        if (curSchema) {
            curSchema->name();
        }
        QStringList schemata = ColorManager::self().schemata();

        ui->cboSchemata->clear();

        foreach( QString schema, schemata )
        {
            ui->cboSchemata->addItem( schema );
            if( schema == current )
            {
                ui->cboSchemata->setCurrentIndex( ui->cboSchemata->count() - 1 );
            }
        }
    }

    void ColorSchemaEditor::setupColorTree()
    {
        typedef QPair< QByteArray, QTreeWidgetItem* > QueueItem;
        QQueue< QueueItem > todo;

        foreach( QByteArray g, ColorManager::self().sortedChildren( QByteArray() ) )
        {
            todo.enqueue( QueueItem( g, ui->twColorTree->invisibleRootItem() ) );
        }

        while( !todo.isEmpty() )
        {
            QueueItem it = todo.dequeue();

            QTreeWidgetItem* item = new QTreeWidgetItem( it.second );
            item->setText( 0, ColorManager::self().translatedPathName( it.first ) );
            item->setData( 0, Qt::UserRole, it.first );
            item->setExpanded( true );

            QList< QByteArray > groups = ColorManager::self().sortedChildren( it.first );
            foreach( QByteArray group, groups )
            {
                todo.enqueue( QueueItem( it.first % '/' % group, item ) );
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

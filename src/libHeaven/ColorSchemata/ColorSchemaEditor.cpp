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

#include <QQueue>
#include <QStringBuilder>
#include <QTreeWidgetItem>

#include "libHeaven/ColorSchemata/ColorSchemaEditor.hpp"
#include "libHeaven/ColorSchemata/ColorManager.hpp"

#include "ui_ColorSchemaEditor.h"

namespace Heaven
{

    ColorSchemaEditor::ColorSchemaEditor( QWidget* parent )
    {
        ui = new Ui::ColorSchemaEditor;
        ui->setupUi( this );

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

}

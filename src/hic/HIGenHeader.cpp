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

#include <QUuid>
#include <QDateTime>

#include "HIGenHeader.h"

HIGenHeader::HIGenHeader( const HIDModel& model, const QString& fileName )
    : HIGeneratorBase( model, fileName )
{
}

bool HIGenHeader::run()
{
    QUuid id = QUuid::createUuid();
    QString idstr = id
                    .toString()
                    .toUpper()
                    .replace( L'{', QString() )
                    .replace( L'}', QString() )
                    .replace( L'-', QLatin1String( "_" ) );

    out() << "/**********************************************************************************\n"
             "*\n"
             "* This file is generated by HIC, the Heaven Interface Compiler\n"
             "*\n"
             "* Any modifications will be lost on the next gererator run. You've been warned.\n"
             "*\n"
             "* " << QDateTime::currentDateTime().toString( Qt::ISODate ) << "\n"
             "*\n"
             "**********************************************************************************/\n"
             "\n"
             "#ifndef HIC_" << idstr << "\n"
             "#define HIC_" << idstr << "\n\n"
             "class QObject;\n"
             "\n"
             "#include \"libHeaven/Actions/Action.hpp\"\n"
             "#include \"libHeaven/Actions/WidgetAction.hpp\"\n"
             "#include \"libHeaven/Actions/Menu.hpp\"\n"
             "#include \"libHeaven/Actions/MenuBar.hpp\"\n"
             "#include \"libHeaven/Actions/ToolBar.hpp\"\n"
             "#include \"libHeaven/Actions/MergePlace.hpp\"\n"
             "#include \"libHeaven/Actions/ActionContainer.hpp\"\n"
             "#include \"libHeaven/Actions/DynamicActionMerger.hpp\"\n"
             "\n";

    foreach( HICObject* uiObject, model() .allObjects( HACO_Ui ) )
    {
        out() << "#ifndef HIC_" << uiObject->name() << "\n"
                 "#define HIC_" << uiObject->name() << "\n\n"

                 "class " << uiObject->name() << "\n"
                 "{\n"
                 "public:\n"
                 "\tvoid setupActions( QObject* parent );\n"
                 "\n"
                 "private:\n"
                 "\tstatic QString trUtf8( const char* sourceText );\n"
                 "\n"
                 "public:\n";

        foreach( HICObject* object, uiObject->content( HACO_Action ) )
        {
            out() << "\tHeaven::Action*              act" << object->name() << ";\n";
        }

        foreach( HICObject* object, uiObject->content( HACO_WidgetAction ) )
        {
            out() << "\tHeaven::WidgetAction*        wac" << object->name() << ";\n";
        }

        foreach( HICObject* object, uiObject->content( HACO_MergePlace ) )
        {
            out() << "\tHeaven::MergePlace*          mp" << object->name() << ";\n";
        }

        foreach( HICObject* object, uiObject->content( HACO_Menu ) )
        {
            out() << "\tHeaven::Menu*                menu" << object->name() << ";\n";
        }

        foreach( HICObject* object, uiObject->content( HACO_MenuBar ) )
        {
            out() << "\tHeaven::MenuBar*             mb" << object->name() << ";\n";
        }

        foreach( HICObject* object, uiObject->content( HACO_ToolBar ) )
        {
            out() << "\tHeaven::ToolBar*             tb" << object->name() << ";\n";
        }

        foreach( HICObject* object, uiObject->content( HACO_Container ) )
        {
            out() << "\tHeaven::ActionContainer*     ac" << object->name() << ";\n";
        }

        foreach( HICObject* object, uiObject->content( HACO_DynamicActionMerger ) )
        {
            out() << "\tHeaven::DynamicActionMerger* dam" << object->name() << ";\n";
        }

        out() << "};\n\n";

        out() << "#endif\n\n";
    }

    out() << "#endif\n\n";

    return true;
}

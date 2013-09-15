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

#include <QDateTime>

#include "HIGenSource.h"

HIGenSource::HIGenSource( const HIDModel& model, const QString& fileName, const QString& baseName )
    : HIGeneratorBase( model, fileName )
    , mBaseName( baseName )
{
}

void HIGenSource::writeActionConnect( HICObject* obj, const char* whitespace, const char* prefix )
{
    QString slot;
    QByteArray receiver = "parent";

    if( obj->hasProperty( QLatin1String( "ConnectContext" ), HICP_String ) )
    {
        HICProperty p2 = obj->getProperty( QLatin1String( "ConnectContext" ) );
        receiver = p2.value().toString().toLocal8Bit();
    }
    else if( obj->hasProperty( QLatin1String( "_ConnectContext" ), HICP_String ) )
    {
        HICProperty p2 = obj->getProperty( QLatin1String( "_ConnectContext" ) );
        receiver = p2.value().toString().toLocal8Bit();
    }

    if( obj->hasProperty( QLatin1String( "ConnectTo" ), HICP_String ) )
    {
        HICProperty p = obj->getProperty( QLatin1String( "ConnectTo" ) );
        slot = p.value().toString();
    }
    else if( obj->hasProperty( QLatin1String( "_ConnectTo" ), HICP_String ) )
    {
        HICProperty p = obj->getProperty( QLatin1String( "_ConnectTo" ) );
        slot = p.value().toString();
    }

    if( !slot.isEmpty() )
    {
        const char* signal = "triggered()";
        if( slot.contains( QLatin1String( "(bool)" ) ) )
        {
            signal = "toggled(bool)";
        }

        out() << whitespace << "QObject::connect( " << prefix << obj->name() << ", SIGNAL(" << signal
              << "), " << receiver << ", SLOT(" << slot << ") );\n";
    }
}

void HIGenSource::writeSetProperties( HICObject* obj, const char* whitespace, const char* prefix )
{
    ObjectTypes type = obj->type();
    QStringList specials;

    if( type == HACO_Action )
    {
        specials << QLatin1String( "_ConnectTo" )       // Historic
                 << QLatin1String( "_ConnectContext" )  // Historic
                 << QLatin1String( "ConnectTo" )
                 << QLatin1String( "ConnectContext" );
    }
    else if( type == HACO_DynamicActionMerger )
    {
        specials << QLatin1String( "Merger" )
                 << QLatin1String( "Rebuild" );
    }

    foreach( QString pname, obj->propertyNames() )
    {
        if( specials.contains( pname ) )
        {
            continue;
        }
        HICProperty p = obj->getProperty( pname );

        out() << whitespace << prefix << obj->name() << "->set" << pname << "( ";

        switch( p.type() )
        {
        case HICP_String:
            out() << "QLatin1String( \"" << latin1Encode( p.value().toString() ) << "\" )";
            break;

        case HICP_TRString:
            out() << "trUtf8( \"" << utf8Encode( p.value().toString() ) << "\" )";
            break;

        case HICP_Boolean:
            out() << ( p.value().toBool() ? "true" : "false" );
            break;

        case HICP_Enum:
            {
                HIDEnumerator::Ptr enumerator = HICPropertyDefs::getEnumerator( obj->type(), pname );
                if( !enumerator->namespacePrefix().isEmpty() )
                {
                    out() << enumerator->namespacePrefix() << "::";
                }
                out() << p.value().toString();
            }
            break;

        default:
            out() << "WTF?";
        }

        out() << " );\n";
    }

    switch( type )
    {
    case HACO_Action:
        writeActionConnect( obj, whitespace, prefix );
        break;

    case HACO_DynamicActionMerger:
        if( obj->hasProperty( QLatin1String( "Merger" ), HICP_String ) )
        {
            HICProperty p = obj->getProperty( QLatin1String( "Merger" ) );
            QString slot = p.value().toString();

            out() << whitespace << prefix << obj->name()
                  << "->setMergerSlot( \"" << slot << "\" );\n";
        }
        if( obj->hasProperty( QLatin1String( "Rebuild" ), HICP_String ) )
        {
            HICProperty p = obj->getProperty( QLatin1String( "Rebuild" ) );
            QString signal = p.value().toString();

            out() << whitespace << "QObject::connect( parent, SIGNAL(" << signal << "), "
                  << prefix << obj->name() << ", SLOT(triggerRebuild()) );\n";
        }

    default:
        break;
    }

}

void HIGenSource::findIncludes()
{
    mIncludes.insert( QLatin1String( "QApplication" ) );

    foreach( HICObject* uiObject, model().allObjects( HACO_Ui ) )
    {
        foreach( HICObject* obj, uiObject->content() )
        {
            foreach( QString pname, obj->propertyNames() )
            {
                HICProperty p = obj->getProperty( pname );
                if( p.type() == HICP_Enum )
                {
                    HIDEnumerator::Ptr enumerator =
                            HICPropertyDefs::getEnumerator( obj->type(), pname );
                    QString file = enumerator->includeFile();
                    if( !mIncludes.contains( file ) )
                    {
                        mIncludes.insert( file );
                    }
                }
            }
        }
    }
}

bool HIGenSource::run()
{
    findIncludes();

    out() << "/**********************************************************************************\n"
             "*\n"
             "* This file is generated by HIC, the Heaven Interface Compiler\n"
             "*\n"
             "* Any modifications will be lost on the next gererator run. You've been warned.\n"
             "*\n"
             "* " << QDateTime::currentDateTime().toString( Qt::ISODate ) << "\n"
             "*\n"
             "**********************************************************************************/\n"
             "\n";

    foreach( QString include, mIncludes )
    {
        out() << "#include <" << include << ">\n";
    }

    out() << "\n"
             "#include \"" << mBaseName << "\"\n\n";

    foreach( HICObject* uiObject, model().allObjects( HACO_Ui ) )
    {
        QString ctx;

        if( uiObject->hasProperty( QLatin1String( "TrContext" ), HICP_String ) )
        {
            ctx = uiObject->getProperty( QLatin1String( "TrContext" ) ).value().toString();
        }

        if( ctx.isEmpty() )
        {
            ctx = uiObject->name();
        }

        out() << "QString "<< uiObject->name() << "::" << "trUtf8( const char* sourceText )\n"
                 "{\n"
                 "\treturn QApplication::translate( \"" << latin1Encode( ctx ) << "\", sourceText, "
                    "NULL"
                    #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
                    ", QCoreApplication::UnicodeUTF8"
                    #endif
                    " );\n"
                 "}\n"
                 "\n";

        out() << "void " << uiObject->name() << "::" << "setupActions( QObject* parent )\n"
                 "{\n"
                 "\t//Setup the actions\n\n";

        foreach( HICObject* actionObject, uiObject->content( HACO_Action ) )
        {
            out() << "\tact" << actionObject->name() << " = new Heaven::Action( parent );\n";
            writeSetProperties( actionObject, "\t", "act" );
            out() << "\n";
        }

        foreach( HICObject* actionObject, uiObject->content( HACO_ActionGroup ) )
        {
            out() << "\tagrp" << actionObject->name() << " = new Heaven::ActionGroup( parent );\n";
            //t writeSetProperties( actionObject, "\t", "act" );
            out() << "\n";
        }

        foreach( HICObject* actionObject, uiObject->content( HACO_WidgetAction ) )
        {
            out() << "\twac" << actionObject->name() << " = new Heaven::WidgetAction( parent );\n";
            writeSetProperties( actionObject, "\t", "wac" );
            out() << "\n";
        }

        out() << "\t//Setup Mergeplaces\n\n";
        foreach( HICObject* mpObject, uiObject->content( HACO_MergePlace ) )
        {
            out() << "\tmp" << mpObject->name() << " = new Heaven::MergePlace( parent );\n"
                     "\tmp" << mpObject->name() << "->setName( QByteArray( \"" <<
                     latin1Encode( mpObject->name() ) << "\" ) );\n";
            writeSetProperties( mpObject, "\t", "mp" );
            out() << "\n";
        }

        out() << "\t//Setup containers\n\n";
        foreach( HICObject* menuObject, uiObject->content( HACO_Menu ) )
        {
            out() << "\tmenu" << menuObject->name() << " = new Heaven::Menu( parent );\n";
            writeSetProperties( menuObject, "\t", "menu" );
            out() << "\n";
        }

        foreach( HICObject* menuObject, uiObject->content( HACO_MenuBar ) )
        {
            out() << "\tmb" << menuObject->name() << " = new Heaven::MenuBar( parent );\n";
            writeSetProperties( menuObject, "\t", "mb" );
            out() << "\n";
        }

        foreach( HICObject* menuObject, uiObject->content( HACO_ToolBar ) )
        {
            out() << "\ttb" << menuObject->name() << " = new Heaven::ToolBar( parent );\n";
            writeSetProperties( menuObject, "\t", "tb" );
            out() << "\n";
        }

        foreach( HICObject* menuObject, uiObject->content( HACO_Container ) )
        {
            out() << "\tac" << menuObject->name() << " = new Heaven::ActionContainer( parent );\n";
            writeSetProperties( menuObject, "\t", "ac" );
            out() << "\n";
        }

        foreach( HICObject* damObject, uiObject->content( HACO_DynamicActionMerger ) )
        {
            out() << "\tdam" << damObject->name() << " = new Heaven::DynamicActionMerger( parent );\n";
            writeSetProperties( damObject, "\t", "dam" );
            out() << "\n";
        }

        out() << "\t//Give containers some content\n\n";

        foreach( HICObject* object, uiObject->content() )
        {
            const char* prefix = "";
            switch( object->type() )
            {
            case HACO_Invalid:
            case HACO_Action:
            case HACO_MergePlace:
            case HACO_Ui:
            case HACO_Separator:
            case HACO_WidgetAction:
            case HACO_DynamicActionMerger:
                continue;

            case HACO_ActionGroup:
                prefix = "\tagrp";
                break;

            case HACO_Menu:
                prefix = "\tmenu";
                break;

            case HACO_MenuBar:
                prefix = "\tmb";
                break;

            case HACO_ToolBar:
                prefix = "\ttb";
                break;

            case HACO_Container:
                prefix = "\tac";
                break;
            }

            foreach( HICObject* child, object->content() )
            {
                switch( child->type() )
                {
                case HACO_Separator:
                    out() << prefix << object->name() << "->addSeparator();\n";
                    break;

                case HACO_Action:
                    out() << prefix << object->name() << "->add( act" << child->name() << " );\n";
                    break;

                case HACO_ActionGroup:
                    out() << prefix << object->name() << "->add( agrp" << child->name() << " );\n";
                    break;

                case HACO_WidgetAction:
                    out() << prefix << object->name() << "->add( wac" << child->name() << " );\n";
                    break;

                case HACO_Container:
                    out() << prefix << object->name() << "->add( ac" << child->name() << " );\n";
                    break;

                case HACO_MergePlace:
                    out() << prefix << object->name() << "->add( mp" << child->name() << " );\n";
                    break;

                case HACO_Menu:
                    out() << prefix << object->name() << "->add( menu" << child->name() << " );\n";
                    break;

                case HACO_DynamicActionMerger:
                    out() << prefix << object->name() << "->add( dam" << child->name() << " );\n";
                    break;

                case HACO_ToolBar:
                case HACO_MenuBar:
                case HACO_Invalid:
                case HACO_Ui:
                    break;
                }
            }

            out() << "\n";
        }

        out() << "}\n\n";
    }

    return true;
}

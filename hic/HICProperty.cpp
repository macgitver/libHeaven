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

#include <QHash>
#include <QString>

#include "HICProperty.h"
#include "HICObject.h"


HIDEnumerator::HIDEnumerator( const QString& name, const QString& namespacePrefix,
                              const QString& fileName )
    : mIncludeFile( fileName )
    , mName( name )
    , mNamespacePrefix( namespacePrefix )
{
}

void HIDEnumerator::addValue( const QString& value )
{
    Q_ASSERT( !mValues.contains( value ) );
    mValues.append( value );
}

HIDEnumerator& HIDEnumerator::operator<<( const QString& value )
{
    addValue( value );
    return *this;
}

HIDEnumerator& HIDEnumerator::operator<<( const char* value )
{
    addValue( QLatin1String( value ) );
    return *this;
}

QStringList HIDEnumerator::values() const
{
    return mValues;
}

QString HIDEnumerator::includeFile() const
{
    return mIncludeFile;
}

QString HIDEnumerator::name() const
{
    return mName;
}

QString HIDEnumerator::namespacePrefix() const
{
    return mNamespacePrefix;
}

HICProperty::HICProperty( const QVariant& v, HICPropertyType type )
    : mValue( v ), mType( type )
{
}

HICProperty::HICProperty()
    : mType( HICP_NULL )
{
}

QVariant HICProperty::value() const
{
    return mValue;
}

HICPropertyType HICProperty::type() const
{
    return mType;
}

namespace HICPropertyDefs
{

    struct PropDef
    {
    public:
        PropDef()
            : types( HICP_NULL )
        {
        }

        PropDef( const PropDef& other )
            : types( other.types )
            , enumerator( other.enumerator )
        {
        }

        PropDef( HICPropertyTypes _types )
            : types( _types )
        {
        }

        PropDef( const HIDEnumerator::Ptr& _enumerator )
            : types( HICP_Enum )
            , enumerator( _enumerator )
        {
        }

    public:
        HICPropertyTypes    types;
        HIDEnumerator::Ptr  enumerator;
    };

    typedef QHash< QString, PropDef > AllowedProps;
    typedef QHash< ObjectTypes, AllowedProps > ClassList;

    ClassList classList()
    {
        static ClassList classes;
        if( classes.count() == 0 )
        {
            HIDEnumerator::Ptr menuRole( new HIDEnumerator( QLatin1String( "MenuRole" ),
                                                            QLatin1String( "QAction" ),
                                                            QLatin1String( "QAction" ) ) );

            menuRole << "NoRole" << "TextHeuristicRole" << "ApplicationSpecificRole"
                     << "AboutQtRole" << "AboutRole" << "PreferencesRole" << "QuitRole";

            HIDEnumerator::Ptr shortcutContext( new HIDEnumerator( QLatin1String( "ShortcutContext" ),
                                                                   QLatin1String( "Qt" ),
                                                                   QLatin1String( "Qt" ) ) );
            shortcutContext << "WidgetShortcut" << "WidgetWithChildrenShortcut"
                            << "WindowShortcut" << "ApplicationShortcut";

            #define ADD(Class,Prop,Types) \
                do { \
                    classes[ (Class) ][ QLatin1String( Prop ) ] = PropDef( Types ); \
                } while(false)

            ADD( HACO_Action,       "Text",             HICP_String | HICP_TRString );
            ADD( HACO_Action,       "StatusToolTip",    HICP_String | HICP_TRString );
            ADD( HACO_Action,       "MenuRole",         menuRole );
            ADD( HACO_Action,       "Checkable",        HICP_Boolean );
            ADD( HACO_Action,       "Checked",          HICP_Boolean );
            ADD( HACO_Action,       "Visible",          HICP_Boolean );
            ADD( HACO_Action,       "Enabled",          HICP_Boolean );
            ADD( HACO_Action,       "IconRef",          HICP_String );
            ADD( HACO_Action,       "Shortcut",         HICP_String | HICP_TRString );
            ADD( HACO_Action,       "ShortcutContext",  shortcutContext );
            ADD( HACO_Action,       "_ConnectTo",       HICP_String );
            ADD( HACO_Action,       "_ConnectContext",  HICP_String );
            ADD( HACO_Action,       "ConnectTo",        HICP_String );
            ADD( HACO_Action,       "ConnectContext",   HICP_String );

            ADD( HACO_DynamicActionMerger, "Merger",    HICP_String );
            ADD( HACO_DynamicActionMerger, "Rebuild",   HICP_String );

            ADD( HACO_Menu,         "Text",             HICP_String | HICP_TRString );
            ADD( HACO_Menu,         "StatusToolTip",    HICP_String | HICP_TRString );

            ADD( HACO_Ui,           "TrContext",        HICP_String );

            #undef ADD
        }

        return classes;
    }


    bool isPropertyAllowed( HICObject* object, const QString& name, HICPropertyType type )
    {
        ClassList classes = classList();

        if( classes.contains( object->type() ) )
        {
            const AllowedProps& props = classes[ object->type() ];
            if( props.contains( name ) )
            {
                HICPropertyTypes allowed = props[ name ].types;
                if( ( allowed & type ) != HICP_NULL )
                {
                    return true;
                }
                if( allowed == HICP_Enum && type == HICP_String )
                {
                    // We can't decide if it is correct based on Enum+String match
                    return true;
                }
            }
        }

        return false;
    }

    HIDEnumerator::Ptr getEnumerator( ObjectTypes classType, const QString& name )
    {
        ClassList classes = classList();

        if( classes.contains( classType ) )
        {
            const AllowedProps& props = classes[ classType ];
            if( props.contains( name ) )
            {
                if( props[ name ].types == HICP_Enum )
                {
                    return props[ name ].enumerator;
                }
            }
        }

        return HIDEnumerator::Ptr();
    }


    bool isPropertyValueOkay( HICObject* object, const QString& pname, const QString& pvalue,
                              HICPropertyType& finalType )
    {
        ClassList classes = classList();

        if( classes.contains( object->type() ) )
        {
            const AllowedProps& props = classes[ object->type() ];
            if( props.contains( pname ) )
            {
                if( props[ pname ].types == HICP_Enum )
                {
                    finalType = HICP_Enum;
                    // If it's an enum, then it is okay as long as the enumerator contains
                    // the value
                    return props[ pname ].enumerator->values().contains( pvalue );
                }

                // If we found the property, for now, assume it's value is valid.
                // We just do the type check again
                return ( props[ pname ].types & finalType ) != HICP_NULL;
            }
        }

        return false;
    }

}

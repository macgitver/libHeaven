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

#include <QHash>
#include <QString>

#include "HICProperty.h"
#include "HICObject.h"

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

bool HICProperty::isPropertyAllowed( HICObject* object, const QString& name, HICPropertyType type )
{
    typedef QHash< QString, HICPropertyTypes > AllowedProps;
    typedef QHash< ObjectTypes, AllowedProps > ClassList;
    static ClassList classes;
    if( classes.count() == 0 )
    {
        #define ADD(Class,Prop,Types) \
            do { \
                classes[ (Class) ][ QLatin1String( Prop ) ] = (Types); \
            } while(false)

        ADD( HACO_Action,       "Text",             HICP_String | HICP_TRString );
        ADD( HACO_Action,       "StatusToolTip",    HICP_String | HICP_TRString );
        ADD( HACO_Action,       "Checkable",        HICP_Boolean );
        ADD( HACO_Action,       "Checked",          HICP_Boolean );
        ADD( HACO_Action,       "Visible",          HICP_Boolean );
        ADD( HACO_Action,       "Enabled",          HICP_Boolean );
        ADD( HACO_Action,       "IconRef",          HICP_String );
        ADD( HACO_Action,       "Shortcut",         HICP_String | HICP_TRString );
// TODO: parse enums in hic
//        ADD( HACO_Action,       "ShortcutContext",  HICP_Enum );
        ADD( HACO_Action,       "_ConnectTo",       HICP_String );
        ADD( HACO_Action,       "_ConnectContext",  HICP_String );

        ADD( HACO_Menu,         "Text",             HICP_String | HICP_TRString );
        ADD( HACO_Menu,         "StatusToolTip",    HICP_String | HICP_TRString );

        ADD( HACO_Ui,           "TrContext",        HICP_String );

        #undef ADD
    }

    if( classes.contains( object->type() ) )
    {
        const AllowedProps& props = classes[ object->type() ];
        if( props.contains( name ) )
        {
            if( ( props[ name ] & type ) != HICP_NULL )
            {
                return true;
            }
        }
    }

    return false;
}

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

#include <QDebug>
#include <QStringBuilder>

#include "libHeaven/CentralUI/Views/ViewIdentifier.hpp"

namespace Heaven
{

    /**
     * @class   ViewIdentifier
     * @ingroup CentralUI
     * @brief   A identifier for a View or HeavenWindow
     *
     * This class exists to distinguish view identifiers from other strings and to ensure that
     * these are not mixed up.
     *
     * Other than that, this is a plain wrapper around QString.
     *
     */

    /**
     * @brief       Constructor
     *
     * Creates an invalid ViewIdentifier.
     *
     */
    ViewIdentifier::ViewIdentifier()
    {
    }

    /**
     * @brief       Constructor (Creation)
     *
     * @param[in]   szName      A null terminated C-Style string. May not be `NULL`.
     *
     * Creates a valid ViewIdentifier, if the @a szName is non empty.
     *
     */
    ViewIdentifier::ViewIdentifier( const char* szName )
        : mName( QLatin1String( szName ) )
    {
    }

    /**
     * @brief       Constructor (Copy)
     *
     * @param[in]   other   Where to copy from
     *
     * Creates a ViewIdentifier that is equivalent to @a other.
     */
    ViewIdentifier::ViewIdentifier( const ViewIdentifier& other )
        : mName( other.mName )
    {
    }

    /**
     * @brief       Constructor (Creation)
     * @deprecated
     *
     * @param[in]   name    Name of this ViewIdentifier.
     *
     * Creates a ViewIdentifier with name @a name. This is deprecated, ViewIdentifiers shall
     * become QByteArrays (i.e. plain Ascii) in the longer run.
     *
     */
    ViewIdentifier::ViewIdentifier( const QString& name )
        : mName( name )
    {
    }

    /**
     * @brief       Assignment Operator
     *
     * @param[in]   szName  A null terminated C-Style string. May not be `NULL`.
     *
     * @return      A reference to this.
     *
     * Change the name of this ViewIdentifier to @a szName.
     *
     */
    ViewIdentifier& ViewIdentifier::operator=( const char* szName )
    {
        Q_ASSERT( szName );
        mName = QLatin1String( szName );
        return *this;
    }

    /**
     * @brief       Assignment Operator (Copy)
     *
     * @param[in]   other   Another ViewIdentifier
     *
     * @return      A reference to this.
     *
     * Changes this ViewIdentifier's name to @a other's name.
     *
     */
    ViewIdentifier& ViewIdentifier::operator=( const ViewIdentifier& other )
    {
        mName = other.mName;
        return *this;
    }

    /**
     * @brief       Assignment Operator
     * @deprecated
     *
     * @param[in]   name    Name for this ViewIdentifier
     *
     * @return      A reference to this.
     *
     * Change the name of this ViewIdentifier to @a name. This is deprecated, ViewIdentifiers shall
     * become QByteArrays (i.e. plain Ascii) in the longer run.
     *
     */
    ViewIdentifier& ViewIdentifier::operator=( const QString& name )
    {
        mName = name;
        return *this;
    }

    /**
     * @brief       Compare for equality operator
     * @deprecated
     *
     * @param[in]   other   QString to compare against.
     *
     * @return      `true`, if this ViewIdentifiers name is @a other, `false` otherwise.
     *
     * This is deprecated, ViewIdentifiers shall become QByteArrays (i.e. plain Ascii) in the longer
     * run.
     */
    bool ViewIdentifier::operator==( const QString& other ) const
    {
        return mName == other;
    }

    /**
     * @brief       Comapre for equality operator
     *
     * @param[in]   other   ViewIdentifier to compare against
     *
     * @return      `true` if both have the same name, `false` otherwise. Note that two invalid()
     *              ViewIdentifiers are considered equal.
     *
     */
    bool ViewIdentifier::operator==( const ViewIdentifier& other ) const
    {
        return mName == other.mName;
    }

    /**
     * @brief       Compare for inequality operator
     * @deprecated
     *
     * @param[in]   other   QString to compare against.
     *
     * @return      `false`, if this ViewIdentifiers name is @a other, `true` otherwise.
     *
     * This is deprecated, ViewIdentifiers shall become QByteArrays (i.e. plain Ascii) in the longer
     * run.
     *
     */
    bool ViewIdentifier::operator!=( const QString& other ) const
    {
        return mName != other;
    }

    /**
     * @brief       Comapre for inequality operator
     *
     * @param[in]   other   ViewIdentifier to compare against
     *
     * @return      `false` if both have the same name, `true` otherwise.
     *
     */
    bool ViewIdentifier::operator!=( const ViewIdentifier& other ) const
    {
        return mName != other.mName;
    }

    /**
     * @brief       Convert to QString
     * @deprecated
     *
     * @return      This ViewIdentifier's name.
     *
     * This is deprecated, ViewIdentifiers shall become QByteArrays (i.e. plain Ascii) in the longer
     * run.
     *
     */
    QString ViewIdentifier::toString() const
    {
        return mName;
    }

    /**
     * @brief       Test for validity.
     *
     * @return      `true` if this ViewIdentifier is valid (i.e. it's name is not of zero length),
     *              `false` otherwise.
     */
    bool ViewIdentifier::isValid() const
    {
        return !mName.isEmpty();
    }

    /**
     * @brief       Hashing function for ViewIdentifiers
     *
     * @param[in]   id  The identifier to hash
     *
     * @return      a hash suitable for QHash template
     *
     */
    uint qHash(const ViewIdentifier& id)
    {
        return qHash(id.toString());
    }

    /**
     * @brief       Compare two ViewIdentifiers (less than)
     *
     * @param[in]   lhs     Left hand side to compare
     * @param[in]   rhs     Right hand side to compare
     *
     * @return      `true` if left is smaller than right
     *
     */
    bool operator<(const ViewIdentifier& lhs, const ViewIdentifier& rhs)
    {
        return lhs.toString() < rhs.toString();
    }

}

/**
 * @brief       QDebug Shift operator for ViewIdentifier
 *
 * @param[in]   stream      The debug stream to shift into
 *
 * @param[in]   id          The ViewIdentifier to output
 *
 * @return      A reference to @a stream.
 *
 */
QDebug& operator<<(QDebug& stream, const Heaven::ViewIdentifier& id)
{
    QString s = QLatin1Literal("(VId:") % id.toString() % QChar(L')');
    return stream << qPrintable(s);
}

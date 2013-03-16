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

#ifndef MGV_HEAVEN_MINISPLITTER_H
#define MGV_HEAVEN_MINISPLITTER_H

#include <QSplitter>

#include "libHeaven/HeavenApi.hpp"

#include "libHeaven/App/IStoredState.hpp"

namespace Heaven
{

    class HEAVEN_API MiniSplitter : public QSplitter, public IStoredState
    {
        Q_OBJECT
    public:
        MiniSplitter( QWidget* parent = NULL );
        MiniSplitter( Qt::Orientation ori );

    private:
        void init();

    public:
        void saveState( QDomElement& elContainer ) const;
        void loadState( const QDomElement& elContainer );

    protected:
        QSplitterHandle* createHandle();
    };
}

#endif

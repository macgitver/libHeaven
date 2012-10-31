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

#ifndef HID_LEXER_H
#define HID_LEXER_H

#include "HIDToken.h"

class QIODevice;

class HIDLexer
{
private:
	HIDLexer( HIDTokenStream& stream );

public:
	static bool lex( QIODevice& fInput, HIDTokenStream& stream );

private:
	bool tokenize( const QString& text );
	void flush( int line );

private:
	HIDTokenStream& mOutStream;
	QString currentText;
	static QHash< QString, HIDTokenId > sTokens;
};

#endif
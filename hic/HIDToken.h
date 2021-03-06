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

#ifndef HID_TOKEN_H
#define HID_TOKEN_H

#include <QString>
#include <QList>

enum HIDTokenId
{
    Token_EOF		= 0,

    Token_Ui,
    Token_Action,
    Token_ActionGroup,
    Token_Menu,
    Token_MenuBar,
    Token_ToolBar,
    Token_Separator,
    Token_Container,
    Token_MergePlace,
    Token_Content,
    Token_WidgetAction,
    Token_DynamicActionMerger,

    Token_string,
    Token_translateString,
    Token_integer,
    Token_true,
    Token_false,

    Token_OpenCurly,
    Token_CloseCurly,
    Token_OpenSquare,
    Token_CloseSquare,
    Token_Comma,
    Token_Semicolon
};

struct HIDToken
{
    HIDTokenId  id;
    int         line;
    QString     value;
};

class HIDTokenStream : private /* public */ QList< HIDToken >
{
public:
    HIDTokenStream();

public:
    void append( const HIDToken& token );

public:
    bool endOfStream() const;
    HIDTokenId cur() const;
    const HIDToken& curToken() const;
    QString curValue() const;

public:
    void advance() const;
    bool advanceAndExpect( HIDTokenId id ) const;

private:
    mutable int mReadPos;
};

#endif

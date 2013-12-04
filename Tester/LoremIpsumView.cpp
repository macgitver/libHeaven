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

#include <QFile>
#include <QTextBrowser>

#include "LoremIpsumView.hpp"

LoremIpsumView::LoremIpsumView()
    : View("12340000-0000-0000-0000-000000000001")
{
    setViewName(QLatin1String("Lorem Ipsum"));

    QFile f(QString::fromLatin1(":/LoremIpsum.txt"));
    f.open(QFile::ReadOnly);
    QString text = QString::fromUtf8(f.readAll().constData());

    QTextBrowser* tb = new QTextBrowser;
    tb->setFrameStyle(0);
    tb->setText(text);

    setWidget(tb);

}

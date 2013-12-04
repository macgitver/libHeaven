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

#include "libBlueSky/Windows.hpp"
#include "libBlueSky/ViewDescriptor.hpp"

#include "Application.hpp"
#include "MainWindow.hpp"
#include "LoremIpsumView.hpp"

Application::Application()
    : BlueSky::Application()
{
    BlueSky::Mode* m = new BlueSky::Mode("12340000-0000-0000-0000-000000000000");
    m->setName(QString::fromLatin1("History"));
    m->setIcon(Heaven::IconRef("mode_Edit@2x", 32));
    addMode(m);

    m = new BlueSky::Mode("1");
    m->setName(QString::fromLatin1("Workspace"));
    m->setIcon(Heaven::IconRef("mode_Reference@2x", 32));
    addMode(m);

    m = new BlueSky::Mode("12340000-0000-0000-0000-000000000002");
    m->setName(QString::fromLatin1("Commit"));
    m->setIcon(Heaven::IconRef("mode_Reference@2x", 32));
    addMode(m);

    m = new BlueSky::Mode("4");
    m->setName(QString::fromLatin1("Conflicts"));
    m->setIcon(Heaven::IconRef("mode_Reference@2x", 32));
    addMode(m);

    m = new BlueSky::Mode("3");
    m->setName(QString::fromLatin1("Rebase"));
    m->setIcon(Heaven::IconRef("mode_Reference@2x", 32));
    addMode(m);

    m = new BlueSky::Mode("2");
    m->setName(QString::fromLatin1("Log book"));
    m->setIcon(Heaven::IconRef("mode_Reference@2x", 32));
    addMode(m);

    new BlueSky::ViewDescriptor("12340000-0000-0000-0000-000000000003",
                                QLatin1String("Lorem Ipsum"),
                                &LoremIpsumView::create);

    new BlueSky::ViewDescriptor("12340000-0000-0000-0000-000000000001",
                                QLatin1String("Lorem Ipsum"),
                                &LoremIpsumView::create);

    new BlueSky::ViewDescriptor("12340000-0000-0000-0000-000000000005",
                                QLatin1String("Lorem Ipsum"),
                                &LoremIpsumView::create);

    loadState(QLatin1String(":/BlueSky.xml"), true);
}

BlueSky::PrimaryWindow* Application::newPrimaryWindow() {
    return new MainWindow;
}

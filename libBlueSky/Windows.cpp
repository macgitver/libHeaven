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

#include <QGridLayout>
#include <QTextEdit>
#include <QMenuBar>

#include "libHeavenActions/MenuBar.hpp"

#include "libBlueSky/Windows.hpp"
#include "libBlueSky/FooterWidget.hpp"
#include "libBlueSky/Containers.hpp"
#include "libBlueSky/Mode.hpp"
#include "libBlueSky/Frame.hpp"

#define UUIDSTR_PRIMARY_WINDOW      "97302003-c519-43b6-aea1-26db56e99941"
#define UUIDSTR_PRIMARY_SPLITTER    "758c6df6-8153-4382-aae7-0f5603302b05"

namespace BlueSky {

    class Window::Data {
    public:
        virtual ~Data() {}

    public:
        ViewIdentifier      mIdentifier;
        ContainerWidget*    mRoot;
    };

    Window::Window(Data* d)
        : d(d) {
    }

    Window::~Window() {
        delete d;
    }

    ContainerWidget* Window::rootContainer() {
        return d->mRoot;
    }

    ViewIdentifier Window::identifier() const {
        return d->mIdentifier;
    }

    ViewIdentifier Window::idPrimaryWindow() {
        return ViewIdentifier(UUIDSTR_PRIMARY_WINDOW);
    }

    ViewIdentifier Window::idPrimarySplitter() {
        return ViewIdentifier(UUIDSTR_PRIMARY_SPLITTER);

    }

    // PrimaryWindow ---------------------------------------------------------------------------- >8

    class PrimaryWindow::Data : public Window::Data {
    public:
        Data(PrimaryWindow* owner);

    public:
        void relayout();

    public:
        PrimaryWindow* mOwner;
        Internal::ModeColumn* mModeColumn;
        QGridLayout* mLayout;
        Heaven::MenuBar* mMenuBar;
        FooterWidget* mFooter;
    };

    PrimaryWindow::Data::Data(PrimaryWindow* owner) {

        Q_ASSERT(owner);
        mOwner = owner;

        mMenuBar = NULL;

        mFooter = new FooterWidget;
        mIdentifier = PrimaryWindow::idPrimaryWindow();
        mRoot = new SplitterContainerWidget(PrimaryWindow::idPrimarySplitter(), false);

        mRoot->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

        mLayout = new QGridLayout;
        mLayout->setMargin(0);
        mLayout->setSpacing(0);

        mModeColumn = new Internal::ModeColumn;
        //mModeColumn->setMinimumWidth(90);
    }

    void PrimaryWindow::Data::relayout() {

        while (mLayout->count()) {
            QLayoutItem* it = mLayout->itemAt(0);
            mLayout->removeItem(it);
        }

        if (mMenuBar) {
        #ifdef Q_OS_MACX
            // On OSX, just ensure it exists
            mMenuBar->menuBarFor(0);
        #else
            mLayout->addWidget(mMenuBar->menuBarFor(mOwner), 0, 0, 1, 2);
        #endif
        }

        mLayout->addWidget(mModeColumn, 1, 0, 2, 1);
        mLayout->addWidget(mRoot, 1, 1);
        mLayout->addWidget(mFooter, 2, 1, 1, 1);
    }

    PrimaryWindow::Data* PrimaryWindow::d() {
        return static_cast<Data*>(Window::d);
    }

    const PrimaryWindow::Data* PrimaryWindow::d() const {
        return static_cast<const Data*>(Window::d);
    }

    PrimaryWindow::PrimaryWindow()
        : Window(new Data(this))
    {
        setProperty("heavenStyle", true);
        d()->relayout();
        setLayout(d()->mLayout);
    }

    PrimaryWindow::~PrimaryWindow() {
    }

    FooterWidget* PrimaryWindow::footer() {
        return d()->mFooter;
    }

    void PrimaryWindow::setMenuBar(Heaven::MenuBar* menu) {
        d()->mMenuBar = menu;
        d()->relayout();
    }

    Heaven::MenuBar* PrimaryWindow::menuBar() const {
        return d()->mMenuBar;
    }

}

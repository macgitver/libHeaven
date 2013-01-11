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

#include <QLabel>
#include <QStackedLayout>

#include "libHeaven/Views/Mode.h"

#include "libHeaven/Widgets/ModeSwitchWidget.h"
#include "libHeaven/Widgets/ModeSwitchCombo.h"

namespace Heaven
{

    ModeSwitchWidget::ModeSwitchWidget()
    {
        mModeDisplay = new QLabel( tr( "<b>Unknown</b>" ) );
        mModeDisplay->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
        mModeSelect = new ModeSwitchCombo();

        mStack = new QStackedLayout;
        mStack->addWidget( mModeDisplay );
        mStack->addWidget( mModeSelect );
        setLayout( mStack );

        mStack->setCurrentWidget( mModeDisplay );
    }

    void ModeSwitchWidget::modeChanged( Heaven::Mode* mode )
    {
        mModeDisplay->setText( tr( "<b>%1</b>" ).arg( mode->name() ) );
    }

}

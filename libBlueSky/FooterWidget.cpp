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

#include <QStyleOption>
#include <QStyle>
#include <QPainter>
#include <QPaintEvent>
#include <QSizeGrip>
#include <QBoxLayout>

#include "FooterWidget.hpp"

namespace BlueSky {

    class FooterWidget::Private
    {
    public:
        struct Widget
        {
            QWidget*    widget;
            int         stretch;
            int         position;
        };
        typedef QList<Widget> WidgetList;

    public:
        Private();

        void relayout();
        QWidget* takeWidget( QWidget* widget, int index );

    public:
        FooterWidget*   mOwner;
        int             mSpacing;
        int             mMarginH;
        int             mMarginV;
        WidgetList      mWidgets;
        QSizeGrip*      mGrip;
        QBoxLayout*     mLayout;
    };


    FooterWidget::Private::Private()
    {
        mOwner = NULL;
        mLayout = NULL;
        mSpacing = 6;
        mMarginH = mMarginV = 4;
    }

    void FooterWidget::Private::relayout()
    {
        delete mLayout;

        QHBoxLayout* inner = new QHBoxLayout();
        inner->addSpacing( mMarginH );
        inner->setSpacing( mSpacing );

        for( int i = 0; i < mWidgets.count(); i++ )
        {
            const Widget& w = mWidgets.at( i );
            inner->addWidget( w.widget, w.stretch );
        }
        inner->addSpacing( mMarginH );

        QVBoxLayout* spacer = new QVBoxLayout();
        spacer->addSpacing( mMarginV + 1 );
        spacer->addLayout( inner );
        spacer->addSpacing( mMarginV );

        mLayout = new QHBoxLayout( mOwner );
        mLayout->setMargin( 0 );
        mLayout->addLayout(spacer);
        mLayout->addStretch( 0 );
        mLayout->addSpacing( 1 );
        mLayout->addWidget( mGrip, 0, Qt::AlignBottom );

        mLayout->activate();
        mOwner->update();
    }

    QWidget* FooterWidget::Private::takeWidget( QWidget* widget, int index )
    {
        QWidget* w = mWidgets.takeAt( index ).widget;
        Q_ASSERT( w == widget );
        Q_UNUSED(w);
        relayout();
        return widget;
    }

    FooterWidget::FooterWidget()
        : Frame()
    {
        setBorders(Frame::Top);
        setMinimumHeight(25);

        setSizePolicy(QSizePolicy::Minimum,
                      QSizePolicy::Minimum);

        d = new Private;
        d->mOwner = this;

        d->mGrip = new QSizeGrip( this );
        d->mGrip->hide();
        d->mGrip->installEventFilter( this );

        d->relayout();

        d->mGrip->setAttribute(Qt::WA_WState_ExplicitShowHide, false);
        QMetaObject::invokeMethod(d->mGrip, "_q_showIfNotHidden", Qt::DirectConnection);
        d->mGrip->setAttribute(Qt::WA_WState_ExplicitShowHide, false);
    }

    FooterWidget::~FooterWidget()
    {
        clear();

        delete d;
    }

    void FooterWidget::setMargins( int horizontal, int vertical )
    {
        if( d->mMarginH != horizontal ||
            d->mMarginV != vertical )
        {
            d->mMarginH = horizontal;
            d->mMarginV = vertical;
            d->relayout();
        }
    }

    void FooterWidget::setSpacing( int spacing )
    {
        if( d->mSpacing != spacing )
        {
            d->mSpacing = spacing;
            d->relayout();
        }
    }

    int FooterWidget::horizontalMargin() const
    {
        return d->mMarginH;
    }

    int FooterWidget::verticalMargin() const
    {
        return d->mMarginV;
    }

    int FooterWidget::spacing() const
    {
        return d->mSpacing;
    }

    void FooterWidget::clear()
    {
        while( d->mWidgets.count() )
        {
            removeWidget( 0 );
        }
    }

    void FooterWidget::addWidget( QWidget* widget, Qt::Alignment align, int stretch )
    {
        Private::Widget w;
        w.widget = widget;
        w.position = align;
        w.stretch = stretch;
        d->mWidgets.append( w );

        d->relayout();

        if( !widget->isHidden() || !widget->testAttribute( Qt::WA_WState_ExplicitShowHide ) )
        {
            widget->show();
        }
    }

    int FooterWidget::widgetCount() const
    {
        return d->mWidgets.count();
    }

    QWidget* FooterWidget::widgetAt( int index ) const
    {
        return d->mWidgets.at( index ).widget;
    }

    void FooterWidget::removeWidget( QWidget* widget )
    {
        delete takeWidget( widget );
    }

    void FooterWidget::removeWidget( int index )
    {
        delete takeWidget( index );
    }

    QWidget* FooterWidget::takeWidget( int index )
    {
        return d->takeWidget( widgetAt( index ), index );
    }

    QWidget* FooterWidget::takeWidget( QWidget* widget )
    {
        return d->takeWidget( widget, indexOf( widget ) );
    }

    int FooterWidget::indexOf( QWidget* widget ) const
    {
        for( int i = 0; i < d->mWidgets.count(); i++ )
        {
            if( d->mWidgets.at( i ).widget == widget )
            {
                return i;
            }
        }
        return -1;
    }

    bool FooterWidget::event( QEvent* ev )
    {
        if( ev->type() == QEvent::LayoutRequest )
        {
            update();
        }

        return QWidget::event( ev );
    }

    void FooterWidget::paintEvent(QPaintEvent* ev) {
        Frame::paintEvent(ev);

        QPainter p(this);

        for (int i = 0; i < d->mWidgets.count(); i++) {
            const Private::Widget& w = d->mWidgets.at(i);
            if (w.widget->isVisible()) {

                QRect ir = w.widget->geometry().adjusted( -2, -2, 2, 1 );
                if (ev->rect().intersects(ir)) {
                    QStyleOption opt(0);
                    opt.rect = ir;
                    opt.palette = palette();
                    opt.state = QStyle::State_None;
                    style()->drawPrimitive(QStyle::PE_FrameStatusBarItem, &opt, &p, w.widget);
                }
            }
        }
    }

}

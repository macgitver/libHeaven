
#include <QDebug>
#include <QToolBar>
#include <QVariant>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include "libHeavenActions/ToolBar.hpp"

#include "libBlueSky/Views.hpp"
#include "libBlueSky/Internal/MultiBar.hpp"

namespace BlueSky {

    namespace Internal {

        MultiBarSectionPrivate::MultiBarSectionPrivate()
        {
            owner = NULL;
            layout = NULL;
            orientation = Qt::Horizontal;
            alignment = Qt::AlignLeft | Qt::AlignVCenter;
            stretch = 0;
        }

        void MultiBarSectionPrivate::relayout()
        {
            delete layout;

            if( orientation == Qt::Horizontal )
                layout = new QHBoxLayout( owner );
            else
                layout = new QVBoxLayout( owner );

            layout->setMargin( 0 );
            layout->setSpacing( 0 );

            layout->addSpacing( 2 );

            for( int i = 0; i < widgets.count(); ++i )
            {
                QWidget* widget = widgets.at( i );
                layout->addWidget( widget );
                if( !widget->isVisible() )
                {
                    //widget->show();
                }
                layout->addSpacing( 2 );
            }

            layout->addStretch( 0 );

            layout->activate();
            owner->update();
        }

        MultiBarSection::MultiBarSection( QWidget* parent )
            : QWidget( parent )
        {
            d = new MultiBarSectionPrivate;
            d->owner = this;

            d->relayout();
        }

        MultiBarSection::MultiBarSection( QWidget* parent, MultiBarSectionPrivate* p )
            : QWidget( parent )
            , d( p )
        {
            setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
            d->owner = this;

            d->relayout();
        }

        MultiBarSection::~MultiBarSection()
        {
            delete d;
        }

        void MultiBarSection::addWidget( QWidget* widget )
        {
            insertWidget( widget, d->widgets.count() );
        }

        void MultiBarSection::insertWidget( QWidget* widget, int index )
        {
            d->widgets.insert( index, widget );
            d->relayout();
        }

        void MultiBarSection::removeWidget( QWidget* widget )
        {
            delete takeWidget( widget );
        }

        void MultiBarSection::removeWidget( int index )
        {
            delete takeWidget( index );
        }

        QWidget* MultiBarSection::takeWidget( QWidget* widget )
        {
            d->widgets.removeOne( widget );

            widget->hide();
            widget->setParent( NULL );
            d->relayout();

            return widget;
        }

        QWidget* MultiBarSection::takeWidget( int index )
        {
            return takeWidget( d->widgets.at( index ) );
        }

        int MultiBarSection::widgetCount() const
        {
            return d->widgets.count();
        }

        int MultiBarSection::indexOf( QWidget* widget ) const
        {
            return d->widgets.indexOf( widget );
        }

        QWidget* MultiBarSection::widgetAt( int index ) const
        {
            return d->widgets.at( index );
        }

        MultiBarSection::Flags MultiBarSection::flags() const
        {
            return d->flags;
        }

        bool MultiBarSection::testFlag( MultiBarSection::Flag flag ) const
        {
            return d->flags.testFlag( flag );
        }

        void MultiBarSection::setFlag( Flag flag, bool set )
        {
            if( set )
                d->flags |= flag;
            else
                d->flags &= ~flag;
        }

        void MultiBarSection::setOrientation( Qt::Orientation orientation )
        {
            if( orientation != d->orientation )
            {
                d->orientation = orientation;
                d->relayout();
            }
        }

        Qt::Orientation MultiBarSection::orientation() const
        {
            return d->orientation;
        }

        void MultiBarSection::paintEvent( QPaintEvent* ev )
        {
            /*
            QPainter p( this );
            p.fillRect( rect(), Qt::magenta );
            p.drawRect( rect().adjusted( 0, 0, -1, -1 ) );
            */
        }

        Qt::Alignment MultiBarSection::alignment() const
        {
            return d->alignment;
        }

        int MultiBarSection::stretch() const
        {
            return d->stretch;
        }

        void MultiBarSection::setAlignment(Qt::Alignment alignment) {

            if (d->alignment != alignment) {
                d->alignment = alignment;

                relayoutBar();
            }
        }

        void MultiBarSection::setStretch(int stretch) {

            if (d->stretch != stretch) {
                d->stretch = stretch;

                relayoutBar();
            }
        }

        void MultiBarSection::relayoutBar() {
            MultiBar* bar = qobject_cast<MultiBar*>(parentWidget());
            if (bar) {
                bar->d()->relayout();
            }
        }

        //------------------------------------------------------------------------------------------

        class MultiBarToolSectionPrivate : public MultiBarSectionPrivate
        {
        public:
            MultiBarToolSectionPrivate()
            {
                toolBar = NULL;
                toolBarWidget = NULL;
            }

        public:
            Heaven::ToolBar*    toolBar;
            QToolBar*           toolBarWidget;
        };

        MultiBarToolSection::MultiBarToolSection( QWidget* parent )
            : MultiBarSection( parent, new MultiBarToolSectionPrivate )
        {
            setFlag( IsToolBar );
            setToolBar( NULL ); // create an initial dummy toolbar
        }

        void MultiBarToolSection::setToolBar(Heaven::ToolBar* tb)
        {
            MultiBarToolSectionPrivate* data = static_cast< MultiBarToolSectionPrivate* >( d );

            if( data->toolBar != tb )
            {

                if( data->toolBarWidget )
                {
                    Q_ASSERT( data->toolBarWidget );
                    takeWidget( data->toolBarWidget );
                    data->toolBarWidget->deleteLater();
                }

                data->toolBar = tb;
                if( data->toolBar )
                {
                    data->toolBarWidget = tb->toolBarFor( this );
                }
                else
                {
                    data->toolBarWidget = new QToolBar( this );
                }
                data->toolBarWidget->setProperty( "heavenMultiBarTool", true );
                addWidget( data->toolBarWidget );

            }
        }

        Heaven::ToolBar* MultiBarToolSection::toolBar() const
        {
            const MultiBarToolSectionPrivate* data =
                    static_cast< const MultiBarToolSectionPrivate* >(d);
            return data->toolBar;
        }

        // -----------------------------------------------------------------------------------------

        class MultiBarViewSectionPrivate : public MultiBarSectionPrivate
        {
        public:
            MultiBarViewSectionPrivate();

        public:
            int                             activeView;
            QList< MultiBarViewWidget* >    views;
        };

        MultiBarViewSectionPrivate::MultiBarViewSectionPrivate()
        {
            activeView = -1;
        }

        MultiBarViewSection::MultiBarViewSection( QWidget* parent )
            : MultiBarSection( parent, new MultiBarViewSectionPrivate )
        {
            setFlag( IsViewsBar );
        }

        void MultiBarViewSection::insertView( int index, View* view )
        {
            MultiBarViewSectionPrivate* data = static_cast< MultiBarViewSectionPrivate* >( d );

            MultiBarViewWidget* w = new MultiBarViewWidget( view );

            connect( w, SIGNAL(wantActivationChange(bool)),
                     this, SLOT(activationChange(bool)) );

            w->setOrientation( data->orientation );
            data->views.insert( index, w );

            if( data->activeView == -1 )
            {
                Q_ASSERT( index == 0 );
                data->activeView = 0;
                w->setActive( true );
            }
            else if( index < data->activeView )
            {
                data->activeView++;
            }

            insertWidget( w, index );
        }

        int MultiBarViewSection::indexOfView( View* view ) const
        {
            const MultiBarViewSectionPrivate* data = static_cast< const MultiBarViewSectionPrivate* >( d );
            for( int i = 0; i < data->views.count(); ++i )
            {
                if( data->views.at( i )->view() == view )
                {
                    return i;
                }
            }

            return -1;
        }

        void MultiBarViewSection::removeView( View* view )
        {
            MultiBarViewSectionPrivate* data = static_cast< MultiBarViewSectionPrivate* >( d );
            int idx = indexOfView( view );
            int nextActive = -2;

            if( !view || idx == -1 )
            {
                return;
            }

            if( data->activeView == idx )
            {
                if( idx != 0 || data->views.count() == 1 )
                {
                    nextActive = idx - 1;
                }
                else
                {
                    nextActive = idx;
                }
                data->views.at( idx )->setActive( false );
                data->activeView = -1;
            }

            removeWidget( idx );
            data->views.removeAt( idx );

            if( nextActive != -2 )
            {
                setActiveView( nextActive );
                emit currentChanged( data->activeView );
            }
        }

        void MultiBarViewSection::setActiveView( int index )
        {
            MultiBarViewSectionPrivate* data = static_cast< MultiBarViewSectionPrivate* >( d );

            if( data->activeView != index )
            {
                if( data->activeView != -1 )
                {
                    data->views[ data->activeView ]->setActive( false );
                }

                data->activeView = index;

                if( data->activeView != -1 )
                {
                    data->views[ data->activeView ]->setActive( true );
                }
            }
        }

        int MultiBarViewSection::activeView() const
        {
            const MultiBarViewSectionPrivate* data = static_cast< const MultiBarViewSectionPrivate* >( d );
            return data->activeView;
        }

        void MultiBarViewSection::setOrientation( Qt::Orientation orientation )
        {
            MultiBarViewSectionPrivate* data = static_cast< MultiBarViewSectionPrivate* >( d );

            foreach( MultiBarViewWidget* w, data->views )
            {
                w->setOrientation( orientation );
            }

            MultiBarSection::setOrientation( orientation );
        }


        void MultiBarViewSection::activationChange( bool desiredActivation )
        {
            MultiBarViewSectionPrivate* data = static_cast< MultiBarViewSectionPrivate* >( d );

            MultiBarViewWidget* w = qobject_cast< MultiBarViewWidget* >( sender() );
            int oldActive = data->activeView;

            if( !w )
            {
                return;
            }

            if( !desiredActivation )
            {
                return;
            }

            for( int i = 0; i < data->views.count(); i++ )
            {
                MultiBarViewWidget* dest = data->views.at( i );
                if( i == oldActive )
                {
                    dest->setActive( false );
                }
                if( dest == w )
                {
                    w->setActive( true );
                    data->activeView = i;
                }
            }

            emit currentChanged( data->activeView );
        }

        // -----------------------------------------------------------------------------------------

        MultiBarViewWidget::MultiBarViewWidget( View* view, QWidget* parent )
            : QWidget( parent )
        {
            Q_ASSERT( view );
            mView = view;
            mText = view->viewName();
            mIsPressed = mIsHovered = mIsActive = false;
            mOrientation = Qt::Horizontal;
        }

        void MultiBarViewWidget::setActive( bool active )
        {
            if( active != mIsActive )
            {
                mIsActive = active;
                mNiceSize = QSize();
                updateGeometry();
                update();
            }
        }

        bool MultiBarViewWidget::isActive() const
        {
            return mIsActive;
        }

        void MultiBarViewWidget::setOrientation(Qt::Orientation orientation) {

            qDebug() << Q_FUNC_INFO << orientation;

            if (orientation != mOrientation) {
                mOrientation = orientation;
                mNiceSize = QSize();

                updateGeometry();
                update();
            }
        }

        Qt::Orientation MultiBarViewWidget::orientation() const
        {
            return mOrientation;
        }

        View* MultiBarViewWidget::view() const
        {
            return mView;
        }

        QString MultiBarViewWidget::text() const
        {
            return mText;
        }

        void MultiBarViewWidget::paintEvent( QPaintEvent* ev )
        {
            QPainter p( this );

            if( mIsHovered || mIsActive )
            {
                QColor base( 0x40, 0x40, 0x40 );
                QLinearGradient grad1;

                if( mOrientation == Qt::Horizontal )
                    grad1 = QLinearGradient( 0., 0., 0., 7. );
                else
                    grad1 = QLinearGradient( 0., 0., 7., 0. );

                if( mIsPressed || mIsActive )
                {
                    grad1.setColorAt( 0, base.darker( 80 ) );
                    grad1.setColorAt( 0.8, base.darker( 120 ) );
                    grad1.setColorAt( 1, base.darker( 120 ) );
                }
                else
                {
                    grad1.setColorAt( 0, base.lighter( 180 ) );
                    grad1.setColorAt( 0.8, base.lighter( 220 ) );
                    grad1.setColorAt( 1, base.lighter( 220 ) );
                }
                QBrush b( grad1 );
                p.fillRect( rect(), b );
            }

            QFont f = font();
            if( mIsActive )
            {
                f.setBold( true );
            }
            p.setFont( f );

            QRect textRect = rect().adjusted( 4, 2, -4, -1 );
            p.setPen( Qt::white );
            if( mOrientation == Qt::Horizontal )
            {
                p.drawText( textRect, Qt::AlignCenter, mText );
            }
            else
            {
                p.save();
                p.translate( textRect.bottomLeft() );
                p.rotate( 270 );
                QRect r( 0, 0, textRect.height(), textRect.width() );
                p.drawText( r, Qt::AlignCenter, mText );
                p.restore();
            }
        }

        void MultiBarViewWidget::calcNiceSize() const
        {
            if( mNiceSize.isValid() )
            {
                return;
            }

            QFont f( font() );
            f.setBold( true );

            QFontMetrics fm( f );
            int w = 8 + fm.width( mText );
            int h = 4 + fm.lineSpacing();

            mNiceSize = ( mOrientation != Qt::Vertical )
                    ? QSize( w, h )
                    : QSize( h, w );

            qDebug() << "Setting nice size to" << mNiceSize;
        }

        QSize MultiBarViewWidget::minimumSizeHint() const
        {
            calcNiceSize();
            return mNiceSize;
        }

        QSize MultiBarViewWidget::sizeHint() const
        {
            calcNiceSize();
            return mNiceSize;
        }

        void MultiBarViewWidget::mousePressEvent( QMouseEvent* ev )
        {
            if( ev->button() == Qt::LeftButton && mIsHovered )
            {
                mIsPressed = true;
                update();
            }
        }

        void MultiBarViewWidget::mouseReleaseEvent( QMouseEvent* ev )
        {
            if( ev->button() == Qt::LeftButton && mIsPressed )
            {
                mIsPressed = false;
                update();
                emit wantActivationChange( !mIsActive );
            }
        }

        void MultiBarViewWidget::leaveEvent( QEvent* )
        {
            if( mIsHovered || mIsPressed )
            {
                mIsPressed = false;
                mIsHovered = false;
                update();
            }
        }

        void MultiBarViewWidget::enterEvent( QEvent* )
        {
            mIsHovered = true;
            update();
        }

        // -----------------------------------------------------------------------------------------

        MultiBar::Private::Private(MultiBar* owner)
            : Frame::Private(owner)
            , layout(NULL)
        {
        }

        void MultiBar::Private::relayout()
        {
            delete layout;

            if (mOrientation != Qt::Horizontal) { /* reversed */
                layout = new QHBoxLayout(mOwner);
            }
            else {
                layout = new QVBoxLayout(mOwner);
            }

            layout->setMargin( 0 );
            layout->setSpacing( 0 );

            for (int i = 0; i < sections.count(); i++) {
                MultiBarSection* section = sections.at(i);
                layout->addWidget(section, section->stretch(), section->alignment());
            }

            layout->activate();
            mOwner->update();
        }

        MultiBar::MultiBar(QWidget* parent)
            : Frame(parent, new Private(this))
        {
            d()->mOrientation = Qt::Horizontal;
            setOrientation(Qt::Horizontal);
            d()->relayout();
        }

        MultiBar::~MultiBar()
        {
            removeAllSections();
        }

        MultiBar::Private* MultiBar::d() {
            return static_cast<Private*>(Frame::d);
        }

        const MultiBar::Private* MultiBar::d() const {
            return static_cast<const Private*>(Frame::d);
        }

        void MultiBar::setOrientation(Qt::Orientation orientation) {

            if (orientation == d()->mOrientation) { /* reversed */

                if (orientation == Qt::Vertical) {
                    Frame::setOrientation(Qt::Horizontal);

                    setMinimumSize(25, 0);
                    setMaximumSize(25, QWIDGETSIZE_MAX);
                }
                else {
                    Frame::setOrientation(Qt::Vertical);
                    setMinimumSize(0, 25);
                    setMaximumSize(QWIDGETSIZE_MAX, 25);
                }

                foreach (MultiBarSection* sect, d()->sections) {
                    sect->setOrientation(orientation);
                }

                d()->relayout();
            }
        }

        int MultiBar::sectionCount() const
        {
            return d()->sections.count();
        }

        int MultiBar::addSection( MultiBarSection* section )
        {
            d()->sections.append( section );
            section->setParent( this );
            if (d()->mOrientation == Qt::Vertical) /* reversed */
                section->setOrientation(Qt::Horizontal);
            else
                section->setOrientation(Qt::Vertical);
            section->show();
            d()->relayout();
            return d()->sections.count() - 1;
        }

        int MultiBar::addSection()
        {
            return addSection( new MultiBarSection( this ) );
        }

        MultiBarSection* MultiBar::takeSection( int index )
        {
            MultiBarSection* sect = d()->sections.takeAt(index);

            if (sect) {
                sect->hide();
                sect->setParent( NULL );
            }

            d()->relayout();
            return sect;
        }

        void MultiBar::insertSection( int index, MultiBarSection* section )
        {
            Q_ASSERT( section );

            d()->sections.insert(index, section);

            section->setParent(this);
            if (d()->mOrientation == Qt::Vertical) /* reversed */
                section->setOrientation(Qt::Horizontal);
            else
                section->setOrientation(Qt::Vertical);
            section->show();

            d()->relayout();
        }

        void MultiBar::removeSection(int index)
        {
            delete takeSection(index);
        }

        void MultiBar::removeAllSections() {
            while (d()->sections.count()) {
                removeSection(0);
            }
        }

    }

}

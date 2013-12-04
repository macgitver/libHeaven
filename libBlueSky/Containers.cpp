
#include <QVBoxLayout>
#include <QStackedWidget>

#include "libBlueSky/Containers.hpp"
#include "libBlueSky/Internal/MiniSplitter.hpp"
#include "libBlueSky/Internal/MultiBar.hpp"

#include "hic_MultiBarContainerWidgetActions.h"

namespace BlueSky {

    /**
     * @class       ContainerWidget
     * @ingroup     CentralUI
     * @brief       Base class for widgets that act as containers for others
     *
     * @fn          ContainerWidget::insert( int pos, AbstractViewWidget* widget )
     * @brief       Insert a widget into this container
     *
     * @param[in]   pos     Position where to insert the widget.
     *
     * @param[in]   widget  The widget to insert.
     *
     * @return      The position where the View was actually inserted.
     *
     * @fn          ContainerWidget::takeAt(int)
     * @brief       Remove a widget from this container and return it
     *
     * @param[in]   pos     The zero based position of the widget to remove.
     *
     * @return      A pointer to the widget.
     *
     * The widget must hidden and be reparented to `NULL`. If @a pos is `-1` nothing will happen.
     *
     * @fn          ContainerWidget::indexOf(AbstractViewWidget*)
     * @brief       Get the index (position) of a widget
     *
     * @param[in]   widget  The widget whose position is to be determined.
     *
     * @return      The zero based position of the widget or `-1` if the widget is not a direct
     *              child of this container.
     *
     * @fn          ContainerWidget::count()
     * @brief       Number of widgets in this container.
     *
     * @return      The number of widgets that are direct children of this container.
     *
     * @fn          ContainerWidget::widget(int)
     * @brief       Get the widget at a given index (position).
     *
     * @param[in]   index   The zero based position to look for the widget.
     *
     * @return      A pointer to the widget at position @a index or `NULL` if no such widget exists.
     *
     * @fn          ContainerWidget::containerType()
     * @brief       Get the type of this container
     *
     * @return      The type of this container
     *
     * @fn          ContainerWidget::ContainerWidget
     * @brief       Constructor
     *
     * @param[in]   identifier  The identifier used to identifiy this container widget. Immutable
     *                          after construction.
     */
    ContainerWidget::ContainerWidget( const ViewIdentifier& identifier )
        : AbstractViewWidget( identifier )
    {
    }

    /**
     * @brief       Add a widget to this container
     *
     * @param[in]   widget  The widget to add into this container.
     *
     */
    void ContainerWidget::add( AbstractViewWidget* widget )
    {
        insert( count(), widget );
    }

    /**
     * @brief       Is this widget a container
     *
     * @return      Always `true` since derivats of ContainerWidget are meant to be containers.
     *
     */
    bool ContainerWidget::isContainerWidget() const
    {
        return true;
    }

    /**
     * @brief       Remove a widget from this container
     *
     * @param[in]   widget  The widget to remove from this container.
     *
     * If the widget is not part of this container, nothing happens.
     *
     */
    void ContainerWidget::take( AbstractViewWidget* widget )
    {
        takeAt( indexOf( widget ) );
    }

    // ---------------------------------------------------------------------------------------------

    SplitterContainerWidget::SplitterContainerWidget(const ViewIdentifier& identifier, bool isVertical)
        : ContainerWidget( identifier )
    {
        mSplitter = new Internal::MiniSplitter( isVertical ? Qt::Vertical : Qt::Horizontal );

        QVBoxLayout* l = new QVBoxLayout;
        l->setMargin( 0 );
        l->setSpacing( 0 );
        l->addWidget( mSplitter );

        setLayout( l );
    }

    ContainerTypes SplitterContainerWidget::containerType() const
    {
        return SplitterContainerType;
    }

    int SplitterContainerWidget::insert( int index, AbstractViewWidget* view )
    {
        view->setParentContainer( this );
        mSplitter->insertWidget( index, view );
        return mSplitter->indexOf( view );
    }

    AbstractViewWidget* SplitterContainerWidget::takeAt( int index )
    {
        AbstractViewWidget* vw = qobject_cast< AbstractViewWidget* >( mSplitter->widget( index ) );
        if( vw )
        {
            vw->hide();
            vw->setParent( NULL );      // Removes the view from the MiniSplitter
            vw->setParentContainer( NULL );
        }

        if (mSplitter->count() == 0) {
            if (ContainerWidget* cw = parentContainer()) {
                cw->take(this);
                deleteLater();
            }
        }

        return vw;
    }

    int SplitterContainerWidget::indexOf( AbstractViewWidget* widget ) const
    {
        return mSplitter->indexOf( widget );
    }

    AbstractViewWidget* SplitterContainerWidget::widget( int index )
    {
        return static_cast< AbstractViewWidget* >( mSplitter->widget( index ) );
    }

    int SplitterContainerWidget::count() const
    {
        return mSplitter->count();
    }

    void SplitterContainerWidget::setVertical( bool vert )
    {
        mSplitter->setOrientation( vert ? Qt::Vertical : Qt::Horizontal );
    }

    bool SplitterContainerWidget::isVertical() const
    {
        return mSplitter->orientation() == Qt::Vertical;
    }

    // ---------------------------------------------------------------------------------------------

    static inline bool isVertical(Positions pos)
    {
        return (pos == Left) || (pos == Right);
    }

    namespace Internal {

        class MultiBarContainerWidgetPrivate : public MultiBarContainerWidgetActions {
        public:
            MultiBarContainerWidgetPrivate( MultiBarContainerWidget* aOwner );

        public:
            void relayout();
            void updateViewsSection();

            void clearToolBar();
            void setupToolBar();

            enum ContainerAction
            {
                None                = 0,
                CloseView           = 1 << 0,
                MaximizeView        = 1 << 1,
                MinimizeView        = 1 << 2
            };
            typedef QFlags< ContainerAction > ContainerActions;

            void updateActions();

        public:
            MultiBarContainerWidget*    owner;
            Positions                   barPos;
            AbstractViewWidget*         active;
            QStackedWidget*             stack;
            MultiBar*                   toolingBar;
            MultiBar*                   viewsBar;
            MultiBarViewSection*        viewsSection;
            MultiBarToolSection*        userToolBar;
            MultiBarToolSection*        adminToolBar;
            QVBoxLayout*                layout;
            ContainerActions            possibileActions;
        };

        MultiBarContainerWidgetPrivate::MultiBarContainerWidgetPrivate( MultiBarContainerWidget* aOwner )
        {
            barPos = Top;
            owner = aOwner;
            layout = NULL;
            active = NULL;
            stack = new QStackedWidget;
            toolingBar = viewsBar = new MultiBar;
            viewsSection = new MultiBarViewSection;

            viewsBar->addSection( viewsSection );

            QObject::connect( viewsSection, SIGNAL(currentChanged(int)),
                              owner, SLOT(viewChanged(int)) );

            setupActions( owner );

            adminToolBar = new MultiBarToolSection;
            adminToolBar->setToolBar( tbDefaultV );

            userToolBar = new MultiBarToolSection;
            userToolBar->setStretch( 1 );

            toolingBar->addSection( userToolBar );
            toolingBar->addSection( adminToolBar );

            relayout();
        }

        void MultiBarContainerWidgetPrivate::relayout()
        {
            delete layout;

            layout = new QVBoxLayout( owner );
            layout->setMargin( 0 );
            layout->setSpacing( 0 );

            layout->addWidget( toolingBar );

            if( isVertical( barPos ) )
            {
                QHBoxLayout* l = new QHBoxLayout;
                l->setMargin( 0 );
                l->setSpacing( 0 );

                if( barPos == Left )
                {
                    l->addWidget( viewsBar );
                }

                l->addWidget( stack );

                if( barPos == Right )
                {
                    l->addWidget( viewsBar );
                }

                layout->addLayout( l );
            }
            else
            {
                layout->addWidget( stack );
            }

            if( barPos == Bottom )
            {
                layout->addWidget( viewsBar );
            }

            Frame::Borders brdView = Frame::None;
            Frame::Borders brdTool = Frame::None;

            switch(barPos) {
            case Top:
                brdTool = Frame::Bottom;
                break;
            case Bottom:
                brdTool = Frame::Top;
                break;
            case Left:
                brdTool = Frame::Bottom;
                brdView = Frame::Right;
                break;
            case Right:
                brdTool = Frame::Bottom;
                brdView = Frame::Left;
                break;
            }

            toolingBar->setBorders(brdTool);
            if (toolingBar != viewsBar) {
                viewsBar->setBorders(brdView);
            }

            layout->activate();
            owner->update();
        }

        void MultiBarContainerWidgetPrivate::updateViewsSection()
        {
            if( active )
            {
                possibileActions |= CloseView;
            }
            else
            {
                possibileActions &= ~CloseView;
            }
        }

        void MultiBarContainerWidgetPrivate::updateActions()
        {
            #define UPDATE_ACTION(Action,Flag,Extra) \
                do { \
                    (Action)->setVisible( possibileActions.testFlag( Flag ) && (Extra) ); \
                } while(0)

            UPDATE_ACTION( actClose,        CloseView,      true );
            UPDATE_ACTION( actMaximizeV,    MaximizeView,   true );
            UPDATE_ACTION( actMinimizeV,    MinimizeView,   true );

            #undef UPDATE_ACTION
        }

        void MultiBarContainerWidgetPrivate::clearToolBar()
        {
            userToolBar->setToolBar( NULL );
        }

        void MultiBarContainerWidgetPrivate::setupToolBar()
        {
            if( !active )
            {
                clearToolBar();
                return;
            }

            Q_ASSERT( !active->isContainerWidget() );
            View* v = qobject_cast< View* >( active );
            if( v )
            {
                userToolBar->setToolBar( v->toolBar() );
            }
        }

    }

    MultiBarContainerWidget::MultiBarContainerWidget( const ViewIdentifier& identifier )
        : ContainerWidget( identifier )
    {
        d = new Internal::MultiBarContainerWidgetPrivate(this);
    }

    MultiBarContainerWidget::~MultiBarContainerWidget()
    {
        delete d;
    }

    Positions MultiBarContainerWidget::barPosition() const
    {
        return d->barPos;
    }

    void MultiBarContainerWidget::setBarPosition(Positions position)
    {
        if( position != d->barPos )
        {
            if( d->barPos != Top && position == Top )
            {
                // We're moving from somewhere TO North
                // => We don't need the viewsBar any longer

                d->viewsBar->takeSection( 0 );

                delete d->viewsBar;
                d->viewsBar = d->toolingBar;

                d->toolingBar->insertSection( 0, d->viewsSection );
            }
            else if( d->barPos == Top )
            {
                // We're moving from North TO somewhere
                // => We now need a viewsBar at that side

                d->toolingBar->takeSection( 0 );

                d->viewsBar = new Internal::MultiBar;

                d->viewsBar->insertSection( 0, d->viewsSection );
            }

            d->barPos = position;

            if( isVertical( d->barPos ) )
            {
                d->viewsBar->setOrientation( Qt::Vertical );
                d->viewsSection->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
            }
            else
            {
                d->viewsBar->setOrientation( Qt::Horizontal );
                d->viewsSection->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
            }

            d->relayout();
            d->updateActions();
        }
    }

    int MultiBarContainerWidget::insert( int index, AbstractViewWidget* viewWidget )
    {
        View* view = qobject_cast< View* >( viewWidget );
        Q_ASSERT( view );

        d->stack->insertWidget( index, viewWidget );
        d->viewsSection->insertView( index, view );

        connect( view, SIGNAL(toolBarChanged(Heaven::ToolBar*)),
                 this, SLOT(viewToolBarChanged(Heaven::ToolBar*)) );

        if( d->stack->count() == 1 )
        {
            d->active = viewWidget;
            d->setupToolBar();
        }

        d->updateViewsSection();
        d->updateActions();

        view->setParentContainer( this );
        view->queueRelayouting();

        return index;
    }

    AbstractViewWidget* MultiBarContainerWidget::takeAt( int index )
    {
        if( index < 0 || index >= d->stack->count() )
        {
            return NULL;
        }

        View* view = qobject_cast< View* >( d->stack->widget( index ) );
        if( !view )
        {
            return NULL;
        }

        view->hide();
        view->setParent( NULL );  // removes it from d->stack
        view->setParentContainer( NULL );
        view->queueRelayouting();

        d->viewsSection->removeView( qobject_cast< View* >( view ) );

        if (d->stack->count() == 0) {
            parentContainer()->take(this);
            deleteLater();
            return view;
        }

        d->updateViewsSection();
        d->updateActions();

        return view;
    }

    int MultiBarContainerWidget::indexOf( AbstractViewWidget* widget ) const
    {
        return d->stack->indexOf( widget );
    }

    AbstractViewWidget* MultiBarContainerWidget::widget( int index )
    {
        return static_cast< AbstractViewWidget* >( d->stack->widget( index ) );
    }

    int MultiBarContainerWidget::count() const
    {
        return d->stack->count();
    }

    void MultiBarContainerWidget::viewChanged(int index)
    {
        d->active = (index == -1) ? NULL
                                  : qobject_cast<AbstractViewWidget*>(d->stack->widget(index));
        d->stack->setCurrentIndex(index);

        d->setupToolBar();
        d->updateViewsSection();
        d->updateActions();
    }

    void MultiBarContainerWidget::viewToolBarChanged(Heaven::ToolBar* toolBar)
    {
        Q_UNUSED(toolBar);

        View* view = qobject_cast<View*>(sender());
        if (!view) {
            return;
        }

        int viewIndex = d->stack->indexOf( view );
        if( viewIndex == -1 || view != d->active )
        {
            return;
        }

        d->setupToolBar();
    }

    void MultiBarContainerWidget::onCloseActiveView()
    {
        View* view = qobject_cast< View* >( d->active );
        if( view )
        {
            view->closeView();
        }
    }

    ContainerTypes MultiBarContainerWidget::containerType() const
    {
        return MultiBarContainerType;
    }

}

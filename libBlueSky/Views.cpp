
#include <QHash>
#include <QDebug>
#include <QToolBar>
#include <QVBoxLayout>
#include <QStringBuilder>

#include "libHeavenActions/ToolBar.hpp"

#include "libBlueSky/Views.hpp"
#include "libBlueSky/ViewDescriptor.hpp"
#include "libBlueSky/Containers.hpp"

#include "libBlueSky/Internal/ViewRegistrar.hpp"

namespace BlueSky {

    /**
     * @class   ViewIdentifier
     * @ingroup CentralUI
     * @brief   A identifier for a View or HeavenWindow
     *
     * This class exists to distinguish view identifiers from other strings and to ensure that
     * these are not mixed up.
     *
     * Other than that, this is a plain wrapper around QString.
     *
     */

    /**
     * @brief       Constructor
     *
     * Creates an invalid ViewIdentifier.
     *
     */
    ViewIdentifier::ViewIdentifier()
    {
    }

    /**
     * @brief       Constructor (Creation)
     *
     * @param[in]   szName      A null terminated C-Style string. May not be `NULL`.
     *
     * Creates a valid ViewIdentifier, if the @a szName is non empty.
     *
     */
    ViewIdentifier::ViewIdentifier( const char* szName )
        : mName( QLatin1String( szName ) )
    {
    }

    /**
     * @brief       Constructor (Copy)
     *
     * @param[in]   other   Where to copy from
     *
     * Creates a ViewIdentifier that is equivalent to @a other.
     */
    ViewIdentifier::ViewIdentifier( const ViewIdentifier& other )
        : mName( other.mName )
    {
    }

    /**
     * @brief       Constructor (Creation)
     * @deprecated
     *
     * @param[in]   name    Name of this ViewIdentifier.
     *
     * Creates a ViewIdentifier with name @a name. This is deprecated, ViewIdentifiers shall
     * become QByteArrays (i.e. plain Ascii) in the longer run.
     *
     */
    ViewIdentifier::ViewIdentifier( const QString& name )
        : mName( name )
    {
    }

    /**
     * @brief       Assignment Operator
     *
     * @param[in]   szName  A null terminated C-Style string. May not be `NULL`.
     *
     * @return      A reference to this.
     *
     * Change the name of this ViewIdentifier to @a szName.
     *
     */
    ViewIdentifier& ViewIdentifier::operator=( const char* szName )
    {
        Q_ASSERT( szName );
        mName = QLatin1String( szName );
        return *this;
    }

    /**
     * @brief       Assignment Operator (Copy)
     *
     * @param[in]   other   Another ViewIdentifier
     *
     * @return      A reference to this.
     *
     * Changes this ViewIdentifier's name to @a other's name.
     *
     */
    ViewIdentifier& ViewIdentifier::operator=( const ViewIdentifier& other )
    {
        mName = other.mName;
        return *this;
    }

    /**
     * @brief       Assignment Operator
     * @deprecated
     *
     * @param[in]   name    Name for this ViewIdentifier
     *
     * @return      A reference to this.
     *
     * Change the name of this ViewIdentifier to @a name. This is deprecated, ViewIdentifiers shall
     * become QByteArrays (i.e. plain Ascii) in the longer run.
     *
     */
    ViewIdentifier& ViewIdentifier::operator=( const QString& name )
    {
        mName = name;
        return *this;
    }

    /**
     * @brief       Compare for equality operator
     * @deprecated
     *
     * @param[in]   other   QString to compare against.
     *
     * @return      `true`, if this ViewIdentifiers name is @a other, `false` otherwise.
     *
     * This is deprecated, ViewIdentifiers shall become QByteArrays (i.e. plain Ascii) in the longer
     * run.
     */
    bool ViewIdentifier::operator==( const QString& other ) const
    {
        return mName == other;
    }

    /**
     * @brief       Comapre for equality operator
     *
     * @param[in]   other   ViewIdentifier to compare against
     *
     * @return      `true` if both have the same name, `false` otherwise. Note that two invalid()
     *              ViewIdentifiers are considered equal.
     *
     */
    bool ViewIdentifier::operator==( const ViewIdentifier& other ) const
    {
        return mName == other.mName;
    }

    /**
     * @brief       Compare for inequality operator
     * @deprecated
     *
     * @param[in]   other   QString to compare against.
     *
     * @return      `false`, if this ViewIdentifiers name is @a other, `true` otherwise.
     *
     * This is deprecated, ViewIdentifiers shall become QByteArrays (i.e. plain Ascii) in the longer
     * run.
     *
     */
    bool ViewIdentifier::operator!=( const QString& other ) const
    {
        return mName != other;
    }

    /**
     * @brief       Comapre for inequality operator
     *
     * @param[in]   other   ViewIdentifier to compare against
     *
     * @return      `false` if both have the same name, `true` otherwise.
     *
     */
    bool ViewIdentifier::operator!=( const ViewIdentifier& other ) const
    {
        return mName != other.mName;
    }

    /**
     * @brief       Convert to QString
     * @deprecated
     *
     * @return      This ViewIdentifier's name.
     *
     * This is deprecated, ViewIdentifiers shall become QByteArrays (i.e. plain Ascii) in the longer
     * run.
     *
     */
    QString ViewIdentifier::toString() const
    {
        return mName;
    }

    /**
     * @brief       Test for validity.
     *
     * @return      `true` if this ViewIdentifier is valid (i.e. it's name is not of zero length),
     *              `false` otherwise.
     */
    bool ViewIdentifier::isValid() const
    {
        return !mName.isEmpty();
    }

    /**
     * @brief       Hashing function for ViewIdentifiers
     *
     * @param[in]   id  The identifier to hash
     *
     * @return      a hash suitable for QHash template
     *
     */
    uint qHash(const ViewIdentifier& id)
    {
        return qHash(id.toString());
    }

    /**
     * @brief       Compare two ViewIdentifiers (less than)
     *
     * @param[in]   lhs     Left hand side to compare
     * @param[in]   rhs     Right hand side to compare
     *
     * @return      `true` if left is smaller than right
     *
     */
    bool operator<(const ViewIdentifier& lhs, const ViewIdentifier& rhs)
    {
        return lhs.toString() < rhs.toString();
    }

    // ---------------------------------------------------------------------------------------------

    AbstractViewWidget::AbstractViewWidget( const ViewIdentifier& identifier )
        : QWidget( NULL )
        , mIdentifier( identifier )
        , mParentContainer( NULL )
    {
    }

    bool AbstractViewWidget::isContainerWidget() const
    {
        return false;
    }

    ContainerWidget* AbstractViewWidget::parentContainer()
    {
        return mParentContainer;
    }

    ViewIdentifier AbstractViewWidget::identifier() const
    {
        return mIdentifier;
    }

    void AbstractViewWidget::setParentContainer( ContainerWidget* container )
    {
        mParentContainer = container;
    }

    /**
     * @brief       Convert this to a ContainerWidget
     *
     * @return      `this` as ContainerWidget, if isContainerWidget() returns `true`; `NULL`
     *              otherwise.
     */
    ContainerWidget* AbstractViewWidget::asContainerWidget()
    {
        return isContainerWidget() ? static_cast< ContainerWidget* >( this ) : NULL;
    }

    // ---------------------------------------------------------------------------------------------

    /**
     * @class   View
     * @ingroup CentralUI
     * @brief   Base class for content viewing widgets
     *
     * A view has a widget (set by setWidget()) which is displayed without border or margin inside
     * the view.
     * A view might have a ToolBar (set by setToolBar()), which is either displayed above the widget
     * or in the parentContainer().
     *
     * @fn          View::nameChanged(const QString&)
     *
     * @param[in]   viewName    the new viewName for this view
     *
     * This signals is emitted when the translated display name of this view has changed.
     *
     * @fn          View::toolBarChanged(Heaven::ToolBar*)
     *
     * @param[in]   toolBar     the new tool bar of this view
     *
     * This signal is emitted when the view's tool bar has been changed (i.e. through a call to
     * setToolBar().
     *
     */

    /**
     * @brief       Constructor
     *
     * @param[in]   identifier  The identifier for this view. This is immutable after construction.
     *
     */
    View::View( const ViewIdentifier& identifier )
        : AbstractViewWidget( identifier )
        , mToolBar( NULL )
        , mRelayoutingIsQueued( false )
        , mRelayoutingForced( false )
        , mToolBarInOwnLayout( false )
    {
    }

    /**
     * @brief       Destructor
     */
    View::~View()
    {
    }

    /**
     * @brief       Get the view's display name
     *
     * @return      A translated string that can be presented to the user to refer to the view.

     *
     */
    QString View::viewName() const
    {
        return mViewName;
    }

    /**
     * @brief       Change the view's display name
     *
     * @param[in]   name    The new display name that should be used to present the user a reference
     *                      to this View. It appears i.e. on tab labels.
     *
     * If the new name differs from the currently set one, the nameChanged() signal will be emitted.
     *
     */
    void View::setViewName( const QString& name )
    {
        if( name != mViewName )
        {
            mViewName = name;
            emit nameChanged( mViewName );
        }
    }

    void View::aboutToRemove()
    {
    }

    /**
     * @brief       Set this view's tool bar
     *
     * @param[in]   tb  A tool bar associated with this view or `NULL` if the view should not
     *                  display a tool bar at all.
     *
     * If the new tool bar is different from the currently set one, the toolBarChanged() singal will
     * be emitted.
     *
     * The layout of the View will be updated on the next return to Qt's event loop.
     *
     */
    void View::setToolBar(Heaven::ToolBar* tb)
    {
        if( mToolBar != tb )
        {
            if( !mToolBar.isNull() && mToolBarInOwnLayout )
            {
                mToolBar->toolBarFor( this )->deleteLater();
            }

            mToolBar = tb;
            emit toolBarChanged( mToolBar );

            // If we already have a toolbar in our own layout, we _must_ relayout regardless of
            // the fact that the toolbar is still in our own layout.
            mRelayoutingForced |= mToolBarInOwnLayout;

            queueRelayouting();
        }
    }

    /**
     * @brief       Get this view's tool bar
     *
     * @return      A pointer to a Heaven::ToolBar that is associated with this view.
     *
     */
    Heaven::ToolBar* View::toolBar() const
    {
        return mToolBar;
    }

    /**
     * @brief       Close this view
     *
     */
    void View::closeView() {
        Internal::ViewRegistrar::self().viewClosed(this);
        parentContainer()->take(this);
        deleteLater();
    }

    /**
     * @brief       Change the view's widget.
     *
     * @param[in]   widget      The new widget to display inside this View or `NULL` to designate
     *                          the the view should not display a widget.
     *
     * The View takes ownership of the widget and will reparent the widget to itself.
     *
     * Any previously set QWidget will be deleted.
     *
     * The layout of the view will be updated on the next run of Qt's event loop.
     *
     */
    void View::setWidget( QWidget* widget )
    {
        if( widget != mWidget )
        {
            if( mWidget )
            {
                mWidget->deleteLater();
            }

            mWidget = widget;

            if( mWidget )
            {
                // Change the parent _now_. Don't wait until the event loop runs again.
                mWidget->hide();
                mWidget->setParent( this );
            }

            mRelayoutingForced = true;
            queueRelayouting();
        }
    }

    /**
     * @brief       Get the view's content widget
     *
     * @return      A QWidget pointer to the widget set by setWidget().
     *
     */
    QWidget* View::widget()
    {
        return mWidget;
    }

    /**
     * @internal
     * @brief       enqueue a relayouting onto the event loop
     *
     * If a relayouting is already enqueued, nothing will be done.
     *
     */
    void View::queueRelayouting()
    {
        if( !mRelayoutingIsQueued )
        {
            QMetaObject::invokeMethod( this, "performQueuedRelayouting", Qt::QueuedConnection );
            mRelayoutingIsQueued = true;
        }
    }

    /**
     * @internal
     * @brief       perform a queued relayouting
     *
     * Relayouts the view, if it is deemed to be required. It is required if mRelayoutingForced is
     * set to `true` or the type of the parentContainer() changed. In the latter case the position
     * of the toolbar has to be changed.
     *
     */
    void View::performQueuedRelayouting()
    {
        // We need an own toolBar, when there is a tool bar set and we have a parent container
        // which is NOT a MultiBarContainerWidget.
        bool needOwnToolBar = !mToolBar.isNull();
        needOwnToolBar &= parentContainer() &&
                          parentContainer()->containerType() != MultiBarContainerType;

        if( needOwnToolBar != mToolBarInOwnLayout )
        {
            // When toolbar or widget are changed, the internal code always sets relayouting
            // forced to true. But when we're comming from a MultiBarContainerWidget, we might
            // get a "add toolbar" followed by a "remove toolbar" (Moving from one MBC to another)
            // Which we catch by this weird logic... Which btw is the only real reason we need this
            // code to be queued.
            mRelayoutingForced = true;
        }

        if( mRelayoutingForced )
        {
            QVBoxLayout* l = new QVBoxLayout;
            l->setMargin( 0 );
            l->setSpacing( 0 );

            if( needOwnToolBar )
            {
                l->addWidget( mToolBar->toolBarFor( this ) );
            }
            if( !mWidget.isNull() )
            {
                l->addWidget( mWidget );

                if( !mWidget->isVisible() )
                {
                    mWidget->show();
                }
            }
            setLayout( l );

            mToolBarInOwnLayout = needOwnToolBar;
        }

        mRelayoutingForced = false;
        mRelayoutingIsQueued = false;
    }


}

/**
 * @brief       QDebug Shift operator for ViewIdentifier
 *
 * @param[in]   stream      The debug stream to shift into
 *
 * @param[in]   id          The ViewIdentifier to output
 *
 * @return      A reference to @a stream.
 *
 */
QDebug& operator<<(QDebug& stream, const BlueSky::ViewIdentifier& id)
{
    QString s = QLatin1Literal("(VId:") % id.toString() % QChar(L')');
    return stream << qPrintable(s);
}

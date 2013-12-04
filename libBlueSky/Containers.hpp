
#include "libBlueSky/Views.hpp"

namespace BlueSky {

    namespace Internal {

        class MiniSplitter;
        class MultiBarContainerWidgetPrivate;
        class MultiBar;

    }

    /**
     * @brief   Types of containers
     * @ingroup CentralUI
     */
    enum ContainerTypes {
        MultiBarContainerType,
        SplitterContainerType
    };

    enum Positions
    {
        Left,
        Right,
        Top,
        Bottom
    };

    class HEAVEN_BLUESKY_API ContainerWidget : public AbstractViewWidget {
        Q_OBJECT
    public:
        ContainerWidget( const ViewIdentifier& identifier );

    public:
        void add( AbstractViewWidget* widget );
        virtual int insert( int pos, AbstractViewWidget* widget ) = 0;
        virtual AbstractViewWidget* takeAt( int pos ) = 0;
        virtual int indexOf( AbstractViewWidget* widget ) const = 0;
        virtual int count() const = 0;
        virtual AbstractViewWidget* widget( int index ) = 0;
        void take( AbstractViewWidget* widget );

        bool isContainerWidget() const;

        virtual ContainerTypes containerType() const = 0;
    };

    class SplitterContainerWidget : public ContainerWidget
    {
        Q_OBJECT
    public:
        SplitterContainerWidget( const ViewIdentifier& identifier, bool isVertical = false );

    public:
        int insert( int index, AbstractViewWidget* view );
        AbstractViewWidget* takeAt( int index );
        int indexOf( AbstractViewWidget* widget ) const;
        AbstractViewWidget* widget( int index );
        int count() const;

    public:
        ContainerTypes containerType() const;

    public:
        void setVertical( bool vert );
        bool isVertical() const;

    private:
        Internal::MiniSplitter* mSplitter;
    };

    class MultiBarContainerWidget : public ContainerWidget
    {
        Q_OBJECT
    public:
        MultiBarContainerWidget(const ViewIdentifier& identifier);
        ~MultiBarContainerWidget();

    public:
        Positions barPosition() const;
        void setBarPosition(Positions position);

    public:
        int insert( int index, AbstractViewWidget* view );
        AbstractViewWidget* takeAt( int index );
        int indexOf( AbstractViewWidget* widget ) const;
        AbstractViewWidget* widget( int index );
        int count() const;

        ContainerTypes containerType() const;

    private slots:
        void viewChanged( int index );
        void viewToolBarChanged(Heaven::ToolBar* toolBar);
        void onCloseActiveView();

    private:
        Internal::MultiBarContainerWidgetPrivate* d;
    };

}


#include <QSharedData>
#include <QVector>
#include <QSet>

class QDomDocument;
class QDomElement;

#include "libBlueSky/Views.hpp"
#include "libBlueSky/Containers.hpp"

namespace BlueSky {

    class Window;

    namespace Internal {

        class XmlState : public QSharedData
        {
        public:
            enum Type
            {
                WSSplitter = 0,
                WSTab,
                WSView,
                WSWindow,

                WSRoot
            };

            enum ChildType
            {
                CTContainers = 1 << 0,
                CTViews = 1 << 1,
                CTWindows = 1 << 2
            };
            Q_DECLARE_FLAGS( ChildTypes, ChildType )

        public:
            typedef QExplicitlySharedDataPointer<XmlState> Ptr;

        protected:
            XmlState();
        public:
            virtual ~XmlState();

        public:
            virtual Type type() const = 0;

            int childrenCount() const;
            XmlState::Ptr childAt( int index ) const;
            QVector< XmlState::Ptr > children() const;

            virtual void updateConfig();

        public:
            ViewIdentifier identifier() const;
            void setIdentifier( const ViewIdentifier& id );

            void setCurrentContent( AbstractViewWidget* avw );
            AbstractViewWidget* currentContent();

        public:
            static XmlState::Ptr read(const QDomElement& el);
            QDomElement save(QDomDocument& doc) const;

        protected:
            virtual bool load(const QDomElement& elParent) = 0;
            virtual bool save(QDomElement& elParent) const = 0;
            void append(const Ptr& child);

            bool readChildren(const QDomElement& elParent, ChildTypes allowed);
            bool saveChildren( QDomElement& elParent ) const;

            bool readOrCreateIdentifier(const QDomElement& el);
            bool saveIdentifier( QDomElement& el ) const;

        private:
            XmlState*           mParent;
            AbstractViewWidget* mCurrentContent;
            QVector<Ptr>        mChildren;
            ViewIdentifier      mId;
        };


        class XmlStateSplitter : public XmlState
        {
        public:
            typedef QExplicitlySharedDataPointer<XmlStateSplitter> Ptr;

        public:
            XmlStateSplitter();

        public:
            Type type() const;

            void setVertical( bool value );
            bool isVertical() const;

            void updateConfig();

        protected:
            bool load(const QDomElement& elParent);
            bool save(QDomElement& elParent) const;

        private:
            bool            mVertical;
        };

        class XmlStateTab : public XmlState
        {
        public:
            typedef QExplicitlySharedDataPointer<XmlStateTab> Ptr;

        public:
            XmlStateTab();

        public:
            Type type() const;

            void setTabPosition( Positions pos );
            Positions tabPosition() const;

            void updateConfig();

        protected:
            bool load(const QDomElement& elParent);
            bool save(QDomElement& elParent) const;

        private:
            Positions mPositions;
        };

        class XmlStateView : public XmlState
        {
        public:
            typedef QExplicitlySharedDataPointer<XmlStateView> Ptr;

        public:
            XmlStateView();

        public:
            Type type() const;

            void updateConfig();

        protected:
            bool load(const QDomElement& elParent);
            bool save(QDomElement& elParent) const;
        };

        class XmlStateRoot : public XmlState
        {
        public:
            typedef QExplicitlySharedDataPointer<XmlStateRoot> Ptr;

        public:
            XmlStateRoot();
            ~XmlStateRoot();

        public:
            Type type() const;

        protected:
            bool load(const QDomElement& elParent);
            bool save(QDomElement& elParent) const;
        };

        class XmlStateWindow : public XmlState
        {
        public:
            typedef QExplicitlySharedDataPointer<XmlStateWindow> Ptr;

        public:
            XmlStateWindow();

        public:
            Type type() const;

        protected:
            bool load(const QDomElement& elParent);
            bool save(QDomElement& elParent) const;
        };


        class ModeSwitcher
        {
        public:
            ModeSwitcher(XmlStateRoot::Ptr state);

        public:
            void run();

        private:
            void findOpened();
            void killUnused();

            void synchronizeWindows();
            void synchronizeWindow(Window* window, XmlStateWindow* state);
            void synchronizeContainer(ContainerWidget* container, XmlState* state);

            AbstractViewWidget* grabView(XmlStateView* view);
            AbstractViewWidget* grabSplitter(XmlStateSplitter* splitter);
            AbstractViewWidget* grabTab(XmlStateTab* tab);

            void cleanUpContainer(ContainerTypes unlessType, ContainerWidget*& cw);

            void associateViewContainer(AbstractViewWidget* view, XmlState* wsView);

        private:
            XmlStateRoot::Ptr    mState;

            QHash< ViewIdentifier, QPointer<View> > mExistingViews;
            QHash< ViewIdentifier, QPointer<ContainerWidget> > mExistingContainers;
            QHash< ViewIdentifier, QPointer<Window> > mExistingWindows;
        };

    }

}

Q_DECLARE_OPERATORS_FOR_FLAGS(BlueSky::Internal::XmlState::ChildTypes)



#ifndef BLUESKY_INTERNAL_MULTIBAR_HPP
#define BLUESKY_INTERNAL_MULTIBAR_HPP

#include <QWidget>

#include "libBlueSky/Internal/Frame.hpp"

class QBoxLayout;

namespace Heaven {

    class ToolBar;

}

namespace BlueSky {

    class View;

    namespace Internal {

        class MultiBarPrivate;
        class MultiBarSectionPrivate;

        class MultiBarSection : public QWidget
        {
            Q_OBJECT
        public:
            enum Flag
            {
                BreakAfter          = 1 << 0,
                NoSeparator         = 1 << 1,

                IsToolBar           = 1 << 16,
                IsViewsBar          = 1 << 17
            };
            typedef QFlags< Flag > Flags;

        protected:
            MultiBarSection( QWidget* parent, MultiBarSectionPrivate* p );

        public:
            MultiBarSection( QWidget* parent = 0 );
            ~MultiBarSection();

        public:
            void addWidget( QWidget* widget );
            void insertWidget( QWidget* widget, int index );
            void removeWidget( QWidget* widget );
            void removeWidget( int index );
            QWidget* takeWidget( QWidget* widget );
            QWidget* takeWidget( int index );
            int widgetCount() const;
            int indexOf( QWidget* widget ) const;
            QWidget* widgetAt( int index ) const;

            Flags flags() const;
            bool testFlag( Flag flag ) const;
            void setFlag( Flag flag, bool set = true );

            virtual void setOrientation( Qt::Orientation orientation );
            void setAlignment( Qt::Alignment alignment );
            void setStretch( int stretch );
            Qt::Orientation orientation() const;
            Qt::Alignment alignment() const;
            int stretch() const;

        protected:
            void paintEvent( QPaintEvent* ev );

        private:
            void relayoutBar();

        protected:
            MultiBarSectionPrivate* d;
        };

        class MultiBarSectionPrivate
        {
        public:
            MultiBarSectionPrivate();

        public:
            void relayout();

        public:
            MultiBarSection*        owner;
            QBoxLayout*             layout;
            QList< QWidget* >       widgets;
            MultiBarSection::Flags  flags;
            Qt::Orientation         orientation;
            int                     stretch;
            Qt::Alignment           alignment;
        };


        class MultiBarToolSection : public MultiBarSection
        {
            Q_OBJECT
        public:
            MultiBarToolSection( QWidget* parent = 0 );

        public:
            void setToolBar(Heaven::ToolBar* tb);
            Heaven::ToolBar* toolBar() const;
        };

        class MultiBarViewSection : public MultiBarSection
        {
            Q_OBJECT
        public:
            MultiBarViewSection( QWidget* parent = 0 );

        public:
            void insertView( int index, View* view );
            void removeView( View* view );

            void setActiveView( int index );
            int activeView() const;

            int indexOfView( View* view ) const;

            void setOrientation( Qt::Orientation orientation );

        signals:
            void currentChanged( int index );

        private slots:
            void activationChange( bool desiredActivation );
        };

        class MultiBar : public Frame
        {
            friend class MultiBarSection;
            Q_OBJECT
        public:
            MultiBar( QWidget* parent = 0 );
            ~MultiBar();

        public:
            int sectionCount() const;
            int addSection( MultiBarSection* section );
            int addSection();
            MultiBarSection* takeSection( int index );
            void insertSection( int index, MultiBarSection* section );
            void removeSection( int index );
            void removeAllSections();

            void setOrientation(Qt::Orientation orientation);

        private:
            class Private;
            Private* d();
            const Private* d() const;
        };

        class MultiBarViewWidget : public QWidget
        {
            Q_OBJECT
        public:
            MultiBarViewWidget( View* view, QWidget* parent = 0 );

        public:
            void setActive( bool active );
            bool isActive() const;

            View* view() const;
            QString text() const;

            void setOrientation(Qt::Orientation orientation);
            Qt::Orientation orientation() const;

            QSize minimumSizeHint() const;
            QSize sizeHint() const;

        protected:
            void paintEvent(QPaintEvent*);
            void leaveEvent(QEvent* ev);
            void enterEvent(QEvent* ev);
            void mousePressEvent(QMouseEvent* ev);
            void mouseReleaseEvent(QMouseEvent* ev);

        signals:
            void wantActivationChange( bool desiredActivation );

        private:
            void calcNiceSize() const;

        private:
            View* mView;
            QString mText;
            bool mIsPressed;
            bool mIsActive;
            bool mIsHovered;
            mutable QSize mNiceSize;
            Qt::Orientation mOrientation;
        };

        class MultiBar::Private : public Frame::Private
        {
        public:
            Private(MultiBar* owner);

        public:
            void relayout();

        public:
            QList< MultiBarSection* >   sections;
            QBoxLayout*                 layout;
        };

    }

}

#endif

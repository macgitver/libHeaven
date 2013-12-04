
#ifndef BLUESKY_INTERN_FRAME_HPP
#define BLUESKY_INTERN_FRAME_HPP

#include <QWidget>

namespace BlueSky {

    class Frame : public QWidget {
        Q_OBJECT
    public:
        enum Border {
            None    = 0,
            Top     = 1 << 0,
            Bottom  = 1 << 1,
            Left    = 1 << 2,
            Right   = 1 << 3
        };
        typedef QFlags<Border> Borders;

    protected:
        class Private;

    public:
        Frame(QWidget* parent = NULL);
        Frame(QWidget* parent, Private* _d);
        ~Frame();

    public:
        void setWidget(QWidget* widget);
        QWidget* widget() const;

        void setBorders(Borders f);
        Borders borders() const;

        virtual void setOrientation(Qt::Orientation orientation);
        Qt::Orientation orientation() const;

    public:
        QSize minimumSizeHint() const;

    protected:
        void resizeEvent(QResizeEvent*);
        void paintEvent(QPaintEvent*);

    protected:
        Private* d;
    };

    class Frame::Private {
    public:
        Private(Frame* owner);

    public:
        void doResize();

    public:
        Frame*          mOwner;
        QWidget*        mWidget;
        Qt::Orientation mOrientation;
        Frame::Borders  mBorders;
    };

}

Q_DECLARE_OPERATORS_FOR_FLAGS(BlueSky::Frame::Borders)

#endif

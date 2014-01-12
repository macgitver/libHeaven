
#include <QPainter>
#include <QDebug>

#include "libBlueSky/ColorSchema.hpp"
#include "libBlueSky/Frame.hpp"

namespace BlueSky {

    Frame::Private::Private(Frame* owner) {
        mOwner = owner;
        mBorders = None;
        mWidget = NULL;
        mOrientation = Qt::Vertical;
    }

    void Frame::Private::doResize() {
        if (mWidget) {
            QRect r = mOwner->contentsRect();

            if (mBorders.testFlag(Left)) {
                r.setLeft(r.left() + 1);
            }

            if (mBorders.testFlag(Right)) {
                r.setRight(r.right() - 1);
            }

            if (mBorders.testFlag(Top)) {
                r.setTop(r.top() + 1);
            }

            if (mBorders.testFlag(Bottom)) {
                r.setBottom(r.bottom() - 1);
            }

            mWidget->setGeometry(r);
        }
    }

    // ---------------------------------------------------------------------------------------------

    Frame::Frame(QWidget* parent)
        : QWidget(parent)
        , d(new Private(this))
    {
    }

    Frame::Frame(QWidget* parent, Private* _d)
        : QWidget(parent)
        , d(_d)
    {
    }

    Frame::~Frame() {
        delete d;
    }

    void Frame::setBorders(Borders f) {
        if (d->mBorders != f) {
            d->mBorders = f;
            d->doResize();
        }
    }

    Frame::Borders Frame::borders() const {
        return d->mBorders;
    }

    void Frame::setOrientation(Qt::Orientation orientation) {
        if (d->mOrientation != orientation) {
            d->mOrientation = orientation;
            update();
        }
    }

    Qt::Orientation Frame::orientation() const {
        return d->mOrientation;
    }

    void Frame::setWidget(QWidget* widget) {
        if (d->mWidget) {
            d->mWidget->hide();
            d->mWidget->setParent(NULL);
        }
        d->mWidget = widget;
        if (d->mWidget) {
            d->mWidget->setParent(this);
            d->mWidget->show();
        }
        update();
    }

    QWidget* Frame::widget() const {
        return d->mWidget;
    }

    void Frame::resizeEvent(QResizeEvent* ) {
        d->doResize();
    }

    void Frame::paintEvent(QPaintEvent* ) {
        QPainter painter(this);
        QRect rect = contentsRect();
        QRect inner = rect;
        QLine lines[4];
        int numLines = 0;

        //qDebug() << inner;

        if (d->mBorders != None) {

            if (d->mBorders.testFlag(Left)) {
                lines[numLines++] = QLine(0, 0, 0, rect.height());
            }

            if (d->mBorders.testFlag(Right)) {
                lines[numLines++] = QLine(rect.right(), rect.top(),
                                          rect.right(), rect.bottom());
            }

            if (d->mBorders.testFlag(Top)) {
                lines[numLines++] = QLine(0, 0, rect.width(), 0);
            }

            if (d->mBorders.testFlag(Bottom)) {
                lines[numLines++] = QLine(0, rect.height()-1, rect.width(), rect.height()-1);
            }
        }

        QLinearGradient grad;
        if (d->mOrientation == Qt::Vertical) {
            grad.setStart(0, inner.top());
            grad.setFinalStop(0, inner.bottom());
            grad.setColorAt(0.0, ColorSchema::get(clrTtbGradientHigh));
            grad.setColorAt(1.0, ColorSchema::get(clrTtbGradientLow));
        }
        else {
            grad.setStart(inner.left(), 0);
            grad.setFinalStop(inner.right(), 0);
            grad.setColorAt(0.0, ColorSchema::get(clrLtrGradientHigh));
            grad.setColorAt(1.0, ColorSchema::get(clrLtrGradientLow));
        }

        painter.fillRect(inner, grad);

        if (numLines) {
            QPen pen(ColorSchema::get(clrSeparator));
            pen.setWidth(0);
            painter.setPen(pen);
            painter.drawLines(lines, numLines);
        }
    }

    QSize sizeMin(const QSize& a, const QSize& b) {
        if (!a.isValid()) {
            return b;
        }
        if (!b.isValid()) {
            return a;
        }
        return QSize(qMin(a.width(), b.width()), qMin(a.height(), b.height()));
    }

    QSize Frame::minimumSizeHint() const {
        QSize s;

        if (d->mWidget) {
            s = sizeMin(s, d->mWidget->minimumSize());
            if (!s.isValid()) {
                s = sizeMin(s, d->mWidget->minimumSizeHint());
            }
            return s;
        }

        return sizeMin(s, QWidget::minimumSizeHint() );
    }

}

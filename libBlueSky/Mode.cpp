
#include <QDebug>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QDomElement>

#include "libHeavenIcons/Icon.hpp"

#include "libBlueSky/Mode.hpp"
#include "libBlueSky/Application.hpp"
#include "libBlueSky/ColorSchema.hpp"
#include "libBlueSky/Internal/XmlState.hpp"
#include "libBlueSky/Internal/Frame.hpp"

namespace BlueSky {

    class Mode::Data {
    public:
        Data(const QByteArray& identifier) {
            mIdentifier = identifier;
            mDisplayOrder = 0;
            mEnabled = true;
            mHidden = false;
            mExclusive = false;
        }

    public:
        int                         mDisplayOrder;
        Heaven::IconRef             mIcon;
        QString                     mName;
        QByteArray                  mIdentifier;

        Internal::XmlStateRoot::Ptr mDefaultState;
        Internal::XmlStateRoot::Ptr mCurrentState;

        bool                        mEnabled        : 1;
        bool                        mExclusive      : 1;
        bool                        mHidden         : 1;
    };

    Mode::Mode(const QByteArray& identifier)
        : d(new Data(identifier))
    {
    }

    Mode::~Mode() {
        delete d;
    }

    bool Mode::isEnabled() const {
        return d->mEnabled;
    }

    bool Mode::isExclusive() const {
        return d->mExclusive;
    }

    bool Mode::isHidden() const {
        return d->mHidden;
    }

    Heaven::IconRef Mode::icon() const {
        return d->mIcon;
    }

    QString Mode::name() const {
        return d->mName;
    }

    QByteArray Mode::identifier() const {
        return d->mIdentifier;
    }

    int Mode::displayOrder() const {
        return d->mDisplayOrder;
    }

    void Mode::setEnabled(bool enabled) {
        if (d->mEnabled != enabled) {
            d->mEnabled = enabled;
            emit changed();
        }
    }

    void Mode::setHidden(bool hidden) {
        if (d->mHidden != hidden) {
            d->mHidden = hidden;
            emit changed();
        }
    }

    void Mode::setExclusive(bool exclusive) {
        if (d->mExclusive != exclusive) {
            d->mExclusive = exclusive;
            emit changed();
        }
    }

    void Mode::setDisplayOrder(int order) {
        d->mDisplayOrder = order;
        emit orderChanged();
    }

    void Mode::setIcon(const Heaven::IconRef& icon) {
        d->mIcon = icon;
        emit changed();
    }

    void Mode::setName(const QString& name) {
        if (d->mName != name) {
            d->mName = name;
            emit changed();
        }
    }

    Internal::XmlStateRoot::Ptr Mode::currentState() const {
        return d->mCurrentState;
    }

    bool Mode::readFromState(const QDomElement& el, bool isDefault) {

        QDomNodeList nl = el.childNodes();
        if (!nl.count()) {
            return false;   // Must be at least <Id>
        }

        for (int i = 0; i < nl.count(); i++) {
            QDomNode node = nl.at(i);

            if (!node.isElement()) {
                continue;
            }

            QDomElement elChild = node.toElement();
            QString name = elChild.tagName();
            if (name == QLatin1String("Sky:Id")) {
                if (elChild.text().toUtf8() != d->mIdentifier) {
                    qDebug() << "Tried to read into wrong mode.";
                    return false;
                }
            }
            else if (name == QLatin1String("Sky:Name")) {
                setName(elChild.text());
            }
            else if (name == QLatin1String("Sky:Layout")) {
                Internal::XmlState::Ptr xmlState = Internal::XmlState::read(elChild);
                if (!xmlState) {
                    qDebug() << "Failed to read mode:" << d->mName
                             << "Identifier=" << d->mIdentifier;
                    return false;
                }

                if (isDefault) {
                    d->mDefaultState = xmlState;
                }

                d->mCurrentState = xmlState;
            }
            else {
                qDebug() << "Unknown tag:" << name;
                return false;
            }
        }

        return true;
    }

    namespace Internal {

        ModeView::ModeView() {
            mActive = NULL;
            rebuildInfos();

            Application* app = Application::instance();
            connect(app, SIGNAL(activeModeChanged(BlueSky::Mode*)),
                    SLOT(activeModeChanged(BlueSky::Mode*)));
            connect(app, SIGNAL(modeAdded(BlueSky::Mode*)),
                    SLOT(modeAdded(BlueSky::Mode*)));
            connect(app, SIGNAL(modeAboutToRemove(BlueSky::Mode*)),
                    SLOT(modeAboutToRemove(BlueSky::Mode*)));
            connect(app, SIGNAL(modesReordered()),
                    SLOT(modesReordered()));
        }

        ModeView::~ModeView() {
        }

        void ModeView::modesReordered() {
            rebuildInfos();
        }

        void ModeView::modeAdded(BlueSky::Mode* mode) {
            rebuildInfos();
        }

        void ModeView::modeAboutToRemove(BlueSky::Mode* mode) {
            rebuildInfos();
        }

        void ModeView::activeModeChanged(BlueSky::Mode* mode) {
            mActive = mode;
            rebuildInfos();
            update();
        }

        void ModeView::rebuildInfos() {
            mModeInfos.clear();

            if (mActive && mActive->isExclusive()) {
                ModeInfo mi;
                mi.mMode = mActive;
                mModeInfos.append(mi);
            }
            else {
                foreach (Mode* mode, Application::instance()->allModesOrdered()) {
                    if (mode->isHidden() && mode != mActive) {
                        continue;
                    }
                    ModeInfo mi;
                    mi.mMode = mode;
                    mModeInfos.append(mi);
                }
            }
            recalcInfos();
        }

        void ModeView::recalcInfos() {
            int top = 4;
            int w = width();

            for (int i = 0; i < mModeInfos.count(); i++) {
                int is = mModeInfos[i].mMode->icon().size();
                int h = 13 + 4 + is;
                mModeInfos[i].mRect = QRect(0, top, w, h);
                int l = (w - is) / 2;
                mModeInfos[i].mIconRect = QRect(l, top, is, is);
                top += is;
                mModeInfos[i].mTextRect = QRect(0, top, w, 13);
                top += 13 + 4;
            }

            setMinimumWidth(70);
            setMinimumHeight(top + 4);
        }

        void ModeView::mouseMoveEvent(QMouseEvent* ev) {
            bool needUpdate = false;

            for (int i = 0; i < mModeInfos.count(); i++) {
                bool newHover = mModeInfos[i].mRect.contains(ev->pos());
                if (mModeInfos[i].mHovered != newHover) {
                    needUpdate = true;
                    mModeInfos[i].mHovered = newHover;
                }
            }

            if (needUpdate) {
                update();
            }
        }

        void ModeView::enterEvent(QEvent*) {
            setMouseTracking(true);
        }

        void ModeView::leaveEvent(QEvent*) {
            bool needUpdate = false;
            setMouseTracking(false);

            for (int i = 0; i < mModeInfos.count(); i++) {
                needUpdate |= mModeInfos[i].mHovered;
                mModeInfos[i].mHovered = false;
            }

            if (needUpdate) {
                update();
            }
        }

        void ModeView::resizeEvent(QResizeEvent*) {
            recalcInfos();
        }

        void ModeView::paintEvent(QPaintEvent*) {
            QPainter painter(this);
            for (int i = 0; i < mModeInfos.count(); i++) {
                const ModeInfo& mi = mModeInfos[i];
                const bool active = mi.mMode == mActive;

                if (active) {
                    QLinearGradient grad(0, 0, width(), 0);
                    grad.setColorAt(0.1, ColorSchema::get(clrCurModeGradientLow, 0));
                    grad.setColorAt(0.5, ColorSchema::get(clrCurModeGradientHigh));
                    grad.setColorAt(0.9, ColorSchema::get(clrCurModeGradientLow, 0));
                    painter.fillRect(mi.mRect, grad);
                }
                else if (mi.mHovered) {
                    QLinearGradient grad(0, 0, width(), 0);
                    grad.setColorAt(0.0, ColorSchema::get(clrCurModeGradientLow,64));
                    grad.setColorAt(1.0, ColorSchema::get(clrCurModeGradientHigh,64));
                    painter.fillRect(mi.mRect, grad);
                }
                //painter.fillRect(mi.mIconRect, Qt::green);
                painter.drawPixmap(mi.mIconRect, mi.mMode->icon().icon().pixmap());

                QFont f;
                f.setBold(true);
                f.setPixelSize(10);
                painter.setFont(f);

                QRect r = mi.mTextRect.adjusted(1,1,0,0);
                painter.setPen(ColorSchema::get(active ? clrCurModeTextShadow : clrModeTextShadow));
                painter.drawText(r, Qt::AlignCenter, mi.mMode->name());

                r = mi.mTextRect.adjusted(0,0,-1,-1);
                painter.setPen(ColorSchema::get(active ? clrCurModeText : clrModeText));
                painter.drawText(r, Qt::AlignCenter, mi.mMode->name());
            }
        }

        // ModeColumn --------------------------------------------------------------------------- >8

        ModeColumn::ModeColumn() {

            QVBoxLayout* l = new QVBoxLayout;
            l->setMargin(0);
            l->setSpacing(0);

            setLayout(l);

            mTopFrame = new Frame;
            mTopFrame->setOrientation(Qt::Vertical);
            mTopFrame->setBorders(Frame::Bottom | Frame::Right);
            mTopFrame->setMaximumHeight(25);
            mTopFrame->setMinimumHeight(25);
            l->addWidget(mTopFrame);

            mModeView = new ModeView;

            mModeFrame = new Frame;
            mModeFrame->setOrientation(Qt::Horizontal);
            mModeFrame->setBorders(Frame::Right);
            mModeFrame->setWidget(mModeView);
            l->addWidget(mModeFrame);
        }

    }

}

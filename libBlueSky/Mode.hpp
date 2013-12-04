
#ifndef BLUESKY_MODE_HPP
#define BLUESKY_MODE_HPP

#include <QObject>
#include <QByteArray>
#include <QIcon>
#include <QWidget>

class QDomElement;

#include "libHeavenIcons/IconRef.hpp"

#include "libBlueSky/libBlueSkyAPI.hpp"

namespace BlueSky {

    class Frame;

    namespace Internal {
        class XmlStateRoot;
    }

    class HEAVEN_BLUESKY_API Mode : public QObject {
        Q_OBJECT
    public:
        typedef QVector<Mode*> List;

    public:
        Mode(const QByteArray& identifier);
        ~Mode();

    public:
        int displayOrder() const;
        Heaven::IconRef icon() const;
        QString name() const;
        QByteArray identifier() const;
        bool isEnabled() const;
        bool isExclusive() const;
        bool isHidden() const;

        QExplicitlySharedDataPointer<Internal::XmlStateRoot> currentState() const;

    public:
        void setDisplayOrder(int order);
        void setIcon(const Heaven::IconRef& icon);
        void setName(const QString& name);
        void setEnabled(bool enabled);
        void setHidden(bool hidden);
        void setExclusive(bool exclusive);

    signals:
        void orderChanged();
        void changed();

    public:
        bool readFromState(const QDomElement& el, bool isDefault);

    private:
        class Data;
        Data* d;
    };

    namespace Internal {

        class ModeView : public QWidget {
            Q_OBJECT
        public:
            ModeView();
            ~ModeView();

        public:

        private slots:
            void modesReordered();
            void modeAdded(BlueSky::Mode* mode);
            void modeAboutToRemove(BlueSky::Mode* mode);
            void activeModeChanged(BlueSky::Mode* mode);

        protected:
            void resizeEvent(QResizeEvent*);
            void paintEvent(QPaintEvent*);
            void mouseMoveEvent(QMouseEvent*);
            void enterEvent(QEvent*);
            void leaveEvent(QEvent*);

        private:
            void rebuildInfos();
            void recalcInfos();

        private:
            struct ModeInfo {
                ModeInfo() : mMode(NULL), mHovered(false) {}
                Mode*   mMode;
                QRect   mRect;
                QRect   mTextRect;
                QRect   mIconRect;
                bool    mHovered;
            };
            QVector<ModeInfo> mModeInfos;
            Mode* mActive;
        };

        class ModeColumn : public QWidget {
            Q_OBJECT
        public:
            ModeColumn();

        private:
            Frame*          mTopFrame;
            Frame*          mModeFrame;
            ModeView*       mModeView;
        };

    }

}

#endif

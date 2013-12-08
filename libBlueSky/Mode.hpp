/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

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
        Mode(const QByteArray& identifier, QObject* parent = 0);
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

    public slots:
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
            void modeChanged();

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
                QIcon   mCachedIcon;
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

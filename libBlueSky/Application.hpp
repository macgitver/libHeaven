
#ifndef BLUESKY_APPLICATION_HPP
#define BLUESKY_APPLICATION_HPP

#include <QObject>
#include <QHash>
#include <QVector>

#include "libBlueSky/Mode.hpp"

class QDomDocument;

namespace BlueSky {

    class Window;
    class PrimaryWindow;
    class Mode;
    class ViewDescriptor;
    class ViewIdentifier;

    class HEAVEN_BLUESKY_API Application : public QObject {
        Q_OBJECT
    public:
        Application();
        ~Application();

    public:
        static Application* instance();

    public:
        PrimaryWindow* primaryWindow();
        QVector<Window*> allWindows() const;
        Window* window(const ViewIdentifier& id, bool create = false);

    public:
        void addMode(Mode* mode);
        void removeMode(Mode* mode);
        void removeMode(const QByteArray& identifier);
        Mode* findMode(const QByteArray& identifier) const;
        Mode::List allModes() const;
        Mode::List allModesOrdered() const;
        Mode* activeMode();
        void setActiveMode(Mode* mode);
        void setActiveMode(const QByteArray& identifier);
        void setModelessView(ViewDescriptor* view);
        ViewDescriptor* modelessView();

    public:
        bool loadState(const QString& fileName, bool isDefault = false);
        bool loadState(const QDomDocument& doc, bool isDefault = false);

    signals:
        void activeModeChanged(BlueSky::Mode* mode);
        void modesReordered();
        void modeAdded(BlueSky::Mode* mode);
        void modeAboutToRemove(BlueSky::Mode* mode);

    private slots:
        void queueModeReorder();
        void emitModesReordered();

    protected:
        virtual PrimaryWindow* newPrimaryWindow();
        virtual Window* newSecondaryWindow();

    private:
        static Application* sInstance;
        QVector<Window*> mWindows;
        QHash<QByteArray, Mode*> mModes;
        ViewDescriptor* mModelessView;
        Mode* mActiveMode;

        bool mModeReorderQueued;
    };

}

#endif

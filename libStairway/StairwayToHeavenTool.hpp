
#pragma once

#include <QPointer>
#include <QWidget>
#include <QAbstractItemModel>
#include <QTreeView>

#ifdef Stairway_EXPORTS
#define STAIRWAY_API Q_DECL_EXPORT
#else
#define STAIRWAY_API Q_DECL_IMPORT
#endif

/*
 * And she's buying a stairway to heaven
 * There's a sign on the wall, but she
 * wants to be sure cause you know
 * sometimes words have two meaingins.
 *
 * In a tree by the brook there's a
 * songbird that sings: sometimes all
 * of our thoughts are misgiven.
 *
 *                            (Led Zeppelin)
 *
 * This library is meant to find false stairways. Esp. in Qt's Widget hierarchies.
 */

namespace Heaven {

    namespace StairwayTool {

        namespace Private {

            class StairwayModel;

            enum Types {
                StepType_Root,
                StepType_Object,
                StepType_Widget,
                StepType_Layout
            };

            class AbstractStep : public QObject
            {
                Q_OBJECT
            protected:
                AbstractStep(StairwayModel* m, AbstractStep* parent);
                ~AbstractStep();

            public:
                void updateMe();
                virtual void suicide();

            public:
                virtual Types type() const = 0;
                virtual QVariant data(int col, int role) = 0;

            public:
                void discovered(QObject* child);
                AbstractStep* parentStep() const;
                QVector<AbstractStep*> steps() const { return subSteps; }

            public:
                StairwayModel* model;
                AbstractStep* parent;
                QVector<AbstractStep*> subSteps;
            };

            class ObjectStep : public AbstractStep
            {
                Q_OBJECT
            public:
                ObjectStep(StairwayModel* m, QObject* o, AbstractStep* parent);
                ~ObjectStep();

            public:
                void suicide();
                Types type() const;
                QVariant data(int col, int role);

            private slots:
                void objectDied();

            protected:
                bool eventFilter(QObject* o, QEvent* e);

            private:
                void doParentChange();

            public:
                QObject* getObject() const;

            protected:
                QString className;
                quintptr objectPtr;
                QPointer<QObject> object;
            };

            class WidgetStep : public ObjectStep
            {
                Q_OBJECT
            public:
                WidgetStep(StairwayModel* m, QWidget* w, AbstractStep* parent);
                ~WidgetStep();

            public:
                Types type() const;
                QVariant data(int col, int role);
            };

            class RootStep : public AbstractStep
            {
                Q_OBJECT
            public:
                RootStep(StairwayModel* m);

            public:
                Types type() const;
                QVariant data(int col, int role);
            };

            /*
            class LayoutStep : public ObjectStep
            {
            };
            */

            class StairwayModel : public QAbstractItemModel
            {
                friend class AbstractStep;
                friend class ObjectStep;
                Q_OBJECT
            public:
                StairwayModel(QWidget *root);
                ~StairwayModel();

            public:
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
                int rowCount(const QModelIndex &parent = QModelIndex()) const;
                int columnCount(const QModelIndex &parent = QModelIndex()) const;
                QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
                QModelIndex parent(const QModelIndex &child) const;

                ObjectStep* findStep(QObject* o) const;
                QModelIndex indexFor(AbstractStep* step, int col = 0) const;

            private:
                RootStep* mRoot;
            };

        }

        class STAIRWAY_API StairwayView : public QWidget
        {
            Q_OBJECT
        public:
            StairwayView();

        public:
            void setRoot(QWidget* root);

        private:
            QWidget* rootWidget;
            Private::StairwayModel* stepsModel;
            QTreeView* stepsView;
        };

    }

}

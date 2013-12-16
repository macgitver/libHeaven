
#include <QFont>
#include <QTimer>
#include <QHeaderView>
#include <QDebug>
#include <QVBoxLayout>
#include <QStringBuilder>
#include <QEvent>
#include <QStack>

#include "libStairway/StairwayToHeavenTool.hpp"

static inline QString rect2string(const QRect& rect) {
    return QString::fromUtf8("%1:%2 %3x%4")
            .arg(rect.left())
            .arg(rect.top())
            .arg(rect.width())
            .arg(rect.height());
}

namespace Heaven {

    namespace StairwayTool {

        namespace Private {

            AbstractStep::AbstractStep(StairwayModel* m, AbstractStep* parent)
                : model(m)
                , parent(parent)
            {
                if (parent) {
                    parent->subSteps.append(this);
                }
            }

            AbstractStep::~AbstractStep()
            {
                if (parent) {
                    int i = parent->subSteps.indexOf(this);
                    if (i != -1) {
                        parent->subSteps.remove(i);
                    }
                }

                foreach (AbstractStep* subStep, subSteps) {
                    subStep->suicide();
                }

                Q_ASSERT(subSteps.count() == 0);
            }

            void AbstractStep::suicide()
            {
                if (parent) {
                    int i = parent->subSteps.indexOf(this);
                    QModelIndex parentIndex = model->indexFor(parent);

                    model->beginRemoveRows(parentIndex, i, i);
                    parent->subSteps.remove(i);
                    parent = NULL;
                    model->endRemoveRows();
                }

                delete this;
            }

            void AbstractStep::updateMe()
            {
                QModelIndex idx1 = model->indexFor(this, 0);
                QModelIndex idx2 = model->indexFor(this, 3);
                model->dataChanged(idx1, idx2);
            }

            AbstractStep* AbstractStep::parentStep() const
            {
                return parent;
            }

            void AbstractStep::discovered(QObject* child) {

                AbstractStep* stepChild = NULL;

                QModelIndex idx = model->indexFor(this);
                model->beginInsertRows(idx, subSteps.count(), subSteps.count());

                if (QWidget* widget = qobject_cast<QWidget*>(child)) {
                    stepChild = new WidgetStep(model, widget, this);
                }
                /*
                else if (QLayout* layout = qobject_cast<QLayout>(child)) {
                }
                */
                else {
                    stepChild = new ObjectStep(model, child, this);
                }

                model->endInsertRows();
            }

            // ---

            ObjectStep::ObjectStep(StairwayModel* m, QObject* o, AbstractStep* parent)
                : AbstractStep(m, parent)
                , objectPtr(quintptr(o))
                , object(o)
            {
                connect(o, SIGNAL(destroyed()),
                        this, SLOT(objectDied()));

                className = QString::fromUtf8(object->metaObject()->className());

                object->installEventFilter(this);

                foreach(QObject* objectChild, o->children()) {
                    discovered(objectChild);
                }
            }

            ObjectStep::~ObjectStep()
            {
                if (object) {
                    object->removeEventFilter(this);
                }
            }

            void ObjectStep::suicide()
            {
                qDebug() << "Suicide of:" << className << QString::number(objectPtr,16);
                AbstractStep::suicide();
            }

            Types ObjectStep::type() const
            {
                return StepType_Object;
            }

            void ObjectStep::objectDied()
            {
                qDebug() << QString::number(objectPtr, 16) << className << "has died";
                suicide();
            }

            void ObjectStep::doParentChange()
            {
                AbstractStep* oldParentStep = parent;

                Q_ASSERT(oldParentStep);
                int i = oldParentStep->steps().indexOf(this);
                if (i != -1) {
                    QModelIndex parentIndex = model->indexFor(oldParentStep);
                    model->beginRemoveRows(parentIndex, i, i);
                    oldParentStep->subSteps.remove(i);
                    parent = NULL;
                    model->endRemoveRows();
                }

                QObject* newQParent = object->parent();
                AbstractStep* newParent = model->findStep(newQParent);

                if (!newParent) {
                    newParent = model->mRoot;
                }

                QModelIndex idx = model->indexFor(newParent);
                model->beginInsertRows(idx, newParent->subSteps.count(), newParent->subSteps.count());

                newParent->subSteps.append(this);
                parent = newParent;

                model->endInsertRows();
            }

            bool ObjectStep::eventFilter(QObject* o, QEvent* e)
            {
                if (o == object) {
                    switch (e->type()) {
                    case QEvent::ChildAdded: {
                        QChildEvent* ev = static_cast<QChildEvent*>(e);
                        discovered(ev->child());
                        break; }

                    case QEvent::Hide:
                    case QEvent::Show:
                    case QEvent::HideToParent:
                    case QEvent::ShowToParent:
                    case QEvent::ContentsRectChange:
                        updateMe();
                        break;

                    case QEvent::ParentAboutToChange:
                        qDebug() << "About TO Parent-Change for" << QString::number(objectPtr, 16) << className;
                        break;

                    case QEvent::ParentChange:
                        qDebug() << "Parent-Change for" << QString::number(objectPtr, 16) << className;
                        doParentChange();
                        break;

                    default:
                        /* foo bar, we don't want that event*/
                        break;
                    }
                }

                return AbstractStep::eventFilter(o, e);
            }

            QVariant ObjectStep::data(int col, int role)
            {
                if (role == Qt::DisplayRole) {
                    switch(col) {
                    case 0:
                        return className;
                    case 1:
                        return object.isNull() ? QLatin1String("<Killed>") : object->objectName();
                    case 2:
                        return QVariant(
                                    QLatin1Literal("QObject@") %
                                    QString::number(objectPtr, 16)
                               );
                    case 3:
                        return QString();
                    }
                }

                return QVariant();
            }

            QObject* ObjectStep::getObject() const
            {
                return object.data();
            }

            // ---

            WidgetStep::WidgetStep(StairwayModel* m, QWidget* w, AbstractStep* parent)
                : ObjectStep(m, w, parent)
            {
            }

            WidgetStep::~WidgetStep()
            {
                if (QWidget* widget = qobject_cast<QWidget*>(object)) {
                    widget->removeEventFilter(this);
                }
            }

            Types WidgetStep::type() const
            {
                return StepType_Widget;
            }

            QVariant WidgetStep::data(int col, int role)
            {
                if (role == Qt::DisplayRole) {
                    switch(col) {
                    case 0:
                        return className;
                    case 1:
                        return object.isNull() ? QLatin1String("<Killed>") : object->objectName();
                    case 2:
                        return QVariant(
                                    QLatin1Literal("QWidget@") %
                                    QString::number(objectPtr, 16)
                                );
                    case 3:
                        if (QWidget* widget = qobject_cast<QWidget*>(object)) {
                            if (widget->isVisible()) {
                                return rect2string(widget->geometry());
                            }
                            else {
                                return QVariant(
                                            QLatin1Literal("Hidden: ") %
                                            rect2string(widget->geometry())
                                        );
                            }
                        }
                    }
                }
                else if (role == Qt::ForegroundRole) {
                    if (QWidget* widget = qobject_cast<QWidget*>(object)) {
                        if (!widget->isVisible()) {
                            return QColor(127,127,127);
                        }
                    }
                }

                return QVariant();
            }

            // ---

            RootStep::RootStep(StairwayModel* m)
                : AbstractStep(m, NULL)
            {
            }

            QVariant RootStep::data(int col, int role)
            {
                return QVariant();
            }

            Types RootStep::type() const
            {
                return StepType_Root;
            }

            // ---

            StairwayModel::StairwayModel(QWidget* root)
            {
                mRoot = new RootStep(this);
                new Private::WidgetStep(this, root, mRoot);
            }

            StairwayModel::~StairwayModel()
            {
                delete mRoot;
            }

            QVariant StairwayModel::data(const QModelIndex &index, int role) const
            {
                if (!index.isValid()) {
                    return QVariant();
                }

                AbstractStep* step = static_cast<AbstractStep*>(index.internalPointer());
                if (!step) {
                    return QVariant();
                }
                return step->data(index.column(), role);
            }

            int StairwayModel::rowCount(const QModelIndex &parent) const
            {
                AbstractStep* par;
                par = parent.isValid() ? static_cast<AbstractStep*>(parent.internalPointer())
                                       : mRoot;
                return par->steps().count();
            }

            int StairwayModel::columnCount(const QModelIndex &parent) const
            {
                return 4;
            }

            QModelIndex StairwayModel::index(int row, int column, const QModelIndex &parent) const
            {
                if (!hasIndex(row, column, parent)) {
                    return QModelIndex();
                }

                AbstractStep* par;
                par = parent.isValid() ? static_cast<AbstractStep*>(parent.internalPointer())
                                       : mRoot;

                AbstractStep* child = par->steps()[row];
                return child ? createIndex(row, column, child) : QModelIndex();
            }

            QModelIndex StairwayModel::indexFor(AbstractStep* step, int col) const
            {
                AbstractStep* par;
                par = step->parentStep() ? step->parentStep() : mRoot;

                int row = par->steps().indexOf(step);
                return (row != -1) ? createIndex(row, col, step) : QModelIndex();
            }

            QModelIndex StairwayModel::parent(const QModelIndex &child) const
            {
                if (child.isValid()) {
                    AbstractStep* childStep = static_cast<AbstractStep*>(child.internalPointer());
                    AbstractStep* parentStep = childStep->parentStep();

                    if (parentStep && parentStep != mRoot) {
                        AbstractStep* granny = parentStep->parentStep();
                        int row = granny->steps().indexOf(parentStep);
                        return createIndex(row, 0, parentStep);
                    }
                }
                return QModelIndex();
            }

            ObjectStep* StairwayModel::findStep(QObject* o) const
            {
                QStack<AbstractStep*> todos;
                todos.append(mRoot);

                while (!todos.empty()) {
                    AbstractStep* cur = todos.pop();
                    if (cur) {
                        ObjectStep* os;

                        switch(cur->type()) {
                        case StepType_Object:
                        case StepType_Widget:
                            os = static_cast<ObjectStep*>(cur);
                            if (os->getObject() == o) {
                                return os;
                            }

                        default:
                            break;
                        }

                        foreach (AbstractStep* step, cur->steps()) {
                            todos.push(step);
                        }
                    }
                }

                return NULL;
            }

        }

        StairwayView::StairwayView()
            : rootWidget(NULL)
            , stepsModel(NULL)
        {
            stepsView = new QTreeView;
            stepsView->header()->setResizeMode(QHeaderView::ResizeToContents);
            stepsView->header()->setVisible(false);
            QFont f;
            f.setPixelSize(10);
            stepsView->setFont(f);

            QVBoxLayout*l = new QVBoxLayout;
            l->addWidget(stepsView);
            l->setMargin(0);
            setLayout(l);

            QTimer* t = new QTimer(this);
            connect(t, SIGNAL(timeout()), stepsView, SLOT(expandAll()));
            t->start(500);
        }

        void StairwayView::setRoot(QWidget* root)
        {
            if (stepsModel) {
                stepsModel->deleteLater();
            }

            rootWidget = root;

            stepsModel = new Private::StairwayModel(root);
            stepsView->setModel(stepsModel);
        }

    }

}

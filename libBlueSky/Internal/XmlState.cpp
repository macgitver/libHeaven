
#include <QQueue>
#include <QDebug>
#include <QUuid>
#include <QDomElement>

#include "libBlueSky/Application.hpp"
#include "libBlueSky/Windows.hpp"
#include "libBlueSky/ViewDescriptor.hpp"
#include "libBlueSky/Internal/XmlState.hpp"

namespace BlueSky {

    namespace Internal {

        XmlState::XmlState()
            : mParent()
            , mCurrentContent(NULL)
        {
        }

        XmlState::~XmlState() {
            Q_ASSERT(mChildren.count() == 0);

            if (mParent) {
                int i = mParent->mChildren.indexOf(Ptr(this));

                if(i != -1) {
                    mParent->mChildren.remove(i);
                }
            }
        }

        void XmlState::append(const Ptr& child) {
            Q_ASSERT(child);
            Q_ASSERT(!child->mParent);

            mChildren.append(child);
            child->mParent = this;
        }

        XmlState::Ptr XmlState::read(const QDomElement& el) {
            XmlState::Ptr result;

            QString name = el.tagName();

            if (name == QLatin1String("Sky:Splitter")) {
                result = new XmlStateSplitter;
            }
            else if (name == QLatin1String("Sky:Tab")) {
                result = new XmlStateTab;
            }
            else if (name == QLatin1String("Sky:View")) {
                result = new XmlStateView;
            }
            else if (name == QLatin1String("Sky:Window")) {
                result = new XmlStateWindow;
            }
            else if (name == QLatin1String("Sky:Layout")) {
                result = new XmlStateRoot;
            }
            else {
                qDebug() << "Don't know what to do with a" << name;
                return Ptr();
            }

            if (!result->load(el)) {
                result = Ptr();
            }

            return result;
        }

        bool XmlState::readChildren(const QDomElement& elParent, ChildTypes allowed) {

            QStringList tags;
            QStringList skipTags;

            if (allowed & CTContainers) {
                tags << QLatin1String("Sky:Splitter")
                     << QLatin1String("Sky:Tab");
            }

            if (allowed & CTViews) {
                tags << QLatin1String("Sky:View");
            }

            if (allowed & CTWindows) {
                tags << QLatin1String("Sky:Window");
            }

            skipTags << QLatin1String("Sky:Id")
                     << QLatin1String("Sky:Horizontal")
                     << QLatin1String("Sky:Vertical")
                     << QLatin1String("Sky:Top")
                     << QLatin1String("Sky:Left")
                     << QLatin1String("Sky:Bottom")
                     << QLatin1String("Sky:Right");

            QDomElement e = elParent.firstChildElement();

            while (e.isElement()) {
                QString tag = e.tagName();

                if (!skipTags.contains(tag)) {

                    if (tags.contains(tag)) {
                        XmlState::Ptr child = read(e);

                        if (!child) {
                            qDebug() << "Could not read a" << tag;
                            return false;
                        }

                        append(child);
                    }
                    else {
                        qDebug() << "Ignoring invalid XmlState-Tag: " << tag;
                    }
                }

                e = e.nextSiblingElement();
            }

            return true;
        }

        bool XmlState::readOrCreateIdentifier(const QDomElement& el) {

            if (!el.isElement()) {
                return false;
            }

            QDomElement eId = el.firstChildElement(QLatin1String("Sky:Id"));
            if (eId.isElement()) {
                mId = eId.text();
            }

            if (!mId.isValid()) {
                mId = QUuid::createUuid().toString();
            }

            return true;
        }

        QDomElement XmlState::save(QDomDocument& doc) const {
            QDomElement el = doc.createElement(QLatin1String("Sky:Layout"));

            if (!saveChildren(el)) {
                return QDomElement();
            }

            return el;
        }

        bool XmlState::saveChildren(QDomElement& elParent) const {
            foreach (const Ptr& ws, mChildren) {
                if (!ws->save(elParent)) {
                    return false;
                }
            }
            return true;
        }

        bool XmlState::saveIdentifier(QDomElement& el) const {

            if (!mId.isValid()) {
                return false;
            }

            QDomDocument doc = el.ownerDocument();
            QDomElement elId = doc.createElement(QLatin1String("Sky:Id"));
            QDomText txt = doc.createTextNode(mId.toString());

            elId.appendChild(txt);
            el.appendChild(elId);
            return true;
        }

        void XmlState::setCurrentContent( AbstractViewWidget* avw )
        {
            if( avw != mCurrentContent )
            {
                if( mCurrentContent )
                {
                    // TODO: mCurrentContent->setXmlState( NULL );
                }

                mCurrentContent = avw;

                if( mCurrentContent )
                {
                    /* TODO:
                    XmlStateBase* wsOld = mCurrentContent->XmlState();
                    if( wsOld )
                    {
                        wsOld->setCurrentContent( NULL );
                    }
                    mCurrentContent->setXmlState( this );
                    */
                }
            }
        }

        void XmlState::updateConfig() {
            foreach (const Ptr& ws, mChildren) {
                ws->updateConfig();
            }
        }

        AbstractViewWidget* XmlState::currentContent() {
            return mCurrentContent;
        }

        int XmlState::childrenCount() const {
            return mChildren.count();
        }

        XmlState::Ptr XmlState::childAt(int index) const {
            return mChildren.at(index);
        }

        QVector< XmlState::Ptr > XmlState::children() const {
            return mChildren;
        }

        ViewIdentifier XmlState::identifier() const {
            return mId;
        }

        void XmlState::setIdentifier(const ViewIdentifier& id) {
            mId = id;
        }

        // -----------------------------------------------------------------------------------------

        XmlStateSplitter::XmlStateSplitter() {
        }

        bool XmlStateSplitter::load(const QDomElement& elParent) {

            if (!readOrCreateIdentifier(elParent)) {
                return false;
            }

            mVertical = true;
            if (elParent.firstChildElement(QLatin1String("Sky:Horizontal")).isElement()) {
                mVertical = false;
            }

            return readChildren(elParent, CTContainers|CTViews);
        }

        bool XmlStateSplitter::save(QDomElement& elParent) const {

            QDomDocument doc = elParent.ownerDocument();
            QDomElement elChild = doc.createElement(QLatin1String("Sky:Splitter"));
            elParent.appendChild(elChild);

            if (!saveIdentifier(elChild)) {
                return false;
            }

            elChild.appendChild(doc.createElement(QLatin1String(mVertical
                                                                ? "Vertical"
                                                                : "Horizontal")));

            return saveChildren(elChild);
        }

        void XmlStateSplitter::updateConfig() {
            XmlState::updateConfig();
        }

        XmlState::Type XmlStateSplitter::type() const {
            return WSSplitter;
        }

        void XmlStateSplitter::setVertical(bool value) {
            mVertical = value;
        }

        bool XmlStateSplitter::isVertical() const {
            return mVertical;
        }

        // -----------------------------------------------------------------------------------------

        XmlStateTab::XmlStateTab() {
        }

        bool XmlStateTab::load(const QDomElement& elParent) {

            if (!readOrCreateIdentifier(elParent)) {
                return false;
            }

            mPositions = Top;
            if (elParent.firstChildElement(QLatin1String("Sky:Bottom")).isElement()) {
                mPositions = Bottom;
            }
            else if (elParent.firstChildElement(QLatin1String("Sky:Left")).isElement()) {
                mPositions = Left;
            }
            else if (elParent.firstChildElement(QLatin1String("Sky:Right")).isElement()) {
                mPositions = Right;
            }
            else if (!elParent.firstChildElement(QLatin1String("Sky:Top")).isElement()) {
                return false;
            }

            return readChildren(elParent, CTContainers|CTViews);
        }

        static inline const char* posText(Positions pos) {
            switch(pos) {
            case Bottom:   return "Sky:Bottom";
            case Top:      return "Sky:Top";
            case Left:     return "Sky:Left";
            case Right:    return "Sky:Right";
            default:       return NULL;
            }
        }

        bool XmlStateTab::save(QDomElement& elParent) const {

            QDomDocument doc = elParent.ownerDocument();
            QDomElement elChild = doc.createElement(QLatin1String("Sky:Tab"));
            elParent.appendChild(elChild);

            if (!saveIdentifier(elChild)) {
                return false;
            }

            const char* s = posText(mPositions);
            if (s) {
                elChild.appendChild(doc.createElement(QString::fromUtf8(s)));
            }

            return saveChildren(elChild);
        }

        void XmlStateTab::updateConfig() {
            XmlState::updateConfig();
        }

        XmlState::Type XmlStateTab::type() const {
            return WSTab;
        }

        void XmlStateTab::setTabPosition(Positions pos) {
            mPositions = pos;
        }

        Positions XmlStateTab::tabPosition() const {
            return mPositions;
        }

        // -----------------------------------------------------------------------------------------

        XmlStateView::XmlStateView() {
        }

        bool XmlStateView::load(const QDomElement& elParent) {
            return readOrCreateIdentifier(elParent);
        }

        void XmlStateView::updateConfig()
        {
        }

        bool XmlStateView::save(QDomElement& elParent) const {

            QDomDocument doc = elParent.ownerDocument();
            QDomElement elChild = doc.createElement(QLatin1String("Sky:View"));
            elParent.appendChild(elChild);

            return saveIdentifier(elChild);
        }

        XmlState::Type XmlStateView::type() const
        {
            return WSView;
        }

        // -----------------------------------------------------------------------------------------

        XmlStateRoot::XmlStateRoot() {
        }

        XmlStateRoot::~XmlStateRoot() {
        }

        bool XmlStateRoot::load(const QDomElement& elParent) {
            return readChildren(elParent, CTWindows);
        }

        bool XmlStateRoot::save(QDomElement& elParent) const {

            QDomDocument doc = elParent.ownerDocument();
            QDomElement elChild = doc.createElement(QLatin1String("Sky:Layout"));
            elParent.appendChild(elChild);

            return saveChildren(elChild);
        }

        XmlState::Type XmlStateRoot::type() const {
            return WSRoot;
        }

        // -----------------------------------------------------------------------------------------

        XmlStateWindow::XmlStateWindow() {
        }

        bool XmlStateWindow::load(const QDomElement& elParent) {
            return readOrCreateIdentifier(elParent)
                && readChildren(elParent, CTContainers);
        }

        XmlState::Type XmlStateWindow::type() const
        {
            return WSWindow;
        }

        bool XmlStateWindow::save(QDomElement& elParent) const {

            QDomDocument doc = elParent.ownerDocument();
            QDomElement elChild = doc.createElement(QLatin1String("Sky:Window"));
            elParent.appendChild(elChild);

            return saveIdentifier(elChild)
                && saveChildren(elChild);
        }

        // -----------------------------------------------------------------------------------------

        /**
         * @internal
         * @class       ModeSwitcher
         * @brief       Handle switching modes
         *
         * The ModeSwitcher class handles all the gory details of switching a mode.
         *
         * After a pointer to the desired state is given through the constructor, the run() method
         * updates the current user interface to reflect the state as designated by the WindowStateRoot.
         *
         */

        /**
         * @brief       Constructor
         *
         * Setup a ModeSwitcher to synchronize the current CentralUI with a given WindowStateRoot.
         *
         * @param[in]   state   The WindowStateRoot to synchronize the current user interface to
         */
        ModeSwitcher::ModeSwitcher(XmlStateRoot::Ptr state)
            : mState( state )
        {
        }

        /**
         * @brief       Main work-horse.
         *
         */
        void ModeSwitcher::run() {
            findOpened();
            synchronizeWindows();
            killUnused();
        }

        /**
         * @brief       synchronize all windows
         *
         * Iterates over all windows that _shall be in the final state_ and matches them to existing
         * windows, creating new (secondary) windows on the fly.
         *
         * For each of those windows synchronzieWindow() is called, which will synchronize the window's
         * rootContainer().
         *
         */
        void ModeSwitcher::synchronizeWindows() {
            Q_ASSERT(mState);

            foreach (const XmlState::Ptr& ws, mState->children()) {

                Q_ASSERT(ws->type() == XmlState::WSWindow);

                XmlStateWindow* windowState = static_cast<XmlStateWindow*>(ws.data());
                ViewIdentifier windowId = windowState->identifier();

                Window* window = mExistingWindows.take(windowId);
                if(!window) {
                    // this actually creates new windows (since we haven't found a window by that id
                    // in the take() above the conditional.
                    window = Application::instance()->window(windowId, true);

                    // TODO: new secondary windows should be made visible.
                }

                Q_ASSERT(window);
                synchronizeWindow(window, windowState);
            }
        }

        /**
         * @brief       synchronize a Top-Level HeavenWindow
         *
         * @param[in]   window      The window to synchronize
         *
         * @param[in]   state       The partial state to synch this window to.
         *
         * This method just asserts that the @a window is in the right shape and then calls out to
         * synchronize the rootContainer of the @a window.
         *
         */
        void ModeSwitcher::synchronizeWindow(Window* window, XmlStateWindow* state) {
            Q_ASSERT(state->childrenCount() == 1); // A window may only have _one_ Container as child
                                                   // and may not be empty.

            XmlState* child = state->childAt(0).data();
            Q_ASSERT( child->type() != XmlState::WSView
                   && child->type() != XmlState::WSWindow
                   && child->type() != XmlState::WSRoot );

            ContainerWidget* cw = window->rootContainer();
            synchronizeContainer(cw, child);
        }

        /**
         * @brief       synchronize a container
         *
         * @param[in]   container   The container to synchronize.
         *
         * @param[in]   state       The partial state to synch this container to.
         *
         * Synchronizes the container @a container and it's desired children to be as written down in
         * the WindowStateBase @a state.
         *
         * The process involves 3 steps:
         * 1.   find or create any View or ContainerWidget objects required to fill the container.
         *      This will recurse into sub containers.
         * 2.   remove anything that is left over in the @a container.
         * 3.   add the found objects to @a container.
         */
        void ModeSwitcher::synchronizeContainer(ContainerWidget* container, XmlState* state) {
            QList< AbstractViewWidget* > newContents;

            // We start by creating a list of what shall be inside the container. Thus we call the
            // grabXXX methods which will search anything existing and give it to us (in case it is
            // reusable). If grabXXX won't find anything, it will create something new.

            // During this run, we recurse into sub-containers.

            foreach (const XmlState::Ptr& ws, state->children()) {
                AbstractViewWidget* next = NULL;

                switch(ws->type()) {
                case XmlState::WSView:
                    next = grabView(static_cast<XmlStateView*>(ws.data()));
                    break;

                case XmlState::WSSplitter:
                    next = grabSplitter(static_cast<XmlStateSplitter*>(ws.data()));
                    synchronizeContainer(next->asContainerWidget(), ws.data());
                    break;

                case XmlState::WSTab:
                    next = grabTab(static_cast<XmlStateTab* >(ws.data()));
                    synchronizeContainer(next->asContainerWidget(), ws.data());
                    break;

                default:
                    break;
                }

                Q_ASSERT_X(next,
                           "synchronizeContainer",
                           qPrintable(
                               QString(QLatin1String("Invalid window state (%1) for child "
                                                     "\"%2\" of container \"%3\""))
                               .arg(ws->type())
                               .arg(ws->identifier().toString())
                               .arg(container->identifier().toString())
                           ));

                associateViewContainer(next, ws.data());
                newContents.append(next);
            }

            // We have now taken all views that we want to reuse "out" of the container, leaving only
            // the ones which are referenced by mExistingViews but shall no longer be inside this
            // container. If they will not be reused until the end of this run, killUnsed() will take
            // care of conserving their content and delete the views.

            // So, all we need to do here is to take anything that's still inside the container away.

            while(container->count()) {
                container->takeAt(container->count() - 1);
            }

            // Insert all Views/Containers we want into the ContainerWidget
            foreach (AbstractViewWidget* viewToAdd, newContents) {
                container->add(viewToAdd);
            }
        }

        /**
         * @brief       find or create a view
         *
         * @param[in]   view    The View-State to look for
         *
         * @return      The found view or a new view.
         *
         * When a view is reused, it is removed from the mExistingViews member.
         *
         */
        AbstractViewWidget* ModeSwitcher::grabView(XmlStateView* view) {

            Q_ASSERT(view);
            ViewIdentifier viewId = view->identifier();

            View* realView = mExistingViews.take(viewId);
            if (realView) {
                ContainerWidget* parent = realView->parentContainer();
                parent->take(realView);
                return realView;
            }

            ViewDescriptor* vd = ViewDescriptor::get(viewId);

            return vd ? vd->createView() : NULL;
        }

        /**
         * @brief       find or create a splitter
         *
         * @param[in]   splitter    The Splitter-State to look for
         *
         * @return      The found splitter or a newly created one.
         *
         * When the ContainerWidget with the required identifier is not a SplitterContainerWidget, it
         * will be replaced by a newly created one.
         *
         */
        AbstractViewWidget* ModeSwitcher::grabSplitter(XmlStateSplitter* splitter) {
            Q_ASSERT( splitter );
            ViewIdentifier id = splitter->identifier();

            SplitterContainerWidget* sc;

            ContainerWidget* cw = mExistingContainers.take(id);
            cleanUpContainer(SplitterContainerType, cw);

            sc = static_cast<SplitterContainerWidget*>(cw);
            if (!sc) {
                sc = new SplitterContainerWidget(id);
            }

            sc->setVertical(splitter->isVertical());
            return sc;
        }

        /**
         * @brief       find or create a tab (MultiBar)
         *
         * @param[in]   tab     The Tab-State to look for
         *
         * @return      The found tab or a newly created one.
         *
         * When the ContainerWidget with the required identifier is not a MultiBarContainerWidget, it
         * will be replaced by a newly created one.
         *
         */
        AbstractViewWidget* ModeSwitcher::grabTab(XmlStateTab* tab) {
            Q_ASSERT( tab );
            ViewIdentifier id = tab->identifier();

            ContainerWidget* cw = mExistingContainers.take(id);
            cleanUpContainer(MultiBarContainerType, cw);

            MultiBarContainerWidget* mbw = static_cast<MultiBarContainerWidget*>(cw);
            if (!mbw) {
                mbw = new MultiBarContainerWidget(id);
            }

            mbw->setBarPosition(tab->tabPosition());
            return mbw;
        }

        /**
         * @brief           clean up a container if it is not of a specific type
         *
         * @param[in]       unlessType  If the ContainerWidget @a cw is valid and of this type, nothing
         *                              will be done at all.
         *
         * @param[in,out]   cw          The ContainerWidget to clean up. This may be `NULL` on input, in
         *                              which case nothing is done. It will be set to `NULL` on output,
         *                              if the container was cleaned up (and deleted).
         *
         * This method is called to prohibit the usage of a SplitterContainerWidget when a
         * MultiBarContainerWidget is expected and vice versa. This can be the case if they have wrongly
         * the same identifier.
         *
         * When it is discovered that such a case is present, anything inside the container will be
         * taken away (It is still referenced in mExistingContainers and mExistingViews) and the
         * ContainerWidget @a cw will be deleted.
         *
         * It is assumed that @a cw is not in mExistingContainers.
         */
        void ModeSwitcher::cleanUpContainer( ContainerTypes unlessType, ContainerWidget*& cw )
        {
            if (!cw || cw->containerType() == unlessType) {
                return;
            }

            while (cw->count()) {
                cw->takeAt(cw->count() - 1);
            }

            delete cw;
            cw = NULL;
        }

        /**
         * @brief       update association between a view/container and it's current state
         *
         * @param[in]   view    The view to update
         * @param[in]   wsView  The WindowState to update
         *
         * Views and WindowStateViews hold a non-counted reference to each other. This is used to call
         * into the view when the state is saved.
         *
         */
        void ModeSwitcher::associateViewContainer(AbstractViewWidget* avw, XmlState* wsView) {
            // TODO: ws->setCurrentContent( vcc );
        }

        /**
         * @brief       find opened views, containers and windows
         *
         * Searches for currently opened views, containers and windows and poplates the mExistingXXX
         * members.
         *
         * As soon as an entry therein is reused, it must be removed from those hashes. Any view,
         * container or window still existing there will be deleted by killUnused() at the end of the
         * synchornization run.
         *
         */
        void ModeSwitcher::findOpened() {
            foreach (Window* hw, Application::instance()->allWindows()) {
                ViewIdentifier windowId = hw->identifier();
                mExistingWindows.insert(windowId, hw);
                QQueue<ContainerWidget*> visit;

                visit.enqueue(hw->rootContainer());

                while (!visit.isEmpty()) {
                    ContainerWidget* cw = visit.dequeue();

                    if (!cw) {
                        continue;
                    }

                    for (int i = 0; i < cw->count(); ++i) {
                        AbstractViewWidget* content = cw->widget(i);
                        if (content->isContainerWidget()) {
                            ContainerWidget* child = content->asContainerWidget();
                            Q_ASSERT(child);

                            mExistingContainers.insert(content->identifier(), child);
                            visit.enqueue(child);
                        }
                        else {
                            View* view = qobject_cast<View*>(content);
                            Q_ASSERT(view);

                            mExistingViews.insert(content->identifier(), view);
                        }
                    }
                }
            }
        }

        /**
         * @brief       Delete any unused objects left over by the synchronization
         *
         */
        void ModeSwitcher::killUnused() {
            foreach (View* view, mExistingViews) {
                delete view;
            }

            foreach (AbstractViewWidget* avw, mExistingContainers) {
                delete avw;
            }

            foreach (Window* hw, mExistingWindows) {
                delete hw;
            }
        }

    }

}

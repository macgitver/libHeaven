/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Cunz RaD Ltd
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

#include <QDebug>
#include <QAction>

#include "libBlueSky/Views.hpp"
#include "libBlueSky/ViewDescriptor.hpp"
#include "libBlueSky/Internal/ViewRegistrar.hpp"

namespace BlueSky {

    /**
     * @class   ViewDescriptor
     * @ingroup CentralUI
     * @brief   A descriptor for available views
     *
     * This class stores meta data about the available views. It also acts as a factory to create
     * real View objects, when required.
     *
     * Each instance of this class represents one View.
     *
     * @typedef ViewDescriptor::CreatorFunc
     * @brief   Pointer to a creator function that creates a view
     *
     */

    /**
     * @brief   Constructor
     *
     * @param[in]   id              The identifier to register.
     *
     * @param[in]   displayName     The translated text to display when referring to this View.
     *
     * @param[in]   creator         Creator function that is able to create this type of View
     *                              object.
     *
     * The descriptor must be created on the __heap__. It will be automatically registered.
     *
     */
    ViewDescriptor::ViewDescriptor( const ViewIdentifier& id, const QString& displayName,
                                    ViewDescriptor::CreatorFunc creator )
        : mIdentifier(id)
        , mDisplayName(displayName)
        , mCreatorFunc(creator)
        , mCreatedView(NULL)
        , mActivatorAction(NULL)
    {
        Internal::ViewRegistrar::self().insert( id, this );
    }

    ViewDescriptor::~ViewDescriptor()
    {
        delete mActivatorAction;
    }

    /**
     * @brief       Get the display name for this descriptor
     *
     * @return      A translated text suitable to refer to the View.
     *
     */
    QString ViewDescriptor::displayName() const
    {
        return mDisplayName;
    }

    /**
     * @brief       Get the identifier for this descriptor
     *
     * @return      The identifier for this descriptor.
     *
     */
    ViewIdentifier ViewDescriptor::identifier() const
    {
        return mIdentifier;
    }

    /**
     * @brief       Get the creator function for this descriptor
     *
     * @return      The creator function for this descriptor
     *
     */
    ViewDescriptor::CreatorFunc ViewDescriptor::creatorFunc() const
    {
        return mCreatorFunc;
    }

    /**
     * @brief       Unregisters this descriptor and deletes it
     *
     */
    void ViewDescriptor::unregister()
    {
        Internal::ViewRegistrar::self().remove(mIdentifier);
        delete this;
    }

    /**
     * @brief       Lookup a descriptor by its identifier
     *
     * @param[in]   id  The identifier to look up.
     *
     * @return      Pointer to the found ViewDescriptor or `NULL` if no descriptor was found.
     *
     */
    ViewDescriptor* ViewDescriptor::get( const ViewIdentifier& id )
    {
        return Internal::ViewRegistrar::self().get(id);
    }

    /**
     * @brief       Create a new view
     *
     * @return      Pointer to the new View.
     *
     * Invokes the creator function and forwards its return value.
     *
     */
    View* ViewDescriptor::createView() const
    {
        View* view = mCreatorFunc();
        if (!view) {
            return NULL;
        }

        mCreatedView = view;

        Internal::ViewRegistrar::self().viewOpened(view);

        return view;
    }

    /**
     * @brief       Get the activator action of this view
     *
     * @return      A QAction that can be used to open/close this view
     */
    QAction* ViewDescriptor::activatorAction()
    {
        if (!mActivatorAction) {
            createActivatorAction();
        }

        return mActivatorAction;
    }

    Heaven::DynamicActionMerger* ViewDescriptor::actionMerger()
    {
        return Internal::ViewRegistrar::self().actionMerger();
    }

    void ViewDescriptor::createActivatorAction()
    {
        if (mActivatorAction) {
            return;
        }

        mActivatorAction = new QAction(mDisplayName, &Internal::ViewRegistrar::self());
        mActivatorAction->setCheckable(true);
    }

    void ViewDescriptor::mergeViewsMenu(const QByteArray& mergePlace)
    {
        Internal::ViewRegistrar::self().acViewsMergerAC->mergeInto(mergePlace);
    }

    void ViewDescriptor::onActivatorAction(bool triggered)
    {
        if (triggered && !mCreatedView) {
            qDebug() << "Should open:" << mIdentifier;
        }
        else if (!triggered && mCreatedView) {
            mCreatedView->closeView();
        }
    }

    void ViewDescriptor::setViewClosed()
    {
        mCreatedView = NULL;
    }

}

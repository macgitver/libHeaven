
QT_PREPARE(Core Gui Widgets)

SET(SRC_FILES
    Action.cpp
    ActionGroup.cpp
    ActionContainer.cpp
    DynamicActionMerger.cpp
    Menu.cpp
    MenuBar.cpp
    MergePlace.cpp
    MergesManager.cpp
    Separator.cpp
    ToolBar.cpp
    UiContainer.cpp
    UiManager.cpp
    UiObject.cpp
    WidgetAction.cpp
    WidgetActionWrapper.cpp
)

SET(HDR_PUB_FILES
    libHeavenActionsAPI.hpp
    HeavenActions.hpp

    Action.hpp
    ActionGroup.hpp
    ActionContainer.hpp
    DynamicActionMerger.hpp
    Menu.hpp
    MenuBar.hpp
    MergePlace.hpp
    ToolBar.hpp
    WidgetAction.hpp
    UiObject.hpp
)

SET(HDR_PRI_FILES

    HeavenActionsPrivate.hpp
    ActionGroupPrivate.hpp
    ActionContainerPrivate.hpp
    ActionPrivate.hpp
    DynamicActionMergerPrivate.hpp
    MenuBarPrivate.hpp
    MenuPrivate.hpp
    MergePlacePrivate.hpp
    MergesManager.hpp
    Separator.hpp
    ToolBarPrivate.hpp
    UiContainer.hpp
    UiManager.hpp
    UiObjectPrivate.hpp
    WidgetActionPrivate.hpp
    WidgetActionWrapper.hpp
)

SET(HDR_FILES ${HDR_PUB_FILES} ${HDR_PRI_FILES})

QT_MOC(MOC_FILES ${HDR_FILES})

ADD_QT_LIBRARY(
    HeavenActions SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    HeavenActions

    LINK_PRIVATE
        HeavenIcons
)

RAD_SET_TARGET_VERSION(HeavenActions HEAVEN)
RAD_SPLIT_SOURCE_TREE(HeavenActions)
RAD_INSTALL_LIBRARY(Heaven HeavenActions)
RAD_INSTALL_HEADERS(libHeavenActions ${HDR_PUB_FILES})


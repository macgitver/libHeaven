
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

SET(HDR_FILES
    libHeavenActionsAPI.hpp
    HeavenActions.hpp
    HeavenActionsPrivate.hpp

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


PROJECT( MGV_HEAVEN )

QT_PREPARE( Core Gui Xml Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_HEAVEN_SOURCE_DIR}
    ${MGV_HEAVEN_BINARY_DIR}
)

SET( SRC_FILES

    Heaven.cpp

    Actions/Action.cpp
    Actions/ActionContainer.cpp
    Actions/DynamicActionMerger.cpp
    Actions/Menu.cpp
    Actions/MenuBar.cpp
    Actions/MergePlace.cpp
    Actions/MergesManager.cpp
    Actions/Separator.cpp
    Actions/ToolBar.cpp
    Actions/UiContainer.cpp
    Actions/UiManager.cpp
    Actions/UiObject.cpp
    Actions/WidgetAction.cpp
    Actions/WidgetActionWrapper.cpp

    App/Dialog.cpp
    App/Application.cpp
    App/HeavenWindow.cpp
    App/PrimaryWindow.cpp
    App/SecondaryWindow.cpp
    App/ModeSwitcher.cpp
    App/Mode.cpp

    ColorSchemata/ColorManager.cpp
    ColorSchemata/ColorSchema.cpp
    ColorSchemata/ColorSet.cpp
    ColorSchemata/ColorSchemaEditor.cpp

    Icons/Icon.cpp
    Icons/IconManager.cpp
    Icons/IconDefaultProvider.cpp
    Icons/IconProvider.cpp
    Icons/IconRef.cpp

    MultiBar/MultiBar.cpp
    MultiBar/MultiBarSection.cpp
    MultiBar/MultiBarToolSection.cpp
    MultiBar/MultiBarViewSection.cpp
    MultiBar/MultiBarViewWidget.cpp

    Style/Style.cpp

    CentralUI/States/WindowState.cpp
    CentralUI/States/WindowStateRoot.cpp
    CentralUI/States/WindowStateSplitter.cpp
    CentralUI/States/WindowStateTab.cpp
    CentralUI/States/WindowStateView.cpp
    CentralUI/States/WindowStateWindow.cpp

    CentralUI/Views/ContextView.cpp
    CentralUI/Views/View.cpp
    CentralUI/Views/AbstractViewWidget.cpp
    CentralUI/Views/ViewDescriptor.cpp
    CentralUI/Views/ViewIdentifier.cpp

    CentralUI/Contexts/ContextKeys.cpp
    CentralUI/Contexts/ViewContext.cpp
    CentralUI/Contexts/ViewContextData.cpp
    CentralUI/Contexts/ViewContextManager.cpp

    CentralUI/ContainerWidgets/ContainerWidget.cpp
    CentralUI/ContainerWidgets/MultiBarContainerWidget.cpp
    CentralUI/ContainerWidgets/SplitterContainerWidget.cpp

    Tools/KVStore.cpp

    Widgets/FooterWidget.cpp
    Widgets/ModeSwitchWidget.cpp
    Widgets/ModeSwitchCombo.cpp
    Widgets/MiniSplitter.cpp
    Widgets/TabBar.cpp
    Widgets/TabWidget.cpp
)

SET( PUB_HDR_FILES

    Heaven.hpp
    HeavenApi.hpp

    Actions/Action.hpp
    Actions/ActionContainer.hpp
    Actions/DynamicActionMerger.hpp
    Actions/Menu.hpp
    Actions/MenuBar.hpp
    Actions/MergePlace.hpp
    Actions/ToolBar.hpp
    Actions/WidgetAction.hpp
    Actions/UiObject.hpp

    App/Dialog.hpp
    App/Application.hpp
    App/HeavenWindow.hpp
    App/PrimaryWindow.hpp
    App/SecondaryWindow.hpp
    App/IStoredState.hpp
    App/Mode.hpp

    ColorSchemata/ColorManager.hpp
    ColorSchemata/ColorSchema.hpp
    ColorSchemata/ColorSchemaEditor.hpp

    Icons/Icon.hpp
    Icons/IconManager.hpp
    Icons/IconDefaultProvider.hpp
    Icons/IconProvider.hpp
    Icons/IconRef.hpp

    MultiBar/MultiBar.hpp
    MultiBar/MultiBarSection.hpp

    CentralUI/Views/ContextView.hpp
    CentralUI/Views/View.hpp
    CentralUI/Views/AbstractViewWidget.hpp
    CentralUI/Views/ViewDescriptor.hpp
    CentralUI/Views/ViewIdentifier.hpp

    CentralUI/Contexts/ViewContext.hpp
    CentralUI/Contexts/ViewContextData.hpp
    CentralUI/Contexts/ContextKeys.hpp

    CentralUI/ContainerWidgets/ContainerWidget.hpp

    Widgets/FooterWidgetPrivate.hpp
    Widgets/ModeSwitchWidget.hpp
    Widgets/MiniSplitter.hpp

    Tools/KVStore.hpp
)

SET( PRI_HDR_FILES

    HeavenPrivate.hpp

    Actions/ActionContainerPrivate.hpp
    Actions/ActionPrivate.hpp
    Actions/DynamicActionMergerPrivate.hpp
    Actions/MenuBarPrivate.hpp
    Actions/MenuPrivate.hpp
    Actions/MergePlacePrivate.hpp
    Actions/MergesManager.hpp
    Actions/Separator.hpp
    Actions/ToolBarPrivate.hpp
    Actions/UiContainer.hpp
    Actions/UiManager.hpp
    Actions/UiObjectPrivate.hpp
    Actions/WidgetActionPrivate.hpp
    Actions/WidgetActionWrapper.hpp

    App/ApplicationPrivate.hpp
    App/HeavenWindowPrivate.hpp
    App/PrimaryWindowPrivate.hpp
    App/SecondaryWindowPrivate.hpp
    App/ModeSwitcher.hpp
    App/ModePrivate.hpp

    ColorSchemata/ColorSchemaPrivate.hpp
    ColorSchemata/ColorManagerPrivate.hpp
    ColorSchemata/ColorSet.hpp

    Icons/IconPrivate.hpp
    Icons/IconManagerPrivate.hpp
    Icons/IconDefaultProviderPrivate.hpp

    MultiBar/MultiBarPrivate.hpp
    MultiBar/MultiBarToolSection.hpp
    MultiBar/MultiBarViewSection.hpp
    MultiBar/MultiBarSectionPrivate.hpp
    MultiBar/MultiBarViewWidget.hpp

    Tools/AdvancedCache.hpp

    Style/Style.hpp

    CentralUI/Contexts/ViewContextManager.hpp
    CentralUI/Contexts/ViewContextPrivate.hpp

    CentralUI/ContainerWidgets/MultiBarContainerWidget.hpp
    CentralUI/ContainerWidgets/SplitterContainerWidget.hpp

    CentralUI/States/WindowState.hpp
    CentralUI/States/WindowStateRoot.hpp
    CentralUI/States/WindowStateSplitter.hpp
    CentralUI/States/WindowStateTab.hpp
    CentralUI/States/WindowStateView.hpp
    CentralUI/States/WindowStateWindow.hpp

    Widgets/FooterWidget.hpp
    Widgets/ModeSwitchCombo.hpp
    Widgets/TabWidget.hpp
    Widgets/TabBar.hpp
)

SET( UI_FILES
    ColorSchemata/ColorSchemaEditor.ui
)

SET( RCC_FILES
    Resources/Heaven.rcc
)

SET( HID_FILES
    CentralUI/ContainerWidgets/MultiBarContainerWidgetActions.hid
    MultiBar/MultiBarViewWidgetActions.hid
)

# These files are not relevant to the build process
# but shall be listed in QtCreator for easier editing
SET( DOX_FILES
    CentralUI/CentralUI.dox
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
QT_RCC( RCC_DATA QRC_FILES ${RCC_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_QT_LIBRARY(
    Heaven SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${HIC_FILES} ${HID_FILES}
    ${QRC_FILES}
    ${RCC_DATA}
    ${DOX_FILES}
)

INSTALL_FRAMEWORK( Heaven Runtime )

MSVC_SPLIT_SOURCES( Heaven )

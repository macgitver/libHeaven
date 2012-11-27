
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
    App/PrimaryWindow.cpp
    App/SecondaryWindow.cpp

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
    MultiBar/MultiBarContainer.cpp
    MultiBar/MultiBarSection.cpp
    MultiBar/MultiBarToolSection.cpp
    MultiBar/MultiBarViewSection.cpp
    MultiBar/MultiBarViewWidget.cpp

    Style/Style.cpp

    Views/ContextManager.cpp
    Views/ContextKeys.cpp
    Views/GlobalView.cpp
    Views/DependantView.cpp
    Views/ContextView.cpp
    Views/MultipleView.cpp
    Views/Decorator.cpp
    Views/GlobalView.cpp
    Views/Mode.cpp
    Views/TopLevelWidget.cpp
    Views/View.cpp
    Views/ViewContainer.cpp
    Views/ViewContainerContent.cpp
    Views/ViewContext.cpp
    Views/WindowState.cpp

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

    Actions/Action.h
    Actions/ActionContainer.h
    Actions/Menu.h
    Actions/MenuBar.h
    Actions/MergePlace.h
    Actions/ToolBar.h
    Actions/WidgetAction.h

    App/Dialog.hpp
    App/Application.hpp
    App/PrimaryWindow.hpp
    App/SecondaryWindow.hpp

    ColorSchemata/ColorManager.hpp
    ColorSchemata/ColorSchema.hpp
    ColorSchemata/ColorSchemaEditor.hpp

    Icons/Icon.hpp
    Icons/IconManager.hpp
    Icons/IconDefaultProvider.hpp
    Icons/IconProvider.hpp
    Icons/IconRef.hpp

    MultiBar/MultiBarPrivate.hpp
    MultiBar/MultiBarToolSection.hpp
    MultiBar/MultiBarViewSection.hpp
    MultiBar/MultiBarSectionPrivate.hpp
    MultiBar/MultiBarViewWidget.hpp

    Views/GlobalView.hpp
    Views/DependantView.hpp
    Views/ContextView.hpp
    Views/ContextKeys.hpp
    Views/MultipleView.hpp
    Views/Mode.h
    Views/TopLevelWidget.h
    Views/View.h
    Views/ViewContainer.h
    Views/ViewContainerContent.h
    Views/ViewContext.h
    Views/WindowState.h

    Widgets/FooterWidgetPrivate.hpp
    Widgets/ModeSwitchWidget.h
    Widgets/MiniSplitter.h
)

SET( PRI_HDR_FILES

    HeavenPrivate.hpp

    Actions/ActionContainerPrivate.h
    Actions/ActionPrivate.h
    Actions/MenuBarPrivate.h
    Actions/MenuPrivate.h
    Actions/MergePlacePrivate.h
    Actions/MergesManager.h
    Actions/Separator.h
    Actions/ToolBarPrivate.h
    Actions/UiContainer.h
    Actions/UiManager.h
    Actions/UiObject.h
    Actions/WidgetActionPrivate.h
    Actions/WidgetActionWrapper.h

    App/ApplicationPrivate.hpp
    App/PrimaryWindowPrivate.hpp
    App/SecondaryWindowPrivate.hpp

    ColorSchemata/ColorSchemaPrivate.hpp
    ColorSchemata/ColorManagerPrivate.hpp
    ColorSchemata/ColorSet.hpp

    Icons/IconManagerPrivate.hpp
    Icons/IconDefaultProviderPrivate.hpp

    MultiBar/MultiBarContainer.hpp
    MultiBar/MultiBar.hpp
    MultiBar/MultiBarSection.hpp

    Style/Style.h

    Views/ContextManager.hpp
    Views/ModePrivate.h
    Views/Decorator.h

    Widgets/FooterWidget.hpp
    Widgets/ModeSwitchCombo.h
    Widgets/TabWidget.h
    Widgets/TabBar.h
)

SET( UI_FILES
    ColorSchemata/ColorSchemaEditor.ui
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )

ADD_QT_LIBRARY(
    Heaven SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES}
)

#INSTALL_MGV_LIB( Heaven )

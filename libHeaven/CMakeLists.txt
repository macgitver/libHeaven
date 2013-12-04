
PROJECT( MGV_HEAVEN )

QT_PREPARE( Core Gui Xml Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_HEAVEN_SOURCE_DIR}
    ${MGV_HEAVEN_BINARY_DIR}
)

SET( SRC_FILES

    Heaven.cpp

    App/Dialog.cpp
    App/Application.cpp
    App/HeavenWindow.cpp
    App/PrimaryWindow.cpp
    App/SecondaryWindow.cpp
    App/ModeSwitcher.cpp
    App/Mode.cpp

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
    CentralUI/Views/ViewDescriptorRegistrar.cpp
    CentralUI/Views/ViewIdentifier.cpp

    CentralUI/Contexts/ContextKeys.cpp
    CentralUI/Contexts/ViewContext.cpp
    CentralUI/Contexts/ViewContextData.cpp
    CentralUI/Contexts/ViewContextManager.cpp

    CentralUI/ContainerWidgets/ContainerWidget.cpp
    CentralUI/ContainerWidgets/MultiBarContainerWidget.cpp
    CentralUI/ContainerWidgets/SplitterContainerWidget.cpp

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

    App/Dialog.hpp
    App/Application.hpp
    App/HeavenWindow.hpp
    App/PrimaryWindow.hpp
    App/SecondaryWindow.hpp
    App/IStoredState.hpp
    App/Mode.hpp

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
)

SET( PRI_HDR_FILES

    HeavenPrivate.hpp

    App/ApplicationPrivate.hpp
    App/HeavenWindowPrivate.hpp
    App/PrimaryWindowPrivate.hpp
    App/SecondaryWindowPrivate.hpp
    App/ModeSwitcher.hpp
    App/ModePrivate.hpp

    MultiBar/MultiBarPrivate.hpp
    MultiBar/MultiBarToolSection.hpp
    MultiBar/MultiBarViewSection.hpp
    MultiBar/MultiBarSectionPrivate.hpp
    MultiBar/MultiBarViewWidget.hpp

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

    CentralUI/Views/ViewDescriptorRegistrar.hpp

    Widgets/FooterWidget.hpp
    Widgets/ModeSwitchCombo.hpp
    Widgets/TabWidget.hpp
    Widgets/TabBar.hpp
)

SET( RCC_FILES
    Resources/Heaven.rcc
)

SET( HID_FILES
    CentralUI/Views/ViewsMergerActions.hid
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

TARGET_LINK_LIBRARIES(
    Heaven

    LINK_PRIVATE
        HeavenActions
)

INSTALL_FRAMEWORK( Heaven Runtime )

MSVC_SPLIT_SOURCES( Heaven )

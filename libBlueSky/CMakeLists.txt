
QT_PREPARE(Core Gui Widgets Xml)

SET(SRC_FILES
    Windows.cpp
    Containers.cpp
    Views.cpp
    Mode.cpp
    Application.cpp
    ColorSchema.cpp
    ViewDescriptor.cpp
    FooterWidget.cpp
    Frame.cpp
    MiniSplitter.cpp
    Contexts.cpp
    Style.cpp
    Dialog.cpp

    Internal/MultiBar.cpp
    Internal/ViewRegistrar.cpp
    Internal/XmlState.cpp
    Internal/ContextManager.cpp

    Internal/HierarchyDumper.cpp
)

SET(HDR_FILES
    libBlueSkyAPI.hpp

    Windows.hpp
    Containers.hpp
    Views.hpp
    Mode.hpp
    Application.hpp
    ColorSchema.hpp
    ViewDescriptor.hpp
    FooterWidget.hpp
    Frame.hpp
    MiniSplitter.hpp
    Style.hpp
    Contexts.hpp
    Dialog.hpp

    Internal/MultiBar.hpp
    Internal/ViewRegistrar.hpp
    Internal/XmlState.hpp
    Internal/ContextManager.hpp

    Internal/HierarchyDumper.hpp
)

SET(RCC_FILES
    Resources/BlueSky.rcc
)

SET(HID_FILES
    Internal/ViewsMergerActions.hid
    Internal/MultiBarContainerWidgetActions.hid
)

QT_MOC(MOC_FILES ${HDR_FILES})
HIC(HIC_FILES ${HID_FILES})
QT_RCC(RCC_DATA QRC_FILES ${RCC_FILES})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_QT_LIBRARY(
    BlueSky SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${HIC_FILES}
    ${HID_FILES}
    ${QRC_FILES}
    ${RCC_FILES}
    ${RCC_DATA}
)

TARGET_LINK_LIBRARIES(
    BlueSky

    LINK_PRIVATE
        HeavenIcons
        HeavenActions
)

INSTALL_FRAMEWORK(BlueSky Runtime)
MSVC_SPLIT_SOURCES(BlueSky)

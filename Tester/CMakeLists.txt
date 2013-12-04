
QT_PREPARE(Core Gui)

SET(SRC_FILES
    Main.cpp
    Application.cpp
    MainWindow.cpp
    LoremIpsumView.cpp
)

SET(HDR_FILES
    Application.hpp
    MainWindow.hpp
    LoremIpsumView.hpp
)

SET(RCC_FILES
    Images/Images.rcc
)

SET(HID_FILES
    MainWindowActions.hid
)

QT_MOC(MOC_FILES ${HDR_FILES})
QT_RCC(QRC_FILES RES_FILES ${RCC_FILES})
HIC(HIC_FILES ${HID_FILES})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_QT_EXECUTABLE(
    SkyTest

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${QRC_FILES}
    ${RES_FILES}
    ${RCC_FILES}
    ${HID_FILES}
    ${HIC_FILES}
)

TARGET_LINK_LIBRARIES(
    SkyTest

    LINK_PRIVATE
        BlueSky
        HeavenIcons
        HeavenActions
)


PROJECT( HEAVEN_TESTER )

INCLUDE_DIRECTORIES(
    ${HEAVEN_TESTER_BINARY_DIR}
)

QT_PREPARE( Core Gui Widgets )

SET( SRC_FILES
    main.cpp
    MainWindow.cpp
)

SET( HDR_FILES
    MainWindow.hpp
)

SET( HID_FILES
    MainWindow.hid
)

QT_MOC( MOC_FILES ${HDR_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_QT_EXECUTABLE(
    HeavenTester WIN32

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${HIC_FILES}
)

TARGET_LINK_LIBRARIES(
    HeavenTester

    Heaven
)

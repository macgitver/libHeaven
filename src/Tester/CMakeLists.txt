
PROJECT( HEAVEN_TESTER )

QT_PREPARE( Core Gui Widgets )

SET( SRC_FILES
    main.cpp
    MainWindow.cpp
)

SET( HDR_FILES
    MainWindow.hpp
)

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_EXECUTABLE(
    HeavenTester WIN32

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    HeavenTester

    Heaven
)

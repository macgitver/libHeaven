
QT_PREPARE(Core Gui Xml)

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET(SRC_FILES
    ColorManager.cpp
    ColorSchema.cpp
    ColorSet.cpp
    ColorSchemaEditor.cpp
)

SET(PRI_HDR_FILES
    ColorSchemaPrivate.hpp
    ColorManagerPrivate.hpp
    ColorSet.hpp
)

SET(PUB_HDR_FILES
    ColorManager.hpp
    ColorSchema.hpp
    ColorSchemaEditor.hpp
)

SET( UI_FILES
    ColorSchemaEditor.ui
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
QT_RCC( RCC_DATA QRC_FILES ${RCC_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_QT_LIBRARY(
    HeavenColors SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${HIC_FILES} ${HID_FILES}
    ${QRC_FILES}
    ${RCC_DATA}
    ${DOX_FILES}
)

#TARGET_LINK_LIBRARIES(
#    HeavenColors
#
#    LINK_PRIVATE
#        HeavenActions
#)

INSTALL_FRAMEWORK(HeavenColors Runtime)

MSVC_SPLIT_SOURCES(Heaven)

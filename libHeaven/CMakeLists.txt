
PROJECT( MGV_HEAVEN )

QT_PREPARE( Core Gui Xml Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_HEAVEN_SOURCE_DIR}
    ${MGV_HEAVEN_BINARY_DIR}
)

SET(SRC_FILES
    App/Dialog.cpp
    Widgets/TabBar.cpp
    Widgets/TabWidget.cpp
)

SET(PUB_HDR_FILES
    HeavenApi.hpp
    App/Dialog.hpp
)

SET(PRI_HDR_FILES
#    HeavenPrivate.hpp // --> Contexts
    Widgets/TabWidget.hpp   # --> These are actually still just kept for reference...
    Widgets/TabBar.hpp
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
        BlueSky         # For Heaven::Dialog
        HeavenActions   # For ??? :)
)

INSTALL_FRAMEWORK( Heaven Runtime )

MSVC_SPLIT_SOURCES( Heaven )

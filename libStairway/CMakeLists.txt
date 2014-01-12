
QT_PREPARE(Core Gui Widgets)

SET(SRC_FILES
    StairwayToHeavenTool.cpp
)

SET(HDR_FILES
    StairwayToHeavenTool.hpp
)

QT_MOC(MOC_FILES ${HDR_FILES})

ADD_QT_LIBRARY(
    Stairway SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)


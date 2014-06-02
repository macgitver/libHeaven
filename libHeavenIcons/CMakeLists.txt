
QT_PREPARE(Core Gui Svg)

SET(SRC_FILES
    Icon.cpp
    IconManager.cpp
    IconDefaultProvider.cpp
    IconProvider.cpp
    IconRef.cpp
)

SET(HDR_PUB_FILES
    libHeavenIconsAPI.hpp

    Icon.hpp
    IconManager.hpp
    IconDefaultProvider.hpp
    IconProvider.hpp
    IconRef.hpp
)

SET(HDR_PRI_FILES
    IconPrivate.hpp
    IconManagerPrivate.hpp
    IconDefaultProviderPrivate.hpp
)

SET(HDR_FILES ${HDR_PUB_FILES} ${HDR_PRI_FILES})

QT_MOC(MOC_FILES ${HDR_FILES})

ADD_QT_LIBRARY(
    HeavenIcons SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

RAD_SPLIT_SOURCE_TREE(HeavenIcons)

RAD_SET_TARGET_VERSION(HeavenIcons HEAVEN)
RAD_INSTALL_LIBRARY(Heaven HeavenIcons)
RAD_INSTALL_HEADERS(libHeavenIcons ${HDR_PUB_FILES})

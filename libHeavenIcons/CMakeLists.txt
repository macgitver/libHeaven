
QT_PREPARE(Core Gui)

SET(SRC_FILES
    Icon.cpp
    IconManager.cpp
    IconDefaultProvider.cpp
    IconProvider.cpp
    IconRef.cpp
)

SET(HDR_FILES
    libHeavenIconsAPI.hpp

    Icon.hpp
    IconManager.hpp
    IconDefaultProvider.hpp
    IconProvider.hpp
    IconRef.hpp

    IconPrivate.hpp
    IconManagerPrivate.hpp
    IconDefaultProviderPrivate.hpp
)

QT_MOC(MOC_FILES ${HDR_FILES})

ADD_QT_LIBRARY(
    HeavenIcons SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)


CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(Heaven)

FIND_PACKAGE(RAD-Tools REQUIRED)

RAD_INIT(
    FEATURES
        QtMacros
        DoxygenTarget
        SplitSources
        Installation
)

RAD_DEFINE_VERSION(HEAVEN 0 0 1)

SET( libHeaven_includes
    ${CMAKE_CURRENT_LIST_DIR}
    CACHE INTERNAL "libHeaven include directories"
)

INCLUDE_DIRECTORIES(
    ${libHeaven_includes}
)

ADD_SUBDIRECTORY(hic)
ADD_SUBDIRECTORY(libHeavenIcons)
ADD_SUBDIRECTORY(libHeavenActions)
ADD_SUBDIRECTORY(libHeavenColors)
ADD_SUBDIRECTORY(libBlueSky)
#ADD_SUBDIRECTORY(libStairway)
#ADD_SUBDIRECTORY(Tester)

RAD_CREATE_PACKAGE(
    Heaven

    TEMPLATE_DIR
        cmake

    TARGETS
        hic
        BlueSky
        HeavenIcons
        HeavenActions
        HeavenColors
)

RAD_ADD_DOXYGEN_TARGET(
    libHeaven
    ${libHeaven_SOURCE_DIR}/doc

    ${MGV_HEAVEN_SOURCE_DIR}
    ${libHeaven_SOURCE_DIR}/doc/pages
)

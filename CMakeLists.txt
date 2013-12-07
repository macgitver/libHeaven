
PROJECT( libHeaven )

SET( libHeaven_includes
    ${CMAKE_CURRENT_LIST_DIR}
    CACHE INTERNAL "libHeaven include directories"
)

INCLUDE_DIRECTORIES(
    ${libHeaven_includes}
)

INCLUDE(cmake/hic.cmake)

ADD_SUBDIRECTORY(hic)
ADD_SUBDIRECTORY(libHeavenIcons)
ADD_SUBDIRECTORY(libHeavenActions)
ADD_SUBDIRECTORY(libHeavenColors)
ADD_SUBDIRECTORY(libBlueSky)
ADD_SUBDIRECTORY(Tester)

ADD_DOXYGEN_TARGET(
    libHeaven
    ${libHeaven_SOURCE_DIR}/doc

    ${MGV_HEAVEN_SOURCE_DIR}
    ${libHeaven_SOURCE_DIR}/doc/pages
)

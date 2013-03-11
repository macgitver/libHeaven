
PROJECT( libHeaven )

SET( libHeaven_includes
    ${CMAKE_CURRENT_LIST_DIR}/src
    CACHE INTERNAL "libHeaven include directories"
)

ADD_SUBDIRECTORY( src )

ADD_DOXYGEN_TARGET(
    libHeaven
    ${libHeaven_SOURCE_DIR}/doc

    ${MGV_HEAVEN_SOURCE_DIR}
    ${libHeaven_SOURCE_DIR}/doc/pages
)

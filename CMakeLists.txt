
PROJECT( libHeaven )

SET( libHeaven_includes
    ${CMAKE_CURRENT_LIST_DIR}/src
    CACHE INTERNAL "libHeaven include directories"
)

ADD_SUBDIRECTORY( src )


PROJECT( libHeaven )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )

INCLUDE( cmake/Utils.cmake )        # Tools for cmake
SET_OUTPUT_PATHS()
INCLUDE( cmake/QtMacros.cmake )     # Our own set of Qt Macros
INCLUDE( cmake/hic.cmake )          # Our HI-Compiler

IF( UNIX )
    GCC_ADD_FLAG( "-Wall" )
    GCC_ADD_FLAG( "-fvisibility=hidden" )
ENDIF()

ADD_SUBDIRECTORY( src )

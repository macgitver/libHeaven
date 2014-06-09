
QT_PREPARE( Core -Gui )

SET( SRC_FILES
    main.cpp
    HICObject.cpp
    HICProperty.cpp
    HIDLexer.cpp
    HIDParser.cpp
    HIDToken.cpp
    HIGeneratorBase.cpp
    HIGenHeader.cpp
    HIGenSource.cpp
)

SET( HDR_FILES
    HICObject.h
    HICProperty.h
    HIDLexer.h
    HIDToken.h
    HIDParser.h
    HIGeneratorBase.h
    HIGenHeader.h
    HIGenSource.h
)

ADD_QT_EXECUTABLE(
    hic

    ${SRC_FILES}
    ${HDR_FILES}

    # Not compilable, but shall show up in IDE
    HIDGrammar.md
)

# We're including hic.cmake from here, since our superprojects will simply
# skip libHeaven's root CMakeLists.txt file. Including hic.cmake doesn't make
# any sence before we scheduled a compilation of hic, anyway.
INCLUDE( ../cmake/hic.cmake )
SET(HIC_TOOL hic CACHE STRING "The hic tool")

INSTALL(
    TARGETS
        hic
    EXPORT
        HeavenTargets
    RUNTIME DESTINATION
        "${RAD_INSTALL_BIN_DIR}"
    COMPONENT
        Tools
)

INSTALL(
    FILES
        "${CMAKE_CURRENT_LIST_DIR}/../cmake/hic.cmake"
    DESTINATION
        "${RAD_INSTALL_CMAKE_DIR}/Heaven"
    COMPONENT
        DevelopmentFiles
)

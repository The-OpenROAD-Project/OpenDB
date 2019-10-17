find_package(BISON REQUIRED)
BISON_TARGET(LEFParser 
    ${CMAKE_CURRENT_SOURCE_DIR}/lef/lef.y 
    ${CMAKE_CURRENT_BINARY_DIR}/lef_parser.cpp
)

add_library(lef
    ${BISON_LEFParser_OUTPUTS}
    lef/FEF_malloc.cpp 
    lef/FEF_stringhash.cpp 
    lef/crypt.cpp 
    lef/lef_keywords.cpp 
    lef/lefiArray.cpp 
    lef/lefiCrossTalk.cpp 
    lef/lefiDebug.cpp 
    lef/lefiLayer.cpp 
    lef/lefiMacro.cpp 
    lef/lefiMisc.cpp 
    lef/lefiNonDefault.cpp 
    lef/lefiProp.cpp 
    lef/lefiPropType.cpp 
    lef/lefiTBExt.cpp 
    lef/lefiUnits.cpp 
    lef/lefiVia.cpp 
    lef/lefiViaRule.cpp 
    lef/lefrReader.cpp 
    lef/lefwWriter.cpp 
    lef/lefwWriterCalls.cpp 
)

target_include_directories(lef
    PUBLIC
        $<INSTALL_INTERFACE:include/cdslef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/cdslef56>
        ${PROJECT_SOURCE_DIR}/src/cdslef56/lef
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        .
)


BISON_TARGET(CLEFParser 
    ${CMAKE_CURRENT_SOURCE_DIR}/clef/lef.y 
    ${CMAKE_CURRENT_BINARY_DIR}/lef.tab.c
)
add_library(clef
    ${BISON_CLEFParser_OUTPUTS}
    clef/FEF_malloc.c 
    clef/FEF_stringhash.c 
    clef/crypt.c 
    clef/lef_keywords.c 
    clef/lefiArray.c 
    clef/lefiCrossTalk.c 
    clef/lefiDebug.c 
    clef/lefiLayer.c 
    clef/lefiMacro.c 
    clef/lefiMisc.c 
    clef/lefiNonDefault.c 
    clef/lefiProp.c 
    clef/lefiPropType.c 
    clef/lefiTBExt.c 
    clef/lefiUnits.c 
    clef/lefiVia.c 
    clef/lefiViaRule.c 
    clef/lefrReader.c 
    clef/lefwWriter.c 
    clef/lefwWriterCalls.c
)

target_include_directories(clef
    PUBLIC
        $<INSTALL_INTERFACE:include/cdslef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/cdslef56>
        ${PROJECT_SOURCE_DIR}/src/cdslef56/clef
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        .
)

add_library(clefzlib
    clefzlib/lefzlib.c 
)

target_include_directories(clefzlib
    PUBLIC
        $<INSTALL_INTERFACE:include/cdslef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/cdslef56>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/cdslef56/clefzlib
)

add_library(lefzlib
    lefzlib/lefzlib.cpp
)

target_include_directories(lefzlib
    PUBLIC
        $<INSTALL_INTERFACE:include/cdslef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/cdslef56>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/cdslef56/lefzlib
)

add_executable(lefrw
    lefrw/lefrw.cpp
)

target_include_directories(lefrw
    PUBLIC
        $<INSTALL_INTERFACE:include/cdslef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/cdslef56>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/cdslef56/lefrw
        ${PROJECT_SOURCE_DIR}/src/cdslef56
)

target_link_libraries(lefrw
    PUBLIC
        lef
)

add_executable(lefwrite
    lefwrite/lefwrite.cpp
)

target_include_directories(lefwrite
    PUBLIC
        $<INSTALL_INTERFACE:include/cdslef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/cdslef56>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/cdslef56
)

target_link_libraries(lefwrite
    PUBLIC
        lef
)
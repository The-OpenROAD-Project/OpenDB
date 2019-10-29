find_package(BISON REQUIRED)
BISON_TARGET(LEFParser 
    ${CMAKE_CURRENT_SOURCE_DIR}/lef/lef.y 
    ${CMAKE_CURRENT_BINARY_DIR}/lef_parser.cpp
)

add_library(opendblef
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

target_include_directories(opendblef
    PUBLIC
        $<INSTALL_INTERFACE:include/lef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/lef56>
        ${PROJECT_SOURCE_DIR}/src/lef56/lef
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        .
)

set_property(TARGET opendblef PROPERTY POSITION_INDEPENDENT_CODE ON)


BISON_TARGET(CLEFParser 
    ${CMAKE_CURRENT_SOURCE_DIR}/clef/lef.y 
    ${CMAKE_CURRENT_BINARY_DIR}/lef.tab.c
)
add_library(opendbclef
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

target_include_directories(opendbclef
    PUBLIC
        $<INSTALL_INTERFACE:include/lef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/lef56>
        ${PROJECT_SOURCE_DIR}/src/lef56/clef
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        .
)

add_library(opendbclefzlib
    clefzlib/lefzlib.c 
)

target_include_directories(opendbclefzlib
    PUBLIC
        $<INSTALL_INTERFACE:include/lef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/lef56>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/lef56/clefzlib
)

add_library(opendblefzlib
    lefzlib/lefzlib.cpp
)

target_include_directories(opendblefzlib
    PUBLIC
        $<INSTALL_INTERFACE:include/lef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/lef56>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/lef56/lefzlib
)

add_library(opendblefrw
    lefrw/lefrw.cpp
)

target_include_directories(opendblefrw
    PUBLIC
        $<INSTALL_INTERFACE:include/lef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/lef56>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/lef56/lefrw
        ${PROJECT_SOURCE_DIR}/src/lef56
)

target_link_libraries(opendblefrw
    PUBLIC
        opendblef
)

set_property(TARGET opendblefrw PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(opendblefwrite
    lefwrite/lefwrite.cpp
)

target_include_directories(opendblefwrite
    PUBLIC
        $<INSTALL_INTERFACE:include/lef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/lef56>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/lef56
)

target_link_libraries(opendblefwrite
    PUBLIC
        opendblef
)

add_library(opendblefdiff
    lefdiff/diffLefRW.cpp
    lefdiff/differLef.cpp
    lefdiff/datestamp.cpp
)

target_include_directories(opendblefdiff
    PUBLIC
        $<INSTALL_INTERFACE:include/lef56>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/lef56>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/lef56/lefdiff
)

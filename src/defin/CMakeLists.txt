FIND_PACKAGE(BISON REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)


set(DEF_SCANNER ${PROJECT_SOURCE_DIR}/src/defin/def_scanner.cpp)
set(DEF_PARSER ${PROJECT_SOURCE_DIR}/src/defin/def_parser.cpp)

set(YACC_PREFIX def)
set(LEX_PREFIX def)

add_library(defin
    definNet.cpp 
    definSNet.cpp 
    definComponent.cpp 
    definVia.cpp 
    definPin.cpp 
    definBlockage.cpp 
    definFill.cpp 
    definTracks.cpp 
    definGCell.cpp 
    definRow.cpp 
    definRegion.cpp 
    definNonDefaultRule.cpp 
    definReader.cpp 
    definBase.cpp 
    ${DEF_PARSER}
    ${DEF_SCANNER}
    create_box.cpp 
    defin.cpp 
    definPolygon.cpp 
    definPropDefs.cpp 
    definPinProps.cpp 
)

target_include_directories(defin
    PUBLIC
        $<INSTALL_INTERFACE:include/opendb>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/opendb>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/defin
)

target_compile_features(defin PRIVATE cxx_auto_type)
target_compile_options(defin PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)
set_property(TARGET opendb PROPERTY POSITION_INDEPENDENT_CODE ON)


add_custom_command(
    OUTPUT ${DEF_PARSER}
    COMMAND ${BISON_EXECUTABLE}
            -v  -pdef -d
            --output=${DEF_PARSER}
            ${PROJECT_SOURCE_DIR}/src/defin/def.ypp
    COMMENT "Generating parser.cpp"
)

add_custom_command(
    OUTPUT ${DEF_SCANNER}
    COMMAND ${FLEX_EXECUTABLE}
            -B -Pdef -F -v
            --outfile=${DEF_SCANNER}
            ${PROJECT_SOURCE_DIR}/src/defin/def.l
    COMMENT "Generating scanner.cpp"
    DEPENDS ${DEF_PARSER}
)
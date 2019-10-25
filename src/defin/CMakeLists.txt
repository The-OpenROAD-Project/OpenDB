FIND_PACKAGE(BISON REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)

set(DEF_SCANNER_BASE ${PROJECT_SOURCE_DIR}/src/defin/def_scanner)
set(DEF_SCANNER ${DEF_SCANNER_BASE}.cpp)
set(DEF_PARSER_BASE ${PROJECT_SOURCE_DIR}/src/defin/def_parser)
set(DEF_PARSER ${DEF_PARSER_BASE}.cpp)
set(DEF_PARSER_HEADER ${DEF_PARSER_BASE}.hpp)
set(DEF_PARSER_OUTPUT_FILE ${DEF_PARSER_BASE}.output)

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

target_link_libraries(defin
    PUBLIC
        zutil
)

target_compile_features(defin PRIVATE cxx_auto_type)
target_compile_options(defin PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

set_property(TARGET defin PROPERTY POSITION_INDEPENDENT_CODE ON)

add_custom_command(
    OUTPUT ${DEF_PARSER}
    COMMAND ${BISON_EXECUTABLE}
            -v  -p${YACC_PREFIX} -d
            --output=${DEF_PARSER}
            ${PROJECT_SOURCE_DIR}/src/defin/def.ypp
    COMMENT "Generating parser.cpp"
)

add_custom_command(
    OUTPUT ${DEF_SCANNER}
    COMMAND ${FLEX_EXECUTABLE}
            -B -P${LEX_PREFIX} -F -v
            --outfile=${DEF_SCANNER}
            ${PROJECT_SOURCE_DIR}/src/defin/def.l
    COMMENT "Generating scanner.cpp"
    DEPENDS ${DEF_PARSER}
)
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES 
        ${DEF_SCANNER}
        ${DEF_PARSER}
        ${DEF_PARSER_HEADER}
        ${DEF_PARSER_OUTPUT_FILE}
)
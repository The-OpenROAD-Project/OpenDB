FIND_PACKAGE(BISON REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)


set(LIBERTY_SCANNER_BASE ${PROJECT_SOURCE_DIR}/src/sylib/token)
set(LIBERTY_SCANNER ${LIBERTY_SCANNER_BASE}.cpp)
set(LIBERTY_PARSER_BASE ${PROJECT_SOURCE_DIR}/src/sylib/liberty_parser)
set(LIBERTY_PARSER ${LIBERTY_PARSER_BASE}.cpp)
set(LIBERTY_PARSER_HEADER ${LIBERTY_PARSER_BASE}.hpp)
set(LIBERTY_PARSER_OUTPUT_FILE ${LIBERTY_PARSER_BASE}.output)

set(YACC_PREFIX liberty_parser_)

add_library(sylib
    sy_lib.cpp
    sy_func.cpp
    sy_pi.cpp
    ${LIBERTY_PARSER}
    ${LIBERTY_SCANNER}
    liberty_parser.cpp
    liberty_front_lex.cpp
    token.cpp
    sy_palloc.cpp
    attr_lookup.cpp
    group_lookup.cpp
)

target_include_directories(sylib
    PUBLIC
        $<INSTALL_INTERFACE:include/opendb>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/opendb>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/sylib
)

target_compile_features(sylib PRIVATE cxx_auto_type)
target_compile_options(sylib PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)


add_custom_command(
    OUTPUT ${LIBERTY_PARSER}
    COMMAND ${BISON_EXECUTABLE}
            -v  -p${YACC_PREFIX} -d
            --output=${LIBERTY_PARSER}
            ${PROJECT_SOURCE_DIR}/src/sylib/liberty_parser.y
    COMMENT "Generating parser.cpp"
)

add_custom_command(
    OUTPUT ${LIBERTY_SCANNER}
    COMMAND ${FLEX_EXECUTABLE}
            -B -F -v
            ${PROJECT_SOURCE_DIR}/src/sylib/token.l
    COMMAND mv lex.yy.c ${LIBERTY_SCANNER}
    COMMENT "Generating scanner.cpp"
    DEPENDS ${LIBERTY_PARSER}
)


set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES 
        ${LIBERTY_SCANNER}
        ${LIBERTY_PARSER}
        ${LIBERTY_PARSER_HEADER}
        ${LIBERTY_PARSER_OUTPUT_FILE}
)
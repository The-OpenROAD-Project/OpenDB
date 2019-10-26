add_executable(opendbtcl
    ${PROJECT_SOURCE_DIR}/src/swig/tcl/main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/opendb_wrap.cpp
)

target_compile_features(opendbtcl PRIVATE cxx_auto_type)
target_compile_options(opendbtcl PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

target_include_directories(opendbtcl
    PUBLIC
        $<INSTALL_INTERFACE:include/opendb>
        ${PROJECT_SOURCE_DIR}/include/opendb
        ${PROJECT_SOURCE_DIR}/include/lef56
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/swig/tcl
)

target_link_libraries(opendbtcl
    PUBLIC
        opendb
        tcl
)

# enable swig compile
set (OPENDB_SWIG_FILES
    ${PROJECT_SOURCE_DIR}/src/swig/tcl/opendbtcl.i
    ${PROJECT_SOURCE_DIR}/src/swig/tcl/dbtypes.i
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/opendb_wrap.cpp
  COMMAND ${SWIG_EXECUTABLE}  -debug-tmsearch -debug-tmused -tcl8 -c++ -Iinclude/opendb -o ${CMAKE_CURRENT_BINARY_DIR}/opendb_wrap.cpp ${PROJECT_SOURCE_DIR}/src/swig/tcl/opendbtcl.i
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  DEPENDS ${OPENDB_SWIG_FILES}
)
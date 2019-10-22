add_executable(opendbtcl
    ${PROJECT_SOURCE_DIR}/src/swig/tcl/main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/opendb_wrap.cpp
)

target_compile_features(opendbtcl PRIVATE cxx_auto_type)
target_compile_options(opendbtcl PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

target_include_directories(opendbtcl
    PUBLIC
        $<INSTALL_INTERFACE:include/opendb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/opendb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lef56>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/opendb
        ${CMAKE_CURRENT_SOURCE_DIR}/src/swig/tcl
)

target_link_libraries(opendbtcl
    PUBLIC
        tcl
    PRIVATE
        opendb 
        zutil 
        zlib 
        tm
        lef
        lefrw
        defin
        defout
        lefin
        lefout
        zutil
)

# enable swig compile
set (OPENDB_SWIG_FILES
    ${PROJECT_SOURCE_DIR}/src/swig/tcl/opendbtcl.i
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/opendb_wrap.cpp
  COMMAND ${SWIG_EXECUTABLE} -tcl8 -c++ -Iinclude/opendb -o ${CMAKE_CURRENT_BINARY_DIR}/opendb_wrap.cpp ${PROJECT_SOURCE_DIR}/src/swig/tcl/opendbtcl.i
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  DEPENDS ${OPENDB_SWIG_FILES}
)
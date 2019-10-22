############################################################################
########################### Project Setup ##################################
############################################################################

# Setting CMake version properly to ensure correct policies behavior
cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Setting up OpenDB project
project(OpenDB VERSION 0.0.1
                DESCRIPTION "Database and Tool Framework for EDA"
                LANGUAGES C CXX)

# Configure build version
configure_file (
    "${PROJECT_SOURCE_DIR}/include/opendb/version.hh.in"
    "${PROJECT_BINARY_DIR}/include/opendb/version.hh"
)

############################################################################
################################# Targets ##################################
############################################################################

# db
add_subdirectory(src/db)

# defin
add_subdirectory(src/defin)

# defout
add_subdirectory(src/defout)

# lef56
add_subdirectory(src/lef56)

# lefin
add_subdirectory(src/lefin)

# lefout
add_subdirectory(src/lefout)

# sylib
add_subdirectory(src/sylib)

# zutil
add_subdirectory(src/zutil)

# zlib
add_subdirectory(src/zlib)

# tm
add_subdirectory(src/tm)

# add_library(sss INTERFACE)
# target_link_libraries(sss INTERFACE db defin defout)


# Create a dummy cpp to generate the combined library
set(OPENDB_LIB_CPP ${CMAKE_CURRENT_BINARY_DIR}/opendb.cpp)
file(WRITE ${OPENDB_LIB_CPP} "")

# Link all to one library!
add_library(opendb ${OPENDB_LIB_CPP})
target_link_libraries(opendb
    db
    defin
    lefin
    lefout
    lef
    defout
    zutil
    zlib
    tm
)

############################################################################
################################# SWIG #####################################
############################################################################

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

option(BUILD_PYTHON "Build Python SWIG module" ON)
if(BUILD_PYTHON)
    add_subdirectory(src/swig/python)
endif()

option(BUILD_TCL "Build TCL SWIG module" ON)
if(BUILD_TCL)
    add_subdirectory(src/swig/tcl)
endif()
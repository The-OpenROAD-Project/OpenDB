############################################################################
########################### Project Setup ##################################
############################################################################

# Setting CMake version properly to ensure correct policies behavior
cmake_minimum_required(VERSION 3.1...3.15)
set(CMAKE_CXX_STANDARD 11)
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

find_package(TCL)
include_directories(${TCL_INCLUDE_PATH})

############################################################################
################################# Targets ##################################
############################################################################

# db
if(NOT TARGET opendb)
    add_subdirectory(src/db)
endif()

# defin
if(NOT TARGET defin)
    add_subdirectory(src/defin)
endif()

# defout
if(NOT TARGET defout)
    add_subdirectory(src/defout)
endif()

# lef
if(NOT TARGET lef)
    add_subdirectory(src/lef)
endif()

# lefin
if(NOT TARGET lefin)
    add_subdirectory(src/lefin)
endif()

# lefout
if(NOT TARGET lefout)
    add_subdirectory(src/lefout)
endif()

# zutil
if(NOT TARGET zutil)
    add_subdirectory(src/zutil)
endif()

# zlib
if(NOT TARGET zlib)
    add_subdirectory(src/zlib)
endif()

# tm
if(NOT TARGET tm)
    add_subdirectory(src/tm)
endif()

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

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
  LANGUAGES CXX)

# Configure build version
configure_file (
    "${PROJECT_SOURCE_DIR}/include/opendb/version.hh.in"
    "${PROJECT_BINARY_DIR}/include/opendb/version.hh"
)

find_package(TCL)

find_package(Boost)

############################################################################
################################# Targets ##################################
############################################################################

add_subdirectory(src/db)
add_subdirectory(src/defin)
add_subdirectory(src/defout)
add_subdirectory(src/lef)
add_subdirectory(src/lefin)
add_subdirectory(src/lefout)
add_subdirectory(src/def)
add_subdirectory(src/zutil)
add_subdirectory(src/zlib)
add_subdirectory(src/tm)

############################################################################
################################# SWIG #####################################
############################################################################

set(SWIG_REQUIRED_VERSION "3.0")
find_package(SWIG REQUIRED)
if(${SWIG_VERSION} VERSION_LESS ${SWIG_REQUIRED_VERSION})
  message(FATAL_ERROR "SWIG version ${SWIG_VERSION} must be at least ${SWIG_REQUIRED_VERSION}")
endif()

include(${SWIG_USE_FILE})

option(BUILD_PYTHON "Build Python SWIG module" ON)
if(NOT OPENROAD_BUILD AND BUILD_PYTHON)
    add_subdirectory(src/swig/python)
endif()

option(BUILD_TCL "Build TCL SWIG module" ON)
if(NOT OPENROAD_BUILD AND BUILD_TCL)
    add_subdirectory(src/swig/tcl)
endif()

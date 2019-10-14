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
target_include_directories(db
    PUBLIC
        $<INSTALL_INTERFACE:include/opendb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/opendb>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/db
)

# defin
add_subdirectory(src/defin)
target_include_directories(defin
    PUBLIC
        $<INSTALL_INTERFACE:include/opendb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/opendb>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/defin
)

# defout
add_subdirectory(src/defout)
target_include_directories(defout
    PUBLIC
        $<INSTALL_INTERFACE:include/opendb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/opendb>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/defout
)

# lefin
add_subdirectory(src/cdslef56)


#add_subdirectory(src/lefin)

# opendb
add_library(opendb
    src/main.cpp
)


target_compile_features(opendb PRIVATE cxx_auto_type)
target_compile_options(opendb PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

target_include_directories(opendb
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${PROJECT_BINARY_DIR}/include
)


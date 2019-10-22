# Include python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

set_source_files_properties(${PROJECT_SOURCE_DIR}/src/swig/python/opendbpy.i 
    PROPERTIES CPLUSPLUS ON
)
include_directories(${PROJECT_SOURCE_DIR}/include/opendb)

# Add swig module
swig_add_library(opendbpy LANGUAGE python SOURCES ${PROJECT_SOURCE_DIR}/src/swig/python/opendbpy.i)
swig_link_libraries(opendbpy 
    opendb 
    zutil 
    zlib 
    tm
    defout
    lefout
    ${PYTHON_LIBRARIES}
)
set_property(TARGET opendbpy PROPERTY POSITION_INDEPENDENT_CODE ON)

# Files to install with Python
set(PYTHON_INSTALL_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/opendbpy.py
    ${CMAKE_CURRENT_BINARY_DIR}/_opendbpy.so
)

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

# Install target to call setup.py
add_custom_target(install-python
        DEPENDS _opendbpy
        COMMAND python ${SETUP_PY_OUT} install)
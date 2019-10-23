add_library(tm
    tm.cpp
    ZNamespace.cpp
)

target_include_directories(tm
    PUBLIC
        $<INSTALL_INTERFACE:include/opendb>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/opendb>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/tm
)

target_compile_features(tm PRIVATE cxx_auto_type)
target_compile_options(tm PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)
set_property(TARGET tm PROPERTY POSITION_INDEPENDENT_CODE ON)
add_library(zlib
    ZException.cpp
    adsRTreeCore.cpp
)

target_include_directories(zlib
    PUBLIC
        $<INSTALL_INTERFACE:include/opendb>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/opendb>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/zlib
)

target_compile_features(zlib PRIVATE cxx_auto_type)
target_compile_options(zlib PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)
set_property(TARGET zlib PROPERTY POSITION_INDEPENDENT_CODE ON)
add_library(lefout
    lefout.cpp
)

target_include_directories(lefout
    PUBLIC
        $<INSTALL_INTERFACE:include/opendb>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/opendb>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/lefout
)

target_compile_features(lefin PRIVATE cxx_auto_type)
target_compile_options(lefin PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)
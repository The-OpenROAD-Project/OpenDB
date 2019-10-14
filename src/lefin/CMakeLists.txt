add_library(lefin
    lefin.cpp
    reader.cpp
)

target_compile_features(lefin PRIVATE cxx_auto_type)
target_compile_options(lefin PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)
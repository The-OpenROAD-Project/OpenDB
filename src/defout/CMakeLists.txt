add_library(defout
    defout.cpp
    defout_impl.cpp
)

target_compile_features(defout PRIVATE cxx_auto_type)
target_compile_options(defout PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)
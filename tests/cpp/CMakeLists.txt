link_libraries(opendb
        zutil 
        zlib 
        tm
        lef
        defin
        defout
        lefout
        lefin
        tcl
)
include_directories(${PROJECT_SOURCE_DIR}/include/opendb)
add_executable( TestCallBacks ${PROJECT_SOURCE_DIR}/tests/cpp/TestCallBacks.cpp )

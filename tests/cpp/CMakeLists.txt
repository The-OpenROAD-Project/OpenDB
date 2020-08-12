link_libraries(opendb
        zutil 
        zlib 
        tm
        lef
        defin
        defout
        lefout
        zutil
        lefin
        tcl
)
include_directories(${PROJECT_SOURCE_DIR}/include/opendb)
add_executable( test1 ${PROJECT_SOURCE_DIR}/tests/cpp/test.cpp )

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(TEST_LIBS
        opendb
        zutil 
        zlib 
        tm
        lef
        defin
        defout
        lefout
        lefin
        tcl
        Boost::boost
)

add_executable( TestCallBacks ${PROJECT_SOURCE_DIR}/tests/cpp/TestCallBacks.cpp )
add_executable( TestGeom ${PROJECT_SOURCE_DIR}/tests/cpp/TestGeom.cpp )

target_link_libraries(TestCallBacks ${TEST_LIBS})
target_link_libraries(TestGeom ${TEST_LIBS})

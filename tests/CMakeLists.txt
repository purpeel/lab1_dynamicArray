find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/inc)

add_executable(runTests
    testMain.cpp
    testArray.cpp
    testDouble.cpp
    testString.cpp
)

target_link_libraries(runTests
    ${GTEST_LIBRARIES}
    pthread
    lab1_library 
)

add_test(NAME unit_tests COMMAND runTests)
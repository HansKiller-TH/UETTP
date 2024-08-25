project(Google_tests)
add_subdirectory(lib)

#set(SOURCE_TEST_FILES
#        ${CMAKE_SOURCE_DIR}/vectorUtils/VectorUtils.cpp
#)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_SOURCE_DIR})
add_executable(Google_Tests_run TestExamDataManipulator.cpp TestVectorUtils.cpp
        ${SOURCE_TEST_FILES}
        TestExamDataPopulator.cpp
        TestExamDataAccdbParser.cpp
)
target_link_libraries(Google_Tests_run gtest gtest_main
        step_counting_hill_climbing_algorithm-lib)
if(WIN32)
    target_link_libraries(Google_Tests_run odbc32 comdlg32)
endif()
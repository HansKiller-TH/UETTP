project(Google_tests)
add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_SOURCE_DIR})
add_executable(Google_Tests_run TestExamDataManipulator.cpp TestVectorUtils.cpp
        TestExamDataPopulator.cpp
        TestExamDataAccdbParser.cpp
        ExamTTSolutionTest.cpp
        TestInitialSolution.cpp
        TestSCHC.cpp
)
target_link_libraries(Google_Tests_run gtest gtest_main
        step_counting_hill_climbing_algorithm-lib)
if(WIN32)
    target_link_libraries(Google_Tests_run odbc32 comdlg32)
endif()
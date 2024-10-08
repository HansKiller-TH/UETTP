cmake_minimum_required(VERSION 3.28)
project(step_counting_hill_climbing_algorithm)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/output_utils)

add_library(${PROJECT_NAME}-lib STATIC
        Algorithm/InitialSolution.cpp
        Algorithm/InitialSolution.h
        getUserInput.h
        file_utils/AccdbFileHandler.cpp
        file_utils/AccdbFileHandler.h
        Exceptions.h
        file_utils/TxtFileHandler.cpp
        file_utils/TxtFileHandler.h
        data_utils/ExamTTData.h
        data_utils/IExamDataParser.h
        data_utils/AbstractExamDataFileParser.h
        data_utils/ExamDataTxtParser.cpp
        data_utils/ExamDataTxtParser.h
        data_utils/ExamDataXmlParser.cpp
        data_utils/ExamDataXmlParser.h
        data_utils/ExamDataAccdbParser.cpp
        data_utils/ExamDataAccdbParser.h
        file_utils/tinyxml2/tinyxml2.cpp
        file_utils/tinyxml2/tinyxml2.h
        file_utils/nlohmann_json/json.hpp
        output_utils/screenOutput.cpp
        output_utils/screenOutput.h
        Algorithm/Evaluation.cpp
        Algorithm/Evaluation.h
        Algorithm/SCHC.cpp
        Algorithm/SCHC.h
        vectorUtils/VectorUtils.cpp
        vectorUtils/VectorUtils.h
        Algorithm/ExamTTSolutionManipulator.cpp
        Algorithm/ExamTTSolutionManipulator.h
        data_utils/ExamTTDataBuilder.cpp
        data_utils/ExamTTDataBuilder.h
        data_utils/ExamDataParserFactory.cpp
        data_utils/ExamDataParserFactory.h
        data_utils/ExamTTFileManager.cpp
        data_utils/ExamTTFileManager.h
        output_utils/CostLogger.cpp
        output_utils/CostLogger.h
        data_utils/ExamTTSolution.h
        output_utils/DateTimeProvider.cpp
        output_utils/DateTimeProvider.h
        output_utils/ExamDataLogger.cpp
        output_utils/ExamDataLogger.h
        output_utils/ILogger.h
        output_utils/ConvergenceLogger.cpp
        output_utils/ConvergenceLogger.h
        data_utils/ExamTTDataTMP.h
        data_utils/RoomType.h
        vectorUtils/PeriodChange.h
)

add_executable(step_counting_hill_climbing_algorithm main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} odbc32 comdlg32)
endif()

# uncomment if you want to use Google tests. The files from the Google tests repo must be in Google_tests/lib
add_subdirectory(Google_tests)
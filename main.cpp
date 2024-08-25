#include <iostream>
#include <future>
#include "getUserInput.h"
#include "Algorithm/SCHC.h"
#include "data_utils/ExamTTDataImporter.h"
#include "Algorithm/InitialSolution.h"
#include "screenOutput.h"
#include "ConvergenceLogger.h"
#include "ExamDataLogger.h"
#include "data_utils/ExamTTSolutionImporter.h"
#include "Algorithm/Evaluation.h"
#include "data_utils/ExamTTSolutionAccdbExporter.h"

int main() {
    // Number of maximum concurrent threads
    const int max_concurrent_threads = 15;
    // Number of total threads
    const int num_threads = 15;

    // Logger will store all solutions
    ExamDataLogger logger;
    // Vector to hold the futures
    std::vector<std::future<std::shared_ptr<ExamTTData>>> futures;

    std::cout << "Please select an *.accdb ExamTT file to import." << std::endl;
    // Open an Open-File-Dialog to get a file path from the user
    std::string filepath = openFileDialog();
    if (filepath.empty()) {
        std::cout << "File is already used by a different program or you entered an empty file path, please check and try again.\n";
        return 1;
    }
    // print filepath to console
    std::cout << filepath << std::endl;

    // From the file parse the data into an ExamTTData object
    auto examDataPtr = ExamTTDataImporter::import(filepath);
    if (!examDataPtr) {
        std::cout << "Failed to parse data, make sure the file " << filepath << " is not malformed or empty.\n";
        return 1;
    }

    // Create an ExamDataManipulator that offers methods to create or change a solution
    auto manipulator = std::make_shared<ExamDataManipulator>(examDataPtr);
    // Create an Initial solution object that uses the ExamDataManipulator to create an initial solution
    InitialSolution initialSolution(manipulator);
    initialSolution.random = true;
    auto examData = initialSolution.build();
    // The ExamTTData object now has an initial solution if any
    if (examData == nullptr) {
        std::cout << "Failed to build Initial Solution!" << std::endl;
        return 1;
    }

    // Launch multiple threads
    for (int i = 0; i < num_threads; ++i) {
        std::cout << std::to_string(i) << std::endl;
        auto examDataManipulator = std::make_shared<ExamDataManipulator>(std::make_shared<ExamTTData>(*examData));
        auto algo = std::make_shared<SCHC>(examDataManipulator);
        algo->stopTime = 0.0;
        algo->random = true;
        algo->fullCollisionCost = true;
        algo->schc_all = true;
        algo->schc_acp = false;
        algo->schc_imp = false;
        algo->counterLimit = 500;
        // Use std::async to run the function in a separate thread and get a future to its result
        futures.push_back(std::async(std::launch::async, [algo]() { return algo->run(); }));
        if ((i + 1) % max_concurrent_threads == 0 || i == num_threads - 1) {
            // Gather the results
            for (auto &fut: futures) {
                // Logger safes the result from the future to a vector
                auto result = fut.get();
                logger.add(result);
            }
            futures.clear();
        }
    }
    // Get the best solution from a multiset sorted by cost asc
    auto result = *logger.getData().begin();
    // Export the solution to the original filepath the ExamTTData was created from
    ExamTTSolutionAccdbExporter::exportExamTTSolution(result);
    logger.reset();

    return 0;
}

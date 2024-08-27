//
// Created by hansk on 12.07.2024.
//

#include "ConvergenceLogger.h"
#include "file_utils/TxtFileHandler.h"

void ConvergenceLogger::add(std::shared_ptr<ExamTTSolution> solution) {
    convergenceData.emplace_back(solution->costAndTimeLog);
}

std::vector<std::vector<std::pair<int, double>>> ConvergenceLogger::getData() {
    return convergenceData;
}

void ConvergenceLogger::writeToFile() {
    std::string name = filename + DateTimeProvider::getDateTimeNow() + "_conv";
    TxtFileHandler fh(name + ".py");
    fh.writeLine(name + " = [");
    for (auto vec = convergenceData.begin(); vec != convergenceData.end(); ++vec){
        fh.writeLine("[");
        for (auto el = vec->begin(); el != vec->end(); ++el){
            std::string str = "(" + std::to_string(el->first) + ", " + std::to_string(el->second) + ")";
            if (std::next(el) != vec->end())
                str += ","; // apparently python don't even care about trailing comma but we do care
            fh.writeLine(str);
        }
        std::string str = "]";
        if (std::next(vec) != convergenceData.end())
            str += ","; // apparently python don't even care about trailing comma but we do care
        fh.writeLine(str);
    }
    fh.writeLine("]");
}

void ConvergenceLogger::setFileName(const std::string &configFilename) {
    this->filename = configFilename;
}

void ConvergenceLogger::reset() {
    convergenceData.clear();
    filename = "";
}

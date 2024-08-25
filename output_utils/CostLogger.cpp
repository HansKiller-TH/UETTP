//
// Created by hansk on 01.07.2024.
//

#include <sstream>
#include "CostLogger.h"
#include "file_utils/TxtFileHandler.h"
#include "DateTimeProvider.h"

void CostLogger::add(std::shared_ptr<ExamTTData> examData) {
    costData.emplace_back(std::vector<int>{examData->cost, examData->costLimit, examData->costAbove});
    timeData.emplace_back(examData->runTime);
}

std::vector<std::vector<int>> CostLogger::getData() {
    return costData;
}

void CostLogger::setFileName(const std::string &configFilename) {
    this->filename = configFilename;
}

void CostLogger::writeToFile() {
    std::string name = filename + DateTimeProvider::getDateTimeNow() + "_limit";
    TxtFileHandler fh(name + ".py");
    fh.writeLine("costs = [");
    for (int i = 0; i < costData.size();++i) {
        std::string str = "[" + std::to_string(costData.at(i).at(0)) + ", " + std::to_string(costData.at(i).at(1)) + ", " + std::to_string(costData.at(i).at(2)) + "]";
        if(i!=costData.size()-1)
            str += ",";
        fh.writeLine(str);
    }
    fh.writeLine("]");
    fh.writeLine("times = [");
    for (int i = 0; i < costData.size();++i) {
        std::string str = std::to_string(timeData.at(i));
        if(i!=costData.size()-1)
            str += ",";
        fh.writeLine(str);
    }
    fh.writeLine("]");
}

void CostLogger::reset() {
    costData.clear();
    filename = "";
}

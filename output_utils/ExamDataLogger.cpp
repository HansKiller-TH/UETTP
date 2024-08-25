//
// Created by hansk on 10.07.2024.
//

#include "ExamDataLogger.h"
#include "Algorithm/Evaluation.h"
#include "file_utils/TxtFileHandler.h"

void ExamDataLogger::add(std::shared_ptr<ExamTTData> examData) {
    solutions.insert(examData);
}

std::multiset<std::shared_ptr<ExamTTData>, ExamDataLogger::CompareExamDataCost> ExamDataLogger::getData() {
    return solutions;
}

void ExamDataLogger::setFileName(const std::string &configFilename) {
    this->filename = configFilename;
}

void ExamDataLogger::writeToFileBest() {
    if(solutions.empty()){
        std::cout << "no data to write" << std::endl;
        return;
    }
    auto examData = *solutions.begin();
    std::string name = examData->configuration + "_" + DateTimeProvider::getDateTimeNow();
    TxtFileHandler fh(name + ".py");
    fh.writeLine("cost = " + std::to_string(Evaluation::calculateCost(*examData)));
    fh.writeLine("alt_cost = " + std::to_string(Evaluation::calculateAltCost(*examData)));
    fh.writeLine("spread_cost = " + std::to_string(Evaluation::spreadCost(*examData)));
    fh.writeLine("spread_cost_limit = " + std::to_string(Evaluation::spreadCostLimit(*examData)));
    fh.writeLine("spread_cost_above = " + std::to_string(Evaluation::spreadCostBeyond(*examData)));
    fh.writeLine("saturday_room_cost = " + std::to_string(Evaluation::roomCost(*examData)));
    fh.writeLine("last_week_cost = " + std::to_string(Evaluation::periodCost(*examData)));

    auto result = Evaluation::spreadPerStudent(*solutions.begin());
    fh.writeLine("sameDay = [");
    for (auto vec = result.first.begin(); vec != result.first.end(); ++vec) {
        std::string str = "[";
        for (auto el = (*vec).begin(); el != (*vec).end(); ++el) {
            str += std::to_string(*el);
            if (std::next(el) != (*vec).end())
                str += ", "; // apparently python don't even care about trailing comma but we do care
        }
        str += "]";
        if (std::next(vec) != result.first.end())
            str += ", "; // apparently python don't even care about trailing comma but we do care
        fh.writeLine(str);
    }
    fh.writeLine("]");
    fh.writeLine("consecutiveDays = [");
    for (auto vec = result.second.begin(); vec != result.second.end(); ++vec) {
        std::string str = "[";
        for (auto el = (*vec).begin(); el != (*vec).end(); ++el) {
            str += std::to_string(*el);
            if (std::next(el) != (*vec).end())
                str += ", "; // apparently python don't even care about trailing comma, but we do care
        }
        str += "]";
        if (std::next(vec) != result.second.end())
            str += ", "; // apparently python don't even care about trailing comma, but we do care
        fh.writeLine(str);
    }
    fh.writeLine("]");

    auto saturdays = Evaluation::roomsOnSaturday(*solutions.begin());
    std::string str = "saturdays = {";
    for (int el = 0; el < saturdays.size(); ++el) {
        if(saturdays.at(el)==0)
            continue;
        str += "'"+ (*solutions.begin())->roomName.at(el) +"': ";
        str += std::to_string(saturdays.at(el));
        if (el + 1 < saturdays.size())
            str += ", "; // apparently python don't even care about trailing comma, but we do care
    }
    str += "}";
    fh.writeLine(str);

    auto lastWeek = Evaluation::studentsLastWeek(*solutions.begin());
    str = "last_week = [";
    for (auto el = lastWeek.begin(); el != lastWeek.end(); ++el) {
        str += std::to_string(*el);
        if (std::next(el) != lastWeek.end())
            str += ", "; // apparently python don't even care about trailing comma, but we do care
    }
    str += "]";
    fh.writeLine(str);
}

void ExamDataLogger::writeToFileTimesCosts() {
    struct CompareExamDataTime {
        bool operator()(const std::shared_ptr<ExamTTData>& e1, const std::shared_ptr<ExamTTData>& e2) const {
            return e1->runTime < e2->runTime;
        }
    };
    std::multiset<std::shared_ptr<ExamTTData>,CompareExamDataTime> datas;
    datas.insert(solutions.begin(),solutions.end());
    std::string name = (*solutions.begin())->configuration + "runs" + std::to_string(runs);
    TxtFileHandler fh(name + ".py");
    fh.writeLine(name + " = [");
    for (auto &sol:datas) {
            std::string str = "(" + std::to_string(sol->cost) + ", " + std::to_string(sol->runTime) + ")";
                str += ",";
            fh.writeLine(str);
    }
    fh.writeLine("]");
}

void ExamDataLogger::reset() {
    solutions.clear();
    filename = "";
}

//
// Created by hansk on 09.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_TXTFILEHANDLER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_TXTFILEHANDLER_H


#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class TxtFileHandler {
public:
    explicit TxtFileHandler(const std::string& filename){
        std::cout << filename << std::endl;
        file.open(filename);
        if (!file)
            throw std::ios_base::failure("Error opening file.");
    }
    void writeLine(const std::string &line){
        if (!file)
            throw std::ios_base::failure("Error opening file for writing.");
        file << line << std::endl;
    }
    ~TxtFileHandler(){
        if (file.is_open())
            file.close();
    }
private:
    std::ofstream file;
};


#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_TXTFILEHANDLER_H

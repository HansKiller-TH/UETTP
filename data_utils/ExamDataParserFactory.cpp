//
// Created by hansk on 05.06.2024.
//

#include <filesystem>
#include <algorithm>
#include "ExamDataParserFactory.h"
#include "ExamDataXmlParser.h"
#include "ExamDataAccdbParser.h"
#include "ExamDataTxtParser.h"

std::unique_ptr<IExamDataParser> ExamDataParserFactory::createParser(const std::string &filepath) {
    std::string extension = getExtensionLowerWithoutDot(filepath);
    if (extension.empty())
        return {};
    if(extension == "xml")
        return std::make_unique<ExamDataXmlParser>(filepath);
    if (extension == "accdb")
        return std::make_unique<ExamDataAccdbParser>(filepath);
    /*if(extension == "txt")
        return std::make_unique<ExamDataTxtParser>(filepath);
        not implemented*/
    return{};
}

std::string ExamDataParserFactory::getExtensionLowerWithoutDot(const std::string &filepath) {
    std::filesystem::path p(filepath);
    std::string ext = p.extension().string();
    if (ext.empty())
        return {};
    // Remove the dot from the extension
    ext = ext.substr(1);
    // Make extension lowercase
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c) { return tolower(c); });
    return ext;
}

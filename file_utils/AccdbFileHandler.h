//
// Created by hansk on 02.05.2024.
//

#ifndef STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ACCDBFILEHANDLER_H
#define STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ACCDBFILEHANDLER_H
#include <iostream>
#include <utility>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include "Exceptions.h"
class AccdbFileHandler {
public:
    explicit AccdbFileHandler(std::string filepath) : FILE_PATH(std::move(filepath)){
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
        SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
        getConnection(hEnv, hDbc);
    }

    ~AccdbFileHandler(){
        if (hStmt) {
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            hStmt = NULL;
        }
        if (hDbc) {
            SQLDisconnect(hDbc);
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
            hDbc = NULL;
        }
        if (hEnv) {
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
            hEnv = NULL;
        }
    }

    std::vector<std::vector<std::string>> readMSAccessFile(const std::string &queryString);

    void writeMSAccessFile(const std::string &tableName,
                           const std::vector<std::vector<std::string>> &data);

    void clearTable(const std::string &tableName);
private:
private:
    SQLHENV hEnv = NULL;
    SQLHDBC hDbc = NULL;
    SQLHSTMT hStmt = NULL;
    void getConnection(SQLHENV &hEnv, SQLHDBC &hDbc);

    static void executeQuery(SQLHSTMT &hStmt, SQLHDBC &hDbc, const std::string &queryString);

    static std::vector<std::vector<std::string>> fetchData(SQLHSTMT &hStmt);

    static std::string generateInsertQuery(const std::string &tableName, const std::vector<std::string> &row);

    const std::string FILE_PATH;
};
#endif //STEP_COUNTING_HILL_CLIMBING_ALGORITHM_ACCDBFILEHANDLER_H

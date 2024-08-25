//
// ODBC documentation can be found here:
// https://learn.microsoft.com/en-us/sql/odbc/reference/syntax/odbc-api-reference?view=sql-server-ver16
// code examples for ODBC can be found here:
// https://learn.microsoft.com/en-us/sql/connect/odbc/cpp-code-example-app-connect-access-sql-db?view=sql-server-ver16
// https://learn.microsoft.com/en-us/previous-versions/office/developer/office-2007/cc811599(v=office.12)?redirectedfrom=MSDN#Y3869
//
#include <vector>
#include "AccdbFileHandler.h"
//TODO: Free resources in destructor and keep connection open during lifetime
std::vector<std::vector<std::string>> AccdbFileHandler::readMSAccessFile(const std::string &queryString){
    try {
        executeQuery(hStmt, hDbc, queryString);
        auto data = fetchData(hStmt);
        return data;
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        throw;
    }
}
void AccdbFileHandler::writeMSAccessFile(const std::string& tableName,
                                         const std::vector<std::vector<std::string>>& data) {

    try {
        for (const auto& row : data) {
            std::string query = generateInsertQuery(tableName, row);
            executeQuery(hStmt, hDbc, query);
        }
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        throw;
    }
}

void AccdbFileHandler::clearTable(const std::string &tableName) {
    executeQuery(hStmt,hDbc,"DELETE FROM " + tableName + ";");
}

void AccdbFileHandler::getConnection(SQLHENV& hEnv, SQLHDBC& hDbc) {
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) {
        throw std::runtime_error("Error allocating environment handle.");
    }
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        throw std::runtime_error("Error allocating database handle.");
    }
    SQLCHAR outConnectionString[1024];
    SQLSMALLINT outConnectionStringLen;
    std::string connString = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=" + FILE_PATH + ";";
    //DO NOT EVER MODIFY THE CONTENTS POINTED TO BY sql_connString
    SQLCHAR* sql_connString = reinterpret_cast<SQLCHAR*>(const_cast<char*>(connString.c_str()));
    if (SQLDriverConnect(hDbc, NULL, sql_connString,
                         SQL_NTS, outConnectionString,
                         sizeof(outConnectionString),
                         &outConnectionStringLen,
                         SQL_DRIVER_NOPROMPT
                         ) != SQL_SUCCESS) {
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        throw std::runtime_error("Error connecting to database.");
    }
}

void AccdbFileHandler::executeQuery(SQLHSTMT& hStmt, SQLHDBC& hDbc, const std::string &queryString) {
    SQLRETURN retcode;
    SQLCHAR sqlState[6], msg[SQL_MAX_MESSAGE_LENGTH];
    SQLINTEGER    errNum;
    SQLSMALLINT msgLen;
    SQLHENV hEnv = NULL;

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        throw std::runtime_error("Error allocating statement handle.");

    //DO NOT EVER MODIFY THE CONTENTS POINTED TO BY query
    SQLCHAR* query = reinterpret_cast<SQLCHAR*>(const_cast<char*>(queryString.c_str()));
    retcode = SQLExecDirect(hStmt, query, SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        while(SQLError(hEnv,
                       hDbc,
                       hStmt,
                       sqlState,
                       &errNum,
                       msg,
                       sizeof(msg),
                       &msgLen
                       ) == SQL_SUCCESS) {
            std::cout << "Error:\n"
                      << "\tSQLState: " << sqlState << "\n"
                      << "\tNative Error: " << errNum << "\n"
                      << "\tMessage: " << msg << std::endl;
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cerr << queryString << std::endl;
        throw std::runtime_error("Error executing query.");
    }
}

std::vector<std::vector<std::string>> AccdbFileHandler::fetchData(SQLHSTMT& hStmt) {
    SQLSMALLINT numColumns = 0;
    SQLNumResultCols(hStmt, &numColumns);

    std::vector<std::vector<std::string>> allRows;
/*    std::vector<std::string> columnNames;
    for (int i = 1; i <= numColumns; i++){
        SQLCHAR columnName[128];
        SQLSMALLINT columnNameLen;
        SQLDescribeCol(hStmt, i, columnName, sizeof(columnName), &columnNameLen, NULL, NULL, NULL, NULL);
        columnNames.emplace_back(reinterpret_cast<char*>(columnName));
    }
    allRows.push_back(columnNames);*/
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        std::vector<std::string> row;
        for(int i = 1; i <= numColumns; i++){
            SQLCHAR columnData[128];
            SQLGetData(hStmt, i, SQL_C_CHAR, columnData, sizeof(columnData), NULL);
            row.emplace_back(reinterpret_cast<char*>(columnData));
        }
        allRows.push_back(row);
    }
    return allRows;
}
std::string AccdbFileHandler::generateInsertQuery(const std::string& tableName, const std::vector<std::string>& row) {
    std::string query = "INSERT INTO " + tableName + " VALUES (";
    for (size_t i = 0; i < row.size(); ++i) {
        query += row[i];
        if (i!=row.size()-1)
            query += ", ";
    }
    query += ");";
    return query;
}
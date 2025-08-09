#include "sql_server.h"
#include "exception/sql_exception.h"
#include "postgres_db.h"
#include <fstream>
#include <iostream>
#include <libpq-fe.h>
#include <nlohmann/json.hpp>
#include <string>

// utilities
bool checkEmptyBaseSQL(PGconn *conn) {

  std::string sql = "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = 'public';";
  PGresult *result = PQexec(conn, sql.c_str());
  std::string error = "";
  try {
    if (!result) {
      error = PQerrorMessage(conn);
      throw exc::SQLEmptyBaseException(error);
    }
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
      error = PQresultErrorMessage(result);
      PQclear(result);
      throw exc::SQLEmptyBaseException(error);
    }

    long count = std::stol(PQgetvalue(result, 0, 0));

    PQclear(result);

    if (count > 0)
      return false;
    else
      return true;
  } // try
  catch (exc::SQLEmptyBaseException &ex) {
    std::cerr << ex.what() << "\n";
    return false;
  }
}
//
//
//
void clearBaseSQL(PGconn *conn) {}

//
//
//
bool checkBaseTablesSQL(PGconn *conn) {

  using json = nlohmann::json;

  std::ifstream file_schema(std::string(CONFIG_DIR) + "/schema_db.conf");
  if (!file_schema.is_open()) {
    std::cerr << "schema_db.conf: файл не открыт\n";
    return false;
  }
  json config;
  try {
    file_schema >> config;
  } catch (const std::exception &e) {
    throw exc::SQLReadConfigException(" schema_db.conf: : ошибка JSON: ");
  }

  int quantity = 0;
  try {
    quantity = config.at("database").at("quantity").get<int>();

  } catch (const std::exception &e) {
    throw exc::SQLReadConfigException(" schema_db.conf: quantity ");
  }

  if (quantity <= 0) {
    throw exc::SQLReadConfigException(" schema_db.conf: quantity ");
  }

  try {
    for (int i = 1; i <= quantity; ++i) {

      // взяли очередную таблицу
      std::string tableKey = "table" + std::to_string(i);
      std::string tableName;
      try {
        tableName = config.at("database").at(tableKey).get<std::string>();
        
      } catch (const std::exception &e) {
        throw exc::SQLCreateTableException("schema_db.conf: отсутствует или неверный тип ключа: " + tableKey);
      }

      // сформировали запрос
      std::string sql = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_schema = 'public' "
                        "     AND table_name = '" +
                        tableName + "');";

     // выполнили запрос
      PGresult *result = ExecSQL(conn, sql);
      if (!result) {
        throw exc::SQLTableAbscentException(tableName);
      }
      
      if (PQresultStatus(result) != PGRES_TUPLES_OK || PQntuples(result) < 1 || PQnfields(result) < 1) {
        PQclear(result);
        throw exc::SQLTableAbscentException(tableName);
      }

      // правка №2: безопасная проверка NULL
      if (PQgetisnull(result, 0, 0)) {
        PQclear(result);
        throw exc::SQLTableAbscentException(tableName);
      }
      
      const char *val = PQgetvalue(result, 0, 0);
      bool exists = (val && std::string(val) == "t");

      if (!exists) {
        PQclear(result);
        throw exc::SQLTableAbscentException(tableName);
      }
      PQclear(result);
    }
  } // try
  catch (exc::SQLTableAbscentException &ex) {
    std::cerr << ex.what() << "\n"
              << " Переинициализируйте базу." << "\n";
    return false;
  } catch (exc::SQLReadConfigException &ex) {
    std::cerr << ex.what() << "\n"
              << " Переинициализируйте базу." << "\n";
    return false;
  } catch (exc::SQLCreateTableException &ex) {
    std::cerr << ex.what() << "\n"
              << " Переинициализируйте базу." << "\n";
    return false;
  }
  return true;
}
//
//
//

bool createTablesSQL(PGconn *conn, std::string &sql) { return true; }
//
//
// sql command
PGresult *ExecSQL(PGconn *conn, const std::string &sql) {

  PGresult *result = PQexec(conn, sql.c_str());

  try {
    if (!result) {
      std::string error = PQerrorMessage(conn);
      throw exc::SQLExecException(error);
    }
    if (PQresultStatus(result) != PGRES_COMMAND_OK && PQresultStatus(result) != PGRES_TUPLES_OK) {
      std::string error = PQresultErrorMessage(result);
      PQclear(result);
      throw exc::SQLExecException(error);
    }
  } // try
  catch (exc::SQLExecException &ex) {
    std::cerr << ex.what() << "\n";
    return nullptr;
  }

  return result;
}

//
//
//
PGresult *selectSQL(PGconn *conn, const std::string &sql) {

  PGresult *result = PQexec(conn, sql.c_str());
  try {

    if (!result) {
      std::string error = PQerrorMessage(conn);
      throw exc::SQLSelectException(error);
    }
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
      std::string error = PQresultErrorMessage(result);
      PQclear(result);
      throw exc::SQLSelectException(error);
    }
  } // try
  catch (exc::SQLSelectException &ex) {
    std::cerr << ex.what() << "\n";
    return nullptr;
  }

  return result;
  /*
  int rows = PQntuples(res);
    int cols = PQnfields(res);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << PQgetvalue(res, i, j) << "\t";
        }
        std::cout << "\n";*/
}
bool insertUserSQL(PGconn *conn, const std::string &login, const std::string &name) {

  const char *sql = "INSERT INTO users(login, name) VALUES($1, $2)";
  const char *vals[2] = {login.c_str(), name.c_str()};

  try {
    PGresult *result = PQexecParams(conn, sql, 2, nullptr, vals, nullptr, nullptr, 0);

    if (!result) {
      std::string error = PQerrorMessage(conn);
      throw exc::SQLInsertUserException(error);
    }

    if (PQresultStatus(result) != PGRES_COMMAND_OK) {

      std::string error = PQresultErrorMessage(result);
      PQclear(result);
      throw exc::SQLInsertUserException(error);
    }
    PQclear(result);
  } // try
  catch (exc::SQLInsertUserException &ex) {
    std::cerr << ex.what() << "\n";
    return false;
  }
  return true;
}

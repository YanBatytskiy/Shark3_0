#pragma once

#include <libpq-fe.h>
#include <string>

bool checkEmptyBaseSQL(PGconn *conn);

void clearBaseSQL(PGconn *conn);

bool checkBaseTablesSQL(PGconn *conn);

bool createTablesSQL(PGconn *conn, std::string &sql);

// sql command
PGresult * ExecSQL(PGconn *conn, const std::string &sql);
// samples
/*

execSQL(conn, "CREATE TABLE IF NOT EXISTS users ("
              "id bigserial PRIMARY KEY, "
              "login text UNIQUE, "
              "name text)");

execSQL(conn, "INSERT INTO users(login, name) VALUES('yan', 'Yan')");

execSQL(conn, "UPDATE users SET name='Yan B' WHERE login='yan'");

execSQL(conn, "DELETE FROM users WHERE login='yan'");

*/

PGresult * selectSQL(PGconn *conn, const std::string &sql);

bool insertUserSQL(PGconn *conn, const std::string &sql);

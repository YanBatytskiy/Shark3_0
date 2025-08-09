#pragma once

#include <libpq-fe.h>
#include <string>

bool createTables(PGconn *conn, std::string &sql);
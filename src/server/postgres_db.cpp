#include "postgres_db.h"
#include "libpq-fe.h"
#include <iostream>

PostgressDatabase::PostgressDatabase(const std::string &connectionString) {
  conn_ = PQconnectdb(connectionString.c_str());

  if (PQstatus(conn_) != CONNECTION_OK) {
    std::cerr << "[DB ERROR] Connection failed: " << PQerrorMessage(conn_);
  } else {
    std::cout << "[DB INFO] Connection successfull\n";
  }
}

PostgressDatabase::~PostgressDatabase() {
  if (conn_) {
    PQfinish(conn_);
    std::cout << "[DB INFO] Connection closed\n";
  }
}

bool PostgressDatabase::isConnected() const { return conn_ && PQstatus(conn_) == CONNECTION_OK; }

PGconn *PostgressDatabase::getConnection() const { return conn_; }
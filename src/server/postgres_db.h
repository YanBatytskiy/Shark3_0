#pragma once

#include <libpq-fe.h>
#include <string>

class PostgressDatabase {
public:
  explicit PostgressDatabase(const std::string &connectionString);
  ~PostgressDatabase();

  bool isConnected() const;
  PGconn *getConnection() const;

private:
  PGconn* conn_;
};
#pragma once

#include <libpq-fe.h>
#include <string>

class PostgressDatabase {
private:
  PGconn *conn_;
  std::string _host;
  std::string _port;
  std::string _baseName;
  std::string _password;

  std::string _connectionString;

  public:
  explicit PostgressDatabase(const std::string &connectionString);
  ~PostgressDatabase();

  //getters
  bool isConnected() const;
  PGconn *getConnection() const;

  const std::string getHost() const;
  const std::string getPort() const;
  const std::string getBaseName() const;
  const std::string getPassword() const;

  const std::string getConnectionString() const;

  // setters
  void setHost(const std::string& host);
  void setPort(const std::string& port);
  void setBaseName(const std::string& baseName);
  void setPassword(const std::string &password);
  
  void setConnectionString(const std::string& connectionString);
  
};
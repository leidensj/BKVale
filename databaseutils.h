#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlDatabase>
#include <cons.h>

class Database
{
  QSqlDatabase m_db;

public:

  Database();

  bool isOpen(QString& error);

  bool open(QString& error);

  bool createTable(QString& error);

  bool insert(const PromissoryNote note,
              QString& error);

  bool select(int id,
              PromissoryNote& note,
              QString& error);

  int nextNumber(int offset = 1000);
};
#endif // DATABASEUTILS_H

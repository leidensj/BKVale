#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlQuery>
#include <QDate>

namespace BaitaSQL
{
  bool isOpen(QString& error);

  bool open(const QString& hostName,
            int port,
            QString& error);

  void close();

  bool createTables(QString& error);

  bool init(const QString& hostName,
            int port,
            QString& error);
}

#endif // DATABASEUTILS_H

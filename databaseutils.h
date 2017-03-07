#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlDatabase>
#include <note.h>

class Database
{
  QSqlDatabase m_db;

public:

  Database();

  bool isOpen(QString& error);

  bool open(const QString& path,
            QString& error);

  bool createTables(QString& error);

  bool insert(const Note& note,
              QString& error);

  bool select(int id,
              Note& note,
              QString& error);
};
#endif // DATABASEUTILS_H

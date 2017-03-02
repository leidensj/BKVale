#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlDatabase>
#include <promissorynotewidget.h>

class Database
{
  QSqlDatabase m_db;

public:

  Database();

  bool isOpen(QString& error);

  bool open(QString& error);

  bool createTable(QString& error);

  bool insert(const PromissoryNoteWidget& note,
              QString& error);

  bool select(int id,
              PromissoryNoteWidget& note,
              QString& error);

  int nextNumber(int offset = 1000);
};
#endif // DATABASEUTILS_H

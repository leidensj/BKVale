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

  bool open(const QString& path,
            QString& error);

  bool createTables(QString& error);

<<<<<<< HEAD
  bool insert(const PromissoryNoteWidget& note,
=======
  bool insert(PromissoryNote note,
>>>>>>> parent of da3042d... criando banco de dados
              QString& error);

  bool select(int id,
              PromissoryNoteWidget& note,
              QString& error);
};
#endif // DATABASEUTILS_H

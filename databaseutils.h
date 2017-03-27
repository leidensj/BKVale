#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlDatabase>
#include <note.h>
#include <vector>

#define DEFAULT_NUMBER     1000
#define DEFAULT_NUMBER_STR "1000"

class Database
{
  QSqlDatabase m_db;

  void incNumber();

  bool hasConfig();

public:

  Database();

  bool isOpen(QString& error) const;

  bool open(const QString& path,
            QString& error);

  void close();

  bool init(QString& error);

  bool insert(const Note& note,
              QString& error);

  void insertDescriptions(const QStringList& descriptions);

  bool select(int id,
              Note& note,
              QString& error);

  bool selectAll(Notes& notes,
                 QString& error);

  QStringList selectSuppliers();

  QStringList selectDescriptions();

  int number();
};
#endif // DATABASEUTILS_H

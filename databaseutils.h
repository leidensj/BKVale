#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlDatabase>
#include <vector>
#include "note.h"
#include "settings.h"

#define DEFAULT_NUMBER     1000
#define DEFAULT_NUMBER_STR "1000"

namespace NoteDatabase
{
  int nextNumber(QSqlDatabase db);
  void incNumber(QSqlDatabase db);
  QStringList suppliers(QSqlDatabase db);
  QStringList descriptions(QSqlDatabase db);
  bool insertOrUpdate(const Note& note, QSqlDatabase db, QString& error);
}

class Database
{
  QSqlDatabase m_db;

  bool hasConfig();

public:

  Database();

  const QSqlDatabase& getSqlDatabase() const { return m_db; }

  bool isOpen(QString& error) const;

  bool open(const QString& path,
            QString& error);

  void close();

  bool init(QString& error);

  void insertDescriptions(const QStringList& descriptions);

  bool select(int id,
              Note& note,
              QString& error);

  bool selectAll(Notes& notes,
                 QString& error);

  bool insertSettings(const Settings& settings,
                      QString& error);

  void selectSettings(Settings& settings);
};
#endif // DATABASEUTILS_H

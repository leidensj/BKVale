#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlDatabase>
#include <QVector>
#include "note.h"
#include "item.h"
#include "settings.h"

#define DEFAULT_NUMBER     1000
#define DEFAULT_NUMBER_STR "1000"

namespace BaitaDatabase
{
  bool isOpen(QSqlDatabase db,
              QString& error);

  bool hasConfig(QSqlDatabase db);

  bool open(QSqlDatabase db,
            const QString& path,
            QString& error);

  void close(QSqlDatabase db);

  bool init(QSqlDatabase db,
            QString& error);

  bool insertSettings(QSqlDatabase db,
                      const Settings& settings,
                      QString& error);

  void selectSettings(QSqlDatabase db,
                      Settings& settings);
}

namespace NoteDatabase
{
  int nextNumber(QSqlDatabase db);

  void incNumber(QSqlDatabase db);

  QStringList suppliers(QSqlDatabase db);

  bool insertOrUpdate(QSqlDatabase db,
                      const Note& note,
                      QString& error);

  bool select(QSqlDatabase db,
              int id,
              Note& note,
              QString& error);

  bool selectAll(QSqlDatabase db,
                 Notes& notes,
                 QString& error);
}

namespace ItemDatabase
{
  bool select(QSqlDatabase db,
              int id,
              Item& item,
              QString& error);
}

namespace ConsumptionSQL
{
  bool selectTotal(QSqlDatabase db,
                   bool bEnable,
                   qint64 datei,
                   qint64 datef,
                   double& total,
                   QString& error);

  bool selectTotal(QSqlDatabase db,
                   bool bEnable,
                   qint64 datei,
                   qint64 datef,
                   QVector<qint64>& dates,
                   QVector<double>& totals,
                   QString &error);
}

#endif // DATABASEUTILS_H

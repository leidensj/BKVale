#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlDatabase>
#include <QVector>
#include "note.h"
#include "item.h"
#include "consumption.h"
#include "reminder.h"
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

namespace NoteSQL
{
  int nextNumber(QSqlDatabase db);

  void incNumber(QSqlDatabase db);

  QStringList suppliers(QSqlDatabase db);

  bool insertOrUpdate(QSqlDatabase db,
                      const Note& note,
                      QString& error);

  bool select(QSqlDatabase db,
              Note& note,
              QString& error);
}

namespace ItemDatabase
{
  bool select(QSqlDatabase db,
              Item& item,
              QString& error);
}

namespace ReminderSQL
{
  bool insertOrUpdate(QSqlDatabase db,
                      const Reminder& r,
                      QString& error);

  bool select(QSqlDatabase db,
              Reminder& r,
              QString error);

  void setFavorite(QSqlDatabase db,
                   int id,
                   bool bFav);

  bool isFavorite(QSqlDatabase db,
                  int id);
}

namespace ConsumptionSQL
{
  bool selectByDate(QSqlDatabase db,
                    qint64 date,
                    QVector<Consumption>& consumptions,
                    QString& error);

  bool selectTotal(QSqlDatabase db,
                   const Consumption::Filter& filter,
                   double& total,
                   QString& error);

  bool selectSubTotal(QSqlDatabase db,
                      const Consumption::Filter& filter,
                      QVector<qint64>& dates,
                      QVector<double>& totals,
                      QString &error);

  void getConsumption(QSqlDatabase db,
                      qint64 date,
                      QVector<Consumption>& vConsumption,
                      QVector<Item>& vItem);

  double getTotal(QSqlDatabase db,
                  const Consumption::Filter& filter);

  double getTotal(QSqlDatabase db,
                  qint64 date);
}

#endif // DATABASEUTILS_H

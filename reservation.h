#ifndef RESERVATION_H
#define RESERVATION_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDateTime>
#include "defines.h"
#include "jitem.h"

struct Reservation : public SQL_JItem
{
  Reservation();
  void clear();
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query);
  bool SQL_update_proc(QSqlQuery& query);
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query);

  mutable qlonglong m_number;
  QString m_name;
  QString m_location;
  QString m_dateTime;
  QString m_phone;
  int m_ammount;
  QString m_observation;
};

#endif // RESERVATION_H

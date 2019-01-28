#ifndef RESERVATION_H
#define RESERVATION_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDateTime>
#include "defines.h"
#include "jitem.h"

struct Reservation : public JItem
{
  Reservation();
  void clear();
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString strTableName() const;

  mutable qlonglong m_number;
  QString m_name;
  QString m_location;
  QString m_dateTime;
  QString m_phone;
  int m_ammount;
  QString m_observation;
};

#endif // RESERVATION_H

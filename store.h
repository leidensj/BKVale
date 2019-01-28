#ifndef STORE_H
#define STORE_H

#include "jitem.h"
#include "person.h"
#include "timeinterval.h"
#include <QVector>
#include <QVariantList>

struct StoreEmployee : public JItem
{
  StoreEmployee();
  void clear();
  bool isValid() const;
  bool operator ==(const JItem& other) const;
  bool operator !=(const JItem& other) const;
  QString strTableName() const;

  QString strHours() const;

  Person m_employee;
  QVector<TimeInterval> m_hours;
};

struct Store : public JItem
{
  Store();
  void clear();
  bool isValid() const;
  bool operator ==(const JItem& other) const;
  bool operator !=(const JItem& other) const;
  QString strTableName() const;

  Person m_person;
  Address m_address;
  Phone m_phone;
  QString m_name;
  QVector<StoreEmployee> m_vEmployee;
};

#endif // STORE_H

#ifndef STORE_H
#define STORE_H

#include "jitem.h"
#include "person.h"
#include "timeinterval.h"
#include <QVector>
#include <QVariantList>

struct StoreEmployee : public JItem
{
  Person m_employee;
  QVector<TimeInterval> m_hours;

  void clear()
  {
    m_employee.clear();
  }

  StoreEmployee()
  {
    clear();
    m_hours.clear();
  }

  QString strHours() const
  {
    QString str;
    for (int i = 0; i != m_hours.size(); ++i)
      str += m_hours.at(i).m_tmBegin.toString("hh:mm") + "-" + m_hours.at(i).m_tmEnd.toString("hh:mm") + " ";
    if (m_hours.size() > 0)
      str.chop(1);
    return str;
  }
};

struct Store : public JItem
{
  Person m_person;
  Address m_address;
  Phone m_phone;
  QString m_name;
  QVector<StoreEmployee> m_vEmployee;

  void clear()
  {
    m_person.clear();
    m_address.clear();
    m_phone.clear();
    m_name.clear();
    m_vEmployee.clear();
  }

  Store()
  {
    clear();
  }
};

#endif // STORE_H

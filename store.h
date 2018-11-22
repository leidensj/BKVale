#ifndef STORE_H
#define STORE_H

#include "jitem.h"
#include "person.h"
#include <QVector>
#include <QVariantList>

#define STORE_EMPLOYEE_EVERY_DAY 0

struct StoreEmployeeHour : public JItem
{
  int m_day;
  QTime m_tmBegin;
  QTime m_tmEnd;

  void clear()
  {
    m_day = STORE_EMPLOYEE_EVERY_DAY;
  }

  StoreEmployeeHour()
  {
    clear();
  }
};

struct StoreEmployee : public JItem
{
  Person m_employee;
  QVector<StoreEmployeeHour> m_hours;

  void clear()
  {
    m_employee.clear();
  }

  StoreEmployee()
  {
    clear();
    m_hours.clear();
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

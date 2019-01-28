#include "store.h"

StoreEmployee::StoreEmployee()
{
  clear();
}

void StoreEmployee::clear()
{
  m_employee.clear();
  m_hours.clear();
}

bool StoreEmployee:: operator != (const JItem& other) const
{
  const StoreEmployee& another = dynamic_cast<const StoreEmployee&>(other);
  return m_employee.m_id != another.m_employee.m_id ||
         m_hours != another.m_hours;
}

bool StoreEmployee:: operator == (const JItem& other) const
{
  return !(*this != other);
}

bool StoreEmployee::isValid() const
{
  return m_employee.m_id.isValid();
}

QString StoreEmployee::strTableName() const
{
  return STORE_EMPLOYEES_SQL_TABLE_NAME;
}

QString StoreEmployee::strHours() const
{
  QString str;
  for (int i = 0; i != m_hours.size(); ++i)
    str += m_hours.at(i).m_tmBegin.toString("hh:mm") + "-" + m_hours.at(i).m_tmEnd.toString("hh:mm") + " ";
  if (m_hours.size() > 0)
    str.chop(1);
  return str;
}

Store::Store()
{
  clear();
}

void Store::clear()
{
  m_person.clear();
  m_address.clear();
  m_phone.clear();
  m_name.clear();
  m_vEmployee.clear();
}

bool Store:: operator != (const JItem& other) const
{
  const Store& another = dynamic_cast<const Store&>(other);
  return m_person.m_id != another.m_person.m_id ||
                           m_address.m_id != another.m_address.m_id ||
                           m_phone.m_id != another.m_phone.m_id ||
                           m_name != another.m_name ||
                           m_vEmployee != another.m_vEmployee;
}

bool Store:: operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Store::isValid() const
{
  return m_person.m_id.isValid();
}

QString Store::strTableName() const
{
  return STORE_SQL_TABLE_NAME;
}



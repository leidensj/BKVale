#include "reservation.h"

Reservation::Reservation()
{
  clear();
}

void Reservation::clear()
{
  m_number = 0;
  m_dateTime = QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate);
  m_location.clear();
  m_name.clear();
  m_ammount = 0;
  m_observation.clear();
  m_phone.clear();
}



bool Reservation::operator != (const JItem& other) const
{
  const Reservation& another = dynamic_cast<const Reservation&>(other);
  return m_dateTime != another.m_dateTime ||
                       m_location != another.m_location ||
                                     m_name != another.m_name ||
                                               m_ammount != another.m_ammount ||
                                                            m_phone != another.m_phone;
}

bool Reservation::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Reservation::isValid() const
{
  return true;
}

QString Reservation::strTableName() const
{
  return RESERVATION_SQL_TABLE_NAME;
}

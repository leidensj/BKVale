#include "phone.h"

Phone::Phone()
{
  clear();
}

void Phone::clear()
{
  m_id.clear();
  m_countryCode = PHONE_DEFAULT_COUNTRY_CODE_VALUE;
  m_code = PHONE_DEFAULT_CODE_VALUE;
  m_number.clear();
  m_name.clear();
}

bool Phone::operator !=(const JItem& other) const
{
  const Phone& another = dynamic_cast<const Phone&>(other);
  return
      m_countryCode != another.m_countryCode ||
      m_code != another.m_code ||
      m_number != another.m_number ||
      m_name != another.m_name;
}

bool Phone::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Phone::isValid() const
{
  return !m_number.isEmpty();
}

QString Phone::strTableName() const
{
  return PHONE_SQL_TABLE_NAME;
}

QString Phone::strFormattedPhone() const
{
  return
    "+" +
    QString::number(m_countryCode) +
    " (" +
    QString::number(m_code) +
    ") " +
    m_number;
}

QString Phone::strFormattedPhoneWithName() const
{
  QString str;
  if (!m_name.isEmpty())
    str += m_name + " ";
  str += strFormattedPhone();
  return str;
}

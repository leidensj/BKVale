#ifndef PHONE_H
#define PHONE_H

#include <QObject>
#include <QString>
#include "defines.h"
#include "jitem.h"

struct Phone : public JItem
{
  int m_countryCode;
  int m_code;
  QString m_number;
  QString m_name;

  void clear()
  {
    m_id.clear();
    m_countryCode = PHONE_DEFAULT_COUNTRY_CODE_VALUE;
    m_code = PHONE_DEFAULT_CODE_VALUE;
    m_number.clear();
    m_name.clear();
  }

  Phone()
  {
    clear();
  }

  bool operator !=(const JItem& other) const
  {
    const Phone& another = dynamic_cast<const Phone&>(other);
    return
        m_countryCode != another.m_countryCode ||
        m_code != another.m_code ||
        m_number != another.m_number ||
        m_name != another.m_name;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    return !m_number.isEmpty();
  }

  QString strFormattedPhone() const
  {
    return
      "+" +
      QString::number(m_countryCode) +
      " (" +
      QString::number(m_code) +
      ") " +
      m_number;
  }

  QString strFormattedPhoneWithName() const
  {
    QString str;
    if (!m_name.isEmpty())
      str += m_name + " ";
    str += strFormattedPhone();
    return str;
  }
};

#endif // PHONENUMBER_H

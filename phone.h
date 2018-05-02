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

  void clear()
  {
    m_id = INVALID_ID;
    m_countryCode = PHONE_DEFAULT_COUNTRY_CODE_VALUE;
    m_code = PHONE_DEFAULT_CODE_VALUE;
    m_number.clear();
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
        m_number != another.m_number;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    return !m_number.isEmpty();
  }

  QString getFormattedPhone() const
  {
    return
      "+" +
      QString::number(m_countryCode) +
      " (" +
      QString::number(m_code) +
      ") " +
      m_number;
  }
};

#endif // PHONENUMBER_H

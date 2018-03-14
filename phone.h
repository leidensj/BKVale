#ifndef PHONE_H
#define PHONE_H

#include <QObject>
#include <QString>
#include "jtablecolumn.h"
#include "defines.h"

struct Phone
{
  mutable qlonglong m_id;
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

  bool operator !=(const Phone& other) const
  {
    return
        m_countryCode != other.m_countryCode ||
        m_code != other.m_code ||
        m_number != other.m_number;
  }

  bool operator ==(const Phone& other) const
  {
    return !(*this != other);
  }

  static bool st_isValid(const Phone& phone)
  {
    return !phone.m_number.isEmpty();
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

  bool isValid() const { return st_isValid(*this); }
  bool isValidId() const { return IS_VALID_ID(m_id); }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(PHONE_SQL_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(PHONE_SQL_COL01, QObject::tr("Id Pessoa")));
    c.push_back(JTableColumn(PHONE_SQL_COL02, QObject::tr("País"), false));
    c.push_back(JTableColumn(PHONE_SQL_COL03, QObject::tr("Código"), false));
    c.push_back(JTableColumn(PHONE_SQL_COL04, QObject::tr("Número"), false, false, JResizeMode::Stretch));
    return c;
  }
};

#endif // PHONENUMBER_H

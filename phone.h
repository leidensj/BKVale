#ifndef PHONE_H
#define PHONE_H

#include <QObject>
#include <QString>
#include "jtablecolumn.h"
#include "person.h"

#define SQL_PHONE_TABLE_NAME "_PHONES"
#define SQL_PHONE_COL00 "_ID"
#define SQL_PHONE_COL01 "_PERSONID"
#define SQL_PHONE_COL02 "_COUNTRYCODE"
#define SQL_PHONE_COL03 "_CODE"
#define SQL_PHONE_COL04 "_NUMBER"


#define INVALID_PHONE_ID                       -1
#define DEFAULT_PHONE_COUNTRY_CODE_VALUE       55
#define DEFAULT_PHONE_CODE_VALUE               54
#define DEFAULT_PHONE_COUNTRY_CODE_VALUE_STR "55"
#define DEFAULT_PHONE_CODE_VALUE_STR         "54"

struct Phone
{
  mutable int m_id;
  int m_countryCode;
  int m_code;
  QString m_number;

  void clear()
  {
    m_id = INVALID_PHONE_ID;
    m_countryCode = DEFAULT_PHONE_COUNTRY_CODE_VALUE;
    m_code = DEFAULT_PHONE_CODE_VALUE;
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
  static bool st_isValidId(int id) { return id != INVALID_PHONE_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(SQL_PHONE_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(SQL_PHONE_COL01, QObject::tr("Id Pessoa")));
    c.push_back(JTableColumn(SQL_PHONE_COL02, QObject::tr("País"), false));
    c.push_back(JTableColumn(SQL_PHONE_COL03, QObject::tr("Código"), false));
    c.push_back(JTableColumn(SQL_PHONE_COL04, QObject::tr("Número"), false, false, JResizeMode::Stretch));
    return c;
  }
};

#endif // PHONENUMBER_H

#ifndef PHONE_H
#define PHONE_H

#include <QObject>
#include <QString>
#include "jtablecolumn.h"
#include "person.h"

#define INVALID_PHONE_ID           -1
#define DEFAULT_COUNTRY_CODE_VALUE 55
#define DEFAULT_CODE_VALUE         54

struct Phone
{
  mutable int m_id;
  int m_countryCode;
  int m_code;
  QString m_number;

  void clear()
  {
    m_id = INVALID_PHONE_ID;
    m_countryCode = DEFAULT_COUNTRY_CODE_VALUE;
    m_code = DEFAULT_CODE_VALUE;
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
  static QString getTableName() { return "_PHONE_NUMBERS"; }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn("_ID", QObject::tr("Id")));
    c.push_back(JTableColumn("_PERSONID", QObject::tr("Pessoa")));
    c.push_back(JTableColumn("_COUNTRYCODE", QObject::tr("País"), false));
    c.push_back(JTableColumn("_CODE", QObject::tr("Código"), false));
    c.push_back(JTableColumn("_NUMBER", QObject::tr("Número"), false, false, JResizeMode::Stretch));
    return c;
  }
};

#endif // PHONENUMBER_H

#ifndef PHONENUMBER_H
#define PHONENUMBER_H

#include <QObject>
#include <QString>
#include "jtablecolumn.h"
#include "person.h"

#define INVALID_PHONE_NUMBER_ID   -1

struct PhoneNumber
{
  mutable int m_id;
  int m_personId;
  QString m_countryCode;
  QString m_code;
  QString m_number;
  bool m_bMobile;

  void clear()
  {
    m_id = INVALID_PHONE_NUMBER_ID;
    m_personId = INVALID_PERSON_ID;
    m_countryCode.clear();
    m_code.clear();
    m_number.clear();
    m_bMobile = false;
  }

  PhoneNumber()
  {
    clear();
  }

  bool operator !=(const PhoneNumber& other)
  {
    return
        m_countryCode != other.m_countryCode ||
        m_code != other.m_code ||
        m_number != other.m_number ||
        m_bMobile != other.m_bMobile;
  }

  static bool st_isValid(const PhoneNumber& phoneNumber)
  {
    return
        !phoneNumber.m_countryCode.isEmpty() &&
        !phoneNumber.m_code.isEmpty() &&
        !phoneNumber.m_number.isEmpty();
  }

  bool isValid() const { return st_isValid(*this); }
  static bool st_isValidId(int id) { return id != INVALID_PHONE_NUMBER_ID; }
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
    c.push_back(JTableColumn("_IS_MOBILE", QObject::tr("Celular")));
    return c;
  }
};

#endif // PHONENUMBER_H

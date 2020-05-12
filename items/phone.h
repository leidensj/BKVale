#ifndef PHONE_H
#define PHONE_H

#include <QObject>
#include <QString>
#include "defines.h"
#include "jitemsql.h"

struct Phone : public JItemSQL
{
  Phone(Id id = Id());
  void clear(bool bClearId = true);
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  QString strFormattedPhone() const;
  QString strFormattedPhoneWithName() const;

  int m_countryCode;
  int m_code;
  QString m_number;
  QString m_name;

  QString name() const { return strFormattedPhone(); }
};

#endif // PHONENUMBER_H

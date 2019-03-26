#ifndef PHONE_H
#define PHONE_H

#include <QObject>
#include <QString>
#include "defines.h"
#include "jitem.h"

struct Phone : public SQL_JItem
{
  Phone();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query);
  bool SQL_update_proc(QSqlQuery& query);
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query);

  QString strFormattedPhone() const;
  QString strFormattedPhoneWithName() const;

  int m_countryCode;
  int m_code;
  QString m_number;
  QString m_name;
};

#endif // PHONENUMBER_H

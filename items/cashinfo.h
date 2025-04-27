#ifndef CASHINFO_H
#define CASHINFO_H

#include "jsubitemsql.h"
#include <QVector>

struct CashInfo : public JSubItemSQL
{
  CashInfo();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashInfo>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  QString m_name;
  Data::Type m_type;
};

#endif // CASHINFO_H

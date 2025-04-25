#ifndef CASHCLOSINGINFO_H
#define CASHCLOSINGINFO_H

#include "jsubitemsql.h"
#include <QVector>

struct CashClosingInfo : public JSubItemSQL
{
  CashClosingInfo();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashClosingInfo>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  QString m_iname;
  int m_itype;
  QVariant m_value;
};

#endif // CASHCLOSINGINFO_H

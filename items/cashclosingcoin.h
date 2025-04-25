#ifndef CASHCLOSINGCOIN_H
#define CASHCLOSINGCOIN_H

#include "jsubitemsql.h"
#include "items/image.h"
#include <QVector>

struct CashClosingCoin : public JSubItemSQL
{
  CashClosingCoin();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashClosingCoin>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  QString m_cname;
  double m_ctax;
  Image m_cimage;
  double m_value;
};

#endif // CASHCLOSINGCOIN_H

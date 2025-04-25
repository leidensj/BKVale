#ifndef CASHCOIN_H
#define CASHCOIN_H

#include "jsubitemsql.h"
#include "coin.h"
#include <QVector>

struct CashCoin : public JSubItemSQL
{
  CashCoin();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashCoin>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  Coin m_coin;
};

#endif // CASHCOIN_H

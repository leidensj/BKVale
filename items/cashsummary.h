#ifndef CASHSUMMARY_H
#define CASHSUMMARY_H

#include "jsubitemsql.h"
#include <QVector>

struct CashSummary : public JSubItemSQL
{
  enum class Item
  {
    Sales,
    Debit,
    Credit,
    Comission,
    TotalGross,
    TotalNet,
    Tax,
    RealTotal,
    RealSales,
    Difference,
    Discrepancy,
    Cards,
    NONE
  };

  CashSummary();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CashSummary>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);
  static QString st_ItemToText(Item item);
  QString itemToText() const;

  Item m_item;
  bool m_bShow;
};

#endif // CASHSUMMARY_H

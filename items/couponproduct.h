#ifndef COUPONPRODUCT_H
#define COUPONPRODUCT_H

#include "jsubitemsql.h"
#include "product.h"
#include <QVector>

struct CouponProduct : public JSubItemSQL
{
  CouponProduct();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CouponProduct>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  QString strAmmount() const;
  QString strFmt() const;

  Product m_product;
  double m_ammount;
};

#endif // COUPONPRODUCT_H

#ifndef PURCHASEPRODUCT_H
#define PURCHASEPRODUCT_H

#include "jsubitemsql.h"
#include "product.h"
#include <QVector>

struct PurchaseProduct : public JSubItemSQL
{
  PurchaseProduct();
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);

  void SQL_select_last(Id supplierId, Id productId);
  static bool SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<PurchaseProduct>& v, QString& error);
  static bool SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId);

  double subtotal() const;
  QString strSubtotal() const;
  QString strAmmount() const;
  QString strPrice() const;

  Product m_product;
  double m_ammount;
  double m_price;
  Package m_package;
};

#endif // PURCHASEPRODUCT_H

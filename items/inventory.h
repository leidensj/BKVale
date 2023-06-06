#ifndef INVENTORY_H
#define INVENTORY_H

#include "inventoryproduct.h"
#include "store.h"

struct Inventory : public JItemSQL
{
  Inventory();
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  Store m_store;
  QDateTime m_dt;
  QString m_description;
  QVector<InventoryProduct> m_products;
  QVector<double> m_purchaseAmmount;
};

#endif // INVENTORY_H

#ifndef PRICE_H
#define PRICE_H

#include "product.h"

struct Price : public JItemSQL
{
  Price();
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  static bool SQL_add_prices(QString& error);

  double m_value;
  Product m_product;
};

#endif // PRICE_H

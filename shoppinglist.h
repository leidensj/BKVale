#ifndef SHOPPINGLIST_H
#define SHOPPINGLIST_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QDate>
#include <QRegExp>
#include "person.h"
#include "product.h"
#include "defines.h"
#include "jitemsql.h"

enum class ShoppingListColumn : int
{
  Unity,
  Description,
  Ammount,
  Price,
  Supplier
};

struct ShoppingListItem : JItem
{
  ShoppingListItem();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString strTableName() const;

  double subtotal() const;
  QString strSubtotal() const;
  QString strAmmount() const;
  QString strPrice() const;

  Product m_product;
  bool m_bAmmount;
  double m_ammount;
  bool m_bPrice;
  double m_price;
  Package m_package;
  Person m_supplier;
};

struct ShoppingList : public JItemSQL
{
  ShoppingList();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  void setWeekDays(const QString& strWeekDays);
  void setMonthDays(const QString& strMonthDays);
  QString getWeekDays() const;
  QString getMonthDays() const;

  QString m_title;
  QString m_description;
  Person m_supplier;
  Image m_image;
  int m_nLines;
  bool m_weekDays[7];
  bool m_monthDays[31];
  QVector<ShoppingListItem> m_vItem;

  QString text() const { return "Lista de Compras"; }
  QString icon() const { return ":/icons/res/shopmgt.png"; }
};


#endif // SHOPPINGLIST_H

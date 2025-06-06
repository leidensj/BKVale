#ifndef SHOPPINGLIST_H
#define SHOPPINGLIST_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QDate>
#include <QRegularExpression>
#include "supplier.h"
#include "product.h"
#include "defines.h"
#include "jitemsql.h"

struct ShoppingListItem : public JItem
{
  ShoppingListItem();
  void clear(bool bClearId = true);
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;

  double subtotal() const;
  QString strSubtotal() const;
  QString strAmmount() const;
  QString strPrice() const;

  Product m_product;
  double m_ammount;
  double m_price;
  Package m_package;
  Supplier m_supplier;
};

struct ShoppingList : public JItemSQL
{
  ShoppingList(Id id = Id());
  void clear(bool bClearId = true);
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;
  QByteArray printVersion(const QVariant& arg) const;

  void setWeekDays(const QString& strWeekDays);
  void setMonthDays(const QString& strMonthDays);
  QString getWeekDays() const;
  QString getMonthDays() const;

  QString m_title;
  QString m_description;
  Supplier m_supplier;
  Image m_image;
  int m_nLines;
  bool m_weekDays[7];
  bool m_monthDays[31];
  QVector<ShoppingListItem> m_vItem;
};


#endif // SHOPPINGLIST_H

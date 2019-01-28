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
#include "jitem.h"

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

struct ShoppingList : public JItem
{
  ShoppingList();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString strTableName() const;

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
};


#endif // SHOPPINGLIST_H

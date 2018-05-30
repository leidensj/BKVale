#ifndef SHOPPINGLIST_H
#define SHOPPINGLIST_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QDate>
#include <QMetaType>
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
  Price
};

struct ShoppingListItem : JItem
{
  Product m_product;
  double m_ammount;
  double m_price;
  bool m_bIsPackageAmmount;

  void clear()
  {
    m_id = INVALID_ID;
    m_product.clear();
    m_ammount = 0.0;
    m_price = 0.0;
    m_bIsPackageAmmount = true;
  }

  ShoppingListItem()
  {
    clear();
  }

  QString strUnity() const { return m_bIsPackageAmmount
                                    ? m_product.m_packageUnity
                                    : m_product.m_unity; }
  double subtotal() const { return m_ammount * m_price; }
  static QString st_strSubTotal(double subtotal) { return QString::number(subtotal, 'f', 2); }
  static QString st_strAmmount(double ammount) { return QString::number(ammount, 'f').remove(QRegExp("\\.?0*$")); }
  static QString st_strPrice(double price) { return QString::number(price, 'f', 2); }
  QString strSubtotal() const { return st_strSubTotal(subtotal()); }
  QString strAmmount() const { return st_strAmmount(m_ammount); }
  QString strPrice() const { return st_strPrice(m_price); }
  bool isValid() const
  {
    return m_product.isValidId();
  }

  bool operator !=(const JItem& other) const
  {
    const ShoppingListItem& another = dynamic_cast<const ShoppingListItem&>(other);
    return
        m_product.m_id != another.m_product.m_id ||
        m_ammount != another.m_ammount ||
        m_price != another.m_price ||
        m_bIsPackageAmmount != another.m_bIsPackageAmmount;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }
};

Q_DECLARE_METATYPE(ShoppingListItem)

struct ShoppingList : public JItem
{
  QString m_title;
  QString m_description;
  Person m_supplier;
  Image m_image;
  bool m_weekDays[7];
  bool m_monthDays[31];
  QVector<ShoppingListItem> m_vItem;
  bool m_bSupplierCalls;
  bool m_bCallSupplier;
  bool m_bWhatsapp;
  bool m_bVisit;
  bool m_bPrintAmmount;
  bool m_bPrintPrice;

  void clear()
  {
    m_id = INVALID_ID;
    m_title.clear();
    m_description.clear();
    m_supplier.clear();
    m_image.clear();
    for (int i = 0; i != 7; ++i)
      m_weekDays[i] = false;
    for (int i = 0; i != 31; ++i)
      m_monthDays[i] = false;
    m_vItem.clear();
    m_bSupplierCalls = false;
    m_bCallSupplier = false;
    m_bWhatsapp = false;
    m_bVisit = false;
    m_bPrintAmmount = true;
    m_bPrintPrice = true;
  }

  ShoppingList()
  {
    clear();
  }

  bool isValid() const
  {
    bool b = true;
    for (int i = 0; i != m_vItem.size(); ++i)
      b &= m_vItem.at(i).isValid();
    return b;
  }

  void setWeekDays(const QString& strWeekDays)
  {
    for (int i = 0; i != 7; ++i)
      m_weekDays[i] = false;
    QStringList lst = strWeekDays.split(SHOPPING_LIST_SEPARATOR);
    for (int i = 0; i != lst.size(); ++i)
    {
      int day = lst.at(i).toInt();
      if (day > 0 && day < 8)
        m_weekDays[day - 1] = true;
    }
  }

  void setMonthDays(const QString& strMonthDays)
  {
    for (int i = 0; i != 7; ++i)
      m_monthDays[i] = false;
    QStringList lst = strMonthDays.split(SHOPPING_LIST_SEPARATOR);
    for (int i = 0; i != lst.size(); ++i)
    {
      int day = lst.at(i).toInt();
      if (day > 0 && day < 32)
        m_monthDays[day - 1] = true;
    }
  }

  QString getWeekDays() const
  {
    QString strWeekDays;
    for (int i = 0; i != 7; ++i)
      if (m_weekDays[i])
        strWeekDays += SHOPPING_LIST_SEPARATOR + QString::number(i + 1);
    if (!strWeekDays.isEmpty())
      strWeekDays += SHOPPING_LIST_SEPARATOR;
    return strWeekDays;
  }

  QString getMonthDays() const
  {
    QString strMonthDays;
    for (int i = 0; i != 31; ++i)
      if (m_monthDays[i])
        strMonthDays += SHOPPING_LIST_SEPARATOR + QString::number(i + 1);
    if (!strMonthDays.isEmpty())
      strMonthDays += SHOPPING_LIST_SEPARATOR;
    return strMonthDays;
  }

  bool hasContact() const
  {
    return m_bSupplierCalls || m_bCallSupplier || m_bVisit || m_bWhatsapp;
  }

  bool operator !=(const JItem& other) const
  {
    const ShoppingList& another = dynamic_cast<const ShoppingList&>(other);
    bool b = m_supplier.m_id != another.m_supplier.m_id ||
             m_description != another.m_description ||
             m_vItem != another.m_vItem ||
             m_bSupplierCalls != another.m_bSupplierCalls ||
             m_bCallSupplier != another.m_bCallSupplier ||
             m_bWhatsapp != another.m_bWhatsapp ||
             m_bVisit != another.m_bVisit;
    if (!b)
    {
      for (int i = 0; i != 7; ++i)
      {
        b |= m_weekDays[i] != another.m_weekDays[i];
        if (b)
          break;
      }

      if (!b)
      for (int i = 0; i != 31; ++i)
      {
        b |= m_monthDays[i] != another.m_monthDays[i];
        if (b)
          break;
      }
    }

    return b;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }
};


#endif // SHOPPINGLIST_H

#ifndef SHOPPINGLIST_H
#define SHOPPINGLIST_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QDate>
#include <QMetaType>
#include "jtablecolumn.h"
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
  bool m_bPrintAmmount;
  bool m_bPrintPrice;

  void clear()
  {
    m_id = INVALID_ID;
    m_product.clear();
    m_ammount = 0.0;
    m_price = 0.0;
    m_bIsPackageAmmount = true;
    m_bPrintAmmount = true;
    m_bPrintPrice = true;
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
  static QString st_strAmmount(double ammount) { return QString::number(ammount, 'f', 3); }
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
  Person m_supplier;
  Image m_image;
  QStringList m_dates;
  QString m_description;
  QVector<ShoppingListItem> m_vItem;
  bool m_bSupplierCalls;
  bool m_bCallSupplier;
  bool m_bWhatsapp;
  bool m_bVisit;

  void clear()
  {
    m_id = INVALID_ID;
    m_supplier.clear();
    m_dates.clear();
    m_description.clear();
    m_vItem.clear();
    m_bSupplierCalls = false;
    m_bCallSupplier = false;
    m_bWhatsapp = false;
    m_bVisit = false;
  }

  ShoppingList()
  {
    clear();
  }

  bool isValid() const
  {
    bool b = m_supplier.isValidId() &&
             !m_dates.isEmpty() &&
             !m_vItem.isEmpty();
    for (int i = 0; i != m_vItem.size(); ++i)
      b &= m_vItem.at(i).isValid();
    return b;
  }

  bool operator !=(const JItem& other) const
  {
    const ShoppingList& another = dynamic_cast<const ShoppingList&>(other);
    return
        m_supplier.m_id != another.m_supplier.m_id ||
        m_dates != another.m_dates ||
        m_description != another.m_description ||
        m_vItem != another.m_vItem ||
        m_bSupplierCalls != another.m_bSupplierCalls ||
        m_bCallSupplier != another.m_bCallSupplier ||
        m_bWhatsapp != another.m_bWhatsapp ||
        m_bVisit != another.m_bVisit;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }
};


#endif // SHOPPINGLIST_H

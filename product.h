#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jitem.h"
#include "defines.h"
#include "category.h"
#include <QRegExp>

struct Package
{
  bool m_bIsPackage;
  QString m_unity;
  double m_ammount;

  void clear()
  {
    m_bIsPackage = false;
    m_unity.clear();
    m_ammount = 0.0;
  }

  Package()
  {
    clear();
  }

  bool operator !=(const Package& other) const
  {
    return
        m_bIsPackage != other.m_bIsPackage ||
        m_unity != other.m_unity ||
        m_ammount != other.m_ammount;
  }

  QString strFormattedUnity(const QString& productUnity) const
  {
    return !m_bIsPackage
        ? productUnity
        : m_unity + " (" +
          QString::number(m_ammount, 'f', 3).remove(QRegExp("\\.?0*$")) +
          productUnity + ")";
  }

  QString strUnity(const QString& productUnity) const
  {
    return m_bIsPackage ? m_unity : productUnity;
  }
};

struct Product : public JItem
{
  Category m_category;
  Image m_image;
  QString m_name;
  QString m_unity;
  QString m_details;
  bool m_bAvailableAtNotes;
  bool m_bAvailableAtShop;
  bool m_bAvailableAtConsumption;
  bool m_bAvailableToBuy;
  bool m_bAvailableToSell;
  bool m_bAvailableAtShoppingList;

  void clear()
  {
    m_id.clear();
    m_category.clear();
    m_image.clear();
    m_name.clear();
    m_unity.clear();
    m_details.clear();
    m_bAvailableAtNotes = false;
    m_bAvailableAtShop = false;
    m_bAvailableAtConsumption = false;
    m_bAvailableToBuy = false;
    m_bAvailableToSell = false;
    m_bAvailableAtShoppingList = false;
  }

  Product()
  {
    clear();
  }

  bool operator != (const JItem& other) const
  {
    const Product& another = dynamic_cast<const Product&>(other);
    return
        m_image.m_id != another.m_image.m_id ||
        m_category.m_id != another.m_category.m_id ||
        m_name != another.m_name ||
        m_unity != another.m_unity ||
        m_details != another.m_details ||
        m_bAvailableAtNotes != another.m_bAvailableAtNotes ||
        m_bAvailableAtShop != another.m_bAvailableAtShop ||
        m_bAvailableAtConsumption != another.m_bAvailableAtConsumption ||
        m_bAvailableToBuy != another.m_bAvailableToBuy ||
        m_bAvailableToSell != another.m_bAvailableToSell ||
        m_bAvailableAtShoppingList != another.m_bAvailableAtShoppingList;
  }

  bool operator == (const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    return
        !m_name.isEmpty() &&
        !m_unity.isEmpty();
  }
};

#endif // PRODUCT_H

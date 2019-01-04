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

struct ProductCode : public JItem
{
  QString m_code;

  void clear()
  {
    m_id.clear();
    m_code.clear();
  }

  ProductCode()
  {
    clear();
  }

  bool operator != (const JItem& other) const
  {
    const ProductCode& another = dynamic_cast<const ProductCode&>(other);
    return m_code != another.m_code;
  }

  bool operator == (const JItem& other) const
  {
    return !(*this != other);
  }
};

struct Product : public JItem
{
  Category m_category;
  Image m_image;
  QString m_name;
  QString m_unity;
  QString m_details;
  bool m_bBuy;
  bool m_bSell;
  QVector<ProductCode> m_vCode;

  void clear()
  {
    m_id.clear();
    m_category.clear();
    m_image.clear();
    m_name.clear();
    m_unity.clear();
    m_details.clear();
    m_bBuy = false;
    m_bSell = false;
    m_vCode.clear();
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
        m_bBuy != another.m_bBuy ||
        m_bSell != another.m_bSell ||
        m_vCode != another.m_vCode;
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

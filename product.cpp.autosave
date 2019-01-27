#include "product.h"

Package::Package()
{
  clear();
}

  void Package::clear()
  {
    m_bIsPackage = false;
    m_unity.clear();
    m_ammount = 0.0;
  }

bool Package::operator !=(const Package& other) const
{
  return
      m_bIsPackage != other.m_bIsPackage ||
      m_unity != other.m_unity ||
      m_ammount != other.m_ammount;
}

QString Package::strFormattedUnity(const QString& productUnity) const
{
  return !m_bIsPackage
      ? productUnity
      : m_unity + " (" +
        QString::number(m_ammount, 'f', 3).remove(QRegExp("\\.?0*$")) +
        productUnity + ")";
}

QString Package::strUnity(const QString& productUnity) const
{
  return m_bIsPackage ? m_unity : productUnity;
}

ProductCode::ProductCode()
{
  clear();
}

void ProductCode::clear()
{
  m_id.clear();
  m_code.clear();
}

bool ProductCode::operator != (const JItem& other) const
{
  const ProductCode& another = dynamic_cast<const ProductCode&>(other);
  return m_code != another.m_code;
}

bool ProductCode::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool ProductCode::isValid() const
{
  return !m_code.isEmpty();
}

QString ProductCode::strTableName() const
{
  return PRODUCT_CODE_ITEMS_SQL_TABLE_NAME;
}




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

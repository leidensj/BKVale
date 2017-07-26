#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

#define INVALID_PRODUCT_ID   -1
#define INVALID_PRODUCT_ICON -1

enum class ProductTableIndex
{
  ID = 0,
  Description,
  Unity,
  Supplier,
  Price,
  Details,
  MidasCode,
  Icon
};

struct Product
{
  Product()
    : m_id(INVALID_PRODUCT_ID)
    , m_icon(INVALID_PRODUCT_ICON)
  {

  }

  int m_id;
  QString m_description;
  QString m_unity;
  QString m_supplier;
  double m_price;
  QString m_details;
  QString m_midasCode;
  int m_icon;

  void clear()
  {
    m_id = INVALID_PRODUCT_ID;
    m_icon = INVALID_PRODUCT_ICON;
    m_description.clear();
    m_unity.clear();
    m_supplier.clear();
    m_price = 0.0;
    m_details.clear();
    m_midasCode.clear();
  }

  bool isValid() const
  {
    return m_id != INVALID_PRODUCT_ID;
  }

  static QString columnName(ProductTableIndex idx)
  {
    QString str;
    switch (idx)
    {
      case ProductTableIndex::ID:
        str = "_ID"; break;
      case ProductTableIndex::Description:
        str = "_DESCRIPTION"; break;
      case ProductTableIndex::Unity:
        str = "_UNITY"; break;
      case ProductTableIndex::Supplier:
        str = "_SUPPLIER"; break;
      case ProductTableIndex::Price:
        str = "_PRICE"; break;
      case ProductTableIndex::Details:
        str = "_DETAILS"; break;
      case ProductTableIndex::MidasCode:
        str = "_MIDASCODE"; break;
      case ProductTableIndex::Icon:
        str = "_ICON"; break;
    }
    return str;
  }
};

#endif // PRODUCT_H

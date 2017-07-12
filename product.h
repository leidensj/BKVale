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
};

#endif // PRODUCT_H

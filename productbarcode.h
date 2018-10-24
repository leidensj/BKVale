#ifndef PRODUCTBARCODE_H
#define PRODUCTBARCODE_H

#include "jitem.h"
#include "product.h"

struct ProductBarcode : public JItem
{
  Product m_product;
  QString m_code;

  void clear()
  {
    m_id.clear();
    m_product.clear();
    m_code.clear();
  }

  ProductBarcode()
  {
    clear();
  }

  bool operator != (const JItem& other) const
  {
    const ProductBarcode& another = dynamic_cast<const ProductBarcode&>(other);
    return
        m_product != another.m_product ||
        m_code != another.m_code;
  }

  bool operator == (const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    return m_product.m_id.isValid() && !m_code.isEmpty();
  }
};

#endif // PRODUCTBARCODE_H

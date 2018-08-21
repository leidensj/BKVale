#ifndef DISCOUNT_H
#define DISCOUNT_H

#include "jitem.h"
#include "product.h"
#include <QVector>
#include <QMetaType>

struct DiscountItem : JItem
{
  enum class Column
  {
    Ammount,
    Unity,
    Name
  };

  Product m_product;
  double m_ammount;

  QString strAmmount() const { return JItem::st_strAmmount(m_ammount); }

  void clear()
  {
    m_id = INVALID_ID;
    m_product.clear();
    m_ammount = 0.0;
  }

  DiscountItem()
  {
    clear();
  }

  bool operator !=(const JItem& other) const
  {
    const DiscountItem& another = dynamic_cast<const DiscountItem&>(other);
    return
        m_product.m_id != another.m_product.m_id ||
        m_ammount != another.m_ammount;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }
};

Q_DECLARE_METATYPE(DiscountItem)


struct Discount : public JItem
{
  enum class Type
  {
    None = 0,
    Value,
    Percentage,
    Product
  };

  QString m_code;
  Type m_type;
  QVector<DiscountItem> m_items;
  bool m_bUsed;

  Discount()
  {
    clear();
  }

  void clear()
  {
    m_id = INVALID_ID;
    m_type = Type::None;
    m_items.clear();
    m_bUsed = false;
  }

  bool operator !=(const JItem& other) const
  {
    const Discount& another = dynamic_cast<const Discount&>(other);
    return
        m_type != another.m_type ||
        m_items != another.m_items ||
        m_bUsed != another.m_bUsed;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const { return m_type != Type::None; }
};

#endif // DISCOUNT_H

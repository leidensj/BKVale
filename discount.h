#ifndef DISCOUNT_H
#define DISCOUNT_H

#include "jitem.h"
#include "product.h"
#include <QVector>
#include <QMetaType>
#include <QDate>

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
  double m_value;
  double m_percentage;
  QVector<DiscountItem> m_items;
  bool m_bRedeemed;
  bool m_bExpires;
  QDate m_dtExp;
  QString m_description;

  Discount()
  {
    clear();
  }

  static QString strType(Type type)
  {
    switch (type)
    {
      case Type::Value: return "Valor";
      case Type::Percentage: return "Porcentagem";
      case Type::Product: return "Produto";
      case Type::None:
      default:
        return "Nenhum";
    }
  }

  QString strExpDate() const { return m_dtExp.toString("dd/MM/yyyy"); }

  static QString getRandomCode(const int length = 10)
  {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    QString randomString;
    for(int i = 0; i < length; ++i)
    {
      int idx = qrand() % possibleCharacters.length();
      QChar nextChar = possibleCharacters.at(idx);
      randomString.append(nextChar);
    }
    return randomString;
  }

  void clear()
  {
    m_id = INVALID_ID;
    m_code = getRandomCode();
    m_type = Type::Value;
    m_value = 0.0;
    m_percentage = 0.0;
    m_items.clear();
    m_bRedeemed = false;
    m_bExpires = false;
    m_dtExp = QDate::currentDate();
    m_description.clear();
  }

  bool operator !=(const JItem& other) const
  {
    const Discount& another = dynamic_cast<const Discount&>(other);
    return
        m_type != another.m_type ||
        m_items != another.m_items ||
        m_bRedeemed != another.m_bRedeemed ||
        m_bExpires != another.m_bExpires ||
        m_dtExp != another.m_dtExp ||
        m_value != another.m_value ||
        m_percentage != another.m_percentage ||
        m_description != another.m_description;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const { return m_type != Type::None; }
};

#endif // DISCOUNT_H

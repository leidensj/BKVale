#include "discount.h"

DiscountItem::DiscountItem()
{
  clear();
}

QString DiscountItem::strAmmount() const
{
  return JItem::st_strAmmount(m_ammount);
}

void DiscountItem::clear()
{
  m_id.clear();
  m_product.clear();
  m_ammount = 0.0;
}

bool DiscountItem::operator !=(const JItem& other) const
{
  const DiscountItem& another = dynamic_cast<const DiscountItem&>(other);
  return
      m_product.m_id != another.m_product.m_id ||
      m_ammount != another.m_ammount;
}

bool DiscountItem::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool DiscountItem::isValid() const
{
  return m_ammount >= 0;
}

QString DiscountItem::strTableName() const
{
  return DISCOUNT_ITEMS_SQL_TABLE_NAME;
}

Discount::Discount()
{
  clear();
}

QString Discount::strType(Type type)
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

QString Discount::strExpDate() const
{
  return m_dtExp.toString("dd/MM/yyyy");
}

QString Discount::getRandomCode(const int length)
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

void Discount::clear()
{
  m_id.clear();
  m_code.clear();
  m_type = Type::Value;
  m_value = 0.0;
  m_percentage = 0.0;
  m_items.clear();
  m_bRedeemed = false;
  m_bExpires = false;
  m_dtExp = QDate::currentDate();
  m_description.clear();
}

bool Discount::operator !=(const JItem& other) const
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

bool Discount::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Discount::isValid() const
{
  switch (m_type)
  {
    case Type::Percentage:
      return m_percentage >= 0.0 && m_percentage <= 100.0;
    case Type::Value:
      return m_value >= 0.0;
    case Type::Product:
      for (int i = 0; i != m_items.size(); ++i)
        if (!m_items.at(i).isValid())
          return false;
      return true;
    case Type::None:
    default:
      return false;
  }
}

QString Discount::strTableName() const
{
  return DISCOUNT_SQL_TABLE_NAME;
}

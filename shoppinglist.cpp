#include "shoppinglist.h"


ShoppingListItem::ShoppingListItem()
{
  clear();
}

void ShoppingListItem::clear()
{
  m_id.clear();
  m_product.clear();
  m_bAmmount = true;
  m_ammount = 0.0;
  m_bPrice = true;
  m_price = 0.0;
  m_package.clear();
  m_supplier.clear();
}

QString ShoppingListItem::strTableName() const
{
  return SHOPPING_LIST_ITEMS_SQL_TABLE_NAME;
}

bool ShoppingListItem::operator !=(const JItem& other) const
{
  const ShoppingListItem& another = dynamic_cast<const ShoppingListItem&>(other);
  return m_product.m_id != another.m_product.m_id ||
         m_ammount != another.m_ammount ||
         m_price != another.m_price ||
         m_package != another.m_package ||
         m_bAmmount != another.m_bAmmount ||
         m_bPrice != another.m_bPrice ||
         m_supplier.m_id != another.m_id;
}

bool ShoppingListItem::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool ShoppingListItem::isValid() const
{
  return m_product.m_id.isValid();
}

double ShoppingListItem::subtotal() const
{
  return m_ammount * m_price;
}

QString ShoppingListItem::strSubtotal() const
{
  return st_strMoney(subtotal());
}

QString ShoppingListItem::strAmmount() const
{
  return st_strAmmount(m_ammount);
}

QString ShoppingListItem::strPrice() const
{
  return st_strMoney(m_price);
}

ShoppingList::ShoppingList()
{
  clear();
}

void ShoppingList::clear()
{
  m_id.clear();
  m_title.clear();
  m_description.clear();
  m_supplier.clear();
  m_image.clear();
  for (int i = 0; i != 7; ++i)
    m_weekDays[i] = false;
  for (int i = 0; i != 31; ++i)
    m_monthDays[i] = false;
  m_vItem.clear();
  m_nLines = 0;
}

bool ShoppingList::isValid() const
{
  bool b = true;
  for (int i = 0; i != m_vItem.size(); ++i)
    b &= m_vItem.at(i).isValid();
  return b;
}

QString ShoppingList::strTableName() const
{
  return SHOPPING_LIST_SQL_TABLE_NAME;
}

void ShoppingList::setWeekDays(const QString& strWeekDays)
{
  for (int i = 0; i != 7; ++i)
    m_weekDays[i] = false;
  QStringList lst = strWeekDays.split(SHOPPING_LIST_SEPARATOR);
  for (int i = 0; i != lst.size(); ++i)
  {
    int day = lst.at(i).toInt();
    if (day > 0 && day < 8)
      m_weekDays[day - 1] = true;
  }
}

void ShoppingList::setMonthDays(const QString& strMonthDays)
{
  for (int i = 0; i != 7; ++i)
    m_monthDays[i] = false;
  QStringList lst = strMonthDays.split(SHOPPING_LIST_SEPARATOR);
  for (int i = 0; i != lst.size(); ++i)
  {
    int day = lst.at(i).toInt();
    if (day > 0 && day < 32)
      m_monthDays[day - 1] = true;
  }
}

QString ShoppingList::getWeekDays() const
{
  QString strWeekDays;
  for (int i = 0; i != 7; ++i)
    if (m_weekDays[i])
      strWeekDays += SHOPPING_LIST_SEPARATOR + QString::number(i + 1);
  if (!strWeekDays.isEmpty())
    strWeekDays += SHOPPING_LIST_SEPARATOR;
  return strWeekDays;
}

QString ShoppingList::getMonthDays() const
{
  QString strMonthDays;
  for (int i = 0; i != 31; ++i)
    if (m_monthDays[i])
      strMonthDays += SHOPPING_LIST_SEPARATOR + QString::number(i + 1);
  if (!strMonthDays.isEmpty())
    strMonthDays += SHOPPING_LIST_SEPARATOR;
  return strMonthDays;
}

bool ShoppingList::operator !=(const JItem& other) const
{
  const ShoppingList& another = dynamic_cast<const ShoppingList&>(other);
  bool b = m_supplier.m_id != another.m_supplier.m_id ||
           m_description != another.m_description ||
           m_nLines != another.m_nLines ||
           m_vItem != another.m_vItem;
  if (!b)
  {
    for (int i = 0; i != 7; ++i)
    {
      b |= m_weekDays[i] != another.m_weekDays[i];
      if (b)
        break;
    }

    if (!b)
      for (int i = 0; i != 31; ++i)
      {
        b |= m_monthDays[i] != another.m_monthDays[i];
        if (b)
          break;
      }
  }

  return b;
}

bool ShoppingList::operator ==(const JItem& other) const
{
  return !(*this != other);
}

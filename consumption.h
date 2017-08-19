#ifndef CONSUMPTION_H
#define CONSUMPTION_H

#include <QString>
#include <item.h>

#define INVALID_CONSUMPTION_ID   -1

enum class ConsumptionTableIndex
{
  ID = 0,
  ItemID,
  Price,
  Ammount
};

struct Consumption
{
  Consumption()
    : m_id(INVALID_ITEM_ID)
  {
    clear();
  }

  int m_id;
  qint64 m_date;
  int m_itemID;
  double m_price;
  double m_ammount;

  void clear()
  {
    m_id = INVALID_CONSUMPTION_ID;
    m_date = 0;
    m_itemID = INVALID_CONSUMPTION_ID;
    m_price = 0.0;
    m_ammount = 0.0;
  }

  bool isValid() const
  {
    return m_id != INVALID_CONSUMPTION_ID;
  }

  static QString columnName(ConsumptionTableIndex idx)
  {
    QString str;
    switch (idx)
    {
      case ConsumptionTableIndex::ID:
        str = "_ID"; break;
      case ConsumptionTableIndex::ItemID:
        str = "_ITEMID"; break;
      case ConsumptionTableIndex::Price:
        str = "_PRICE"; break;
      case ConsumptionTableIndex::Ammount:
        str = "_AMMOUNT"; break;
    }
    return str;
  }
};

#endif // CONSUMPTION_H

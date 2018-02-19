#ifndef CONSUMPTION_H
#define CONSUMPTION_H

#include <QString>
#include <product.h>

#define INVALID_CONSUMPTION_ID   -1

enum class ConsumptionTableIndex
{
  ID = 0,
  Date,
  ItemID,
  Price,
  Ammount,
  Total
};

struct Consumption
{
  Consumption()
    : m_id(INVALID_PRODUCT_ID)
  {
    clear();
  }

  int m_id;
  qint64 m_date;
  int m_itemID;
  double m_price;
  double m_ammount;
  double m_total;

  void clear()
  {
    m_id = INVALID_CONSUMPTION_ID;
    m_date = 0;
    m_itemID = INVALID_CONSUMPTION_ID;
    m_price = 0.0;
    m_ammount = 0.0;
    m_total = 0.0;
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
        str = "_ID";
        break;
      case ConsumptionTableIndex::ItemID:
        str = "_ITEMID";
        break;
      case ConsumptionTableIndex::Date:
        str = "_DATE";
        break;
      case ConsumptionTableIndex::Price:
        str = "_PRICE";
        break;
      case ConsumptionTableIndex::Ammount:
        str = "_AMMOUNT";
        break;
        case ConsumptionTableIndex::Total:
        str = "_TOTAL";
        break;
      default:
        break;
    }
    return str;
  }

  struct Filter
  {
    Filter()
      : m_bDate(false)
      , m_datei(0)
      , m_datef(0)
    {

    }

    bool m_bDate;
    qint64 m_datei;
    qint64 m_datef;
  };
};

#endif // CONSUMPTION_H

#ifndef ITEM_H
#define ITEM_H

#include <QString>

#define INVALID_ITEM_ID   -1
#define INVALID_ITEM_ICON -1

enum class ItemTableIndex
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

struct Item
{
  Item()
    : m_id(INVALID_ITEM_ID)
    , m_icon(INVALID_ITEM_ICON)
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
    m_id = INVALID_ITEM_ID;
    m_icon = INVALID_ITEM_ICON;
    m_description.clear();
    m_unity.clear();
    m_supplier.clear();
    m_price = 0.0;
    m_details.clear();
    m_midasCode.clear();
  }

  bool isValid() const
  {
    return m_id != INVALID_ITEM_ID;
  }

  static QString columnName(ItemTableIndex idx)
  {
    QString str;
    switch (idx)
    {
      case ItemTableIndex::ID:
        str = "_ID"; break;
      case ItemTableIndex::Description:
        str = "_DESCRIPTION"; break;
      case ItemTableIndex::Unity:
        str = "_UNITY"; break;
      case ItemTableIndex::Supplier:
        str = "_SUPPLIER"; break;
      case ItemTableIndex::Price:
        str = "_PRICE"; break;
      case ItemTableIndex::Details:
        str = "_DETAILS"; break;
      case ItemTableIndex::MidasCode:
        str = "_MIDASCODE"; break;
      case ItemTableIndex::Icon:
        str = "_ICON"; break;
    }
    return str;
  }
};

#endif // ITEM_H

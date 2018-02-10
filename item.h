#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include "category.h"

#define INVALID_ITEM_ID   -1
#define INVALID_ITEM_ICON -1

#define MAX_ITEM_NAME_LENGTH            35
#define MAX_ITEM_UNITY_LENGTH           4
#define MAX_ITEM_PACKAGE_UNITY_LENGTH   4
#define MAX_ITEM_PACKAGE_AMMOUNT_LENGTH 9999.99
#define MAX_ITEM_DETAILS_LENGTH         35
#define MAX_ITEM_MIDASCODE_LENGTH       35

enum class ItemTableIndex
{
  ID = 0,
  Name,
  Unity,
  PackageUnity,
  PackageAmmount,
  Details,
  Code,
  Icon,
  AvailableAtNotes,
  AvailableAtShop,
  AvailableAtConsumption
};

struct Item
{
  Item()
    : m_id(INVALID_ITEM_ID)
    , m_categoryId(INVALID_CATEGORY_ID)
    , m_packageAmmount(0.0)
    , m_icon(INVALID_ITEM_ICON)
    , m_bAvailableAtNotes(false)
    , m_bAvailableAtShop(false)
    , m_bAvailableAtConsumption(false)
  {

  }

  mutable int m_id;
  int m_categoryId;
  QString m_name;
  QString m_unity;
  QString m_packageUnity;
  double m_packageAmmount;
  QString m_details;
  QString m_code;
  int m_icon;
  bool m_bAvailableAtNotes;
  bool m_bAvailableAtShop;
  bool m_bAvailableAtConsumption;

  void clear()
  {
    m_id = INVALID_ITEM_ID;
    m_icon = INVALID_ITEM_ICON;
    m_categoryId= INVALID_CATEGORY_ID;
    m_name.clear();
    m_unity.clear();
    m_packageUnity.clear();
    m_packageAmmount = 0.0;
    m_details.clear();
    m_code.clear();
    m_bAvailableAtNotes = false;
    m_bAvailableAtShop = false;
    m_bAvailableAtConsumption = false;
  }

  bool isValidId() const
  {
    return m_id != INVALID_ITEM_ID;
  }

  static QString columnName(ItemTableIndex idx)
  {
    switch (idx)
    {
      case ItemTableIndex::ID:
        return "_ID";
      case ItemTableIndex::Name:
        return "_NAME";
      case ItemTableIndex::Unity:
        return "_UNITY";
      case ItemTableIndex::PackageUnity:
        return "_PACKAGE_UNITY";
      case ItemTableIndex::PackageAmmount:
        return "_PACKAGE_AMMOUNT";
      case ItemTableIndex::Details:
        return "_DETAILS";
      case ItemTableIndex::Code:
        return "_CODE";
      case ItemTableIndex::Icon:
        return "_ICON";
      case ItemTableIndex::AvailableAtNotes:
        return "_AVAILABLE_AT_NOTES";
      case ItemTableIndex::AvailableAtShop:
        return "_AVAILABLE_AT_SHOP";
      case ItemTableIndex::AvailableAtConsumption:
        return "_AVAILABLE_AT_CONSUMPTION";
      default:
        return "";
    }
  }
};

#endif // ITEM_H

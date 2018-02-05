#ifndef ITEM_H
#define ITEM_H

#include <QString>

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
  QString m_name;
  QString m_unity;
  QString m_packageUnity;
  double m_packageAmmount;
  QString m_details;
  QString m_midasCode;
  int m_icon;

  void clear()
  {
    m_id = INVALID_ITEM_ID;
    m_icon = INVALID_ITEM_ICON;
    m_name.clear();
    m_unity.clear();
    m_packageUnity.clear();
    m_packageAmmount = 0.0;
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
      case ItemTableIndex::Name:
        str = "_NAME"; break;
      case ItemTableIndex::Unity:
        str = "_UNITY"; break;
      case ItemTableIndex::PackageUnity:
        str = "_PACKAGE_UNITY"; break;
      case ItemTableIndex::PackageAmmount:
        str = "_PACKAGE_AMMOUNT"; break;
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

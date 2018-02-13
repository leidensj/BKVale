#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include "category.h"

#define INVALID_ITEM_ID   -1

#define MAX_ITEM_NAME_LENGTH            35
#define MAX_ITEM_UNITY_LENGTH           4
#define MAX_ITEM_PACKAGE_UNITY_LENGTH   4
#define MAX_ITEM_PACKAGE_AMMOUNT_LENGTH 9999.99
#define MAX_ITEM_DETAILS_LENGTH         35
#define MAX_ITEM_MIDASCODE_LENGTH       35

struct Item
{
  mutable int m_id;
  int m_categoryId;
  int m_imageId;
  QString m_name;
  QString m_unity;
  QString m_packageUnity;
  double m_packageAmmount;
  QString m_details;
  QString m_code;
  bool m_bAvailableAtNotes;
  bool m_bAvailableAtShop;
  bool m_bAvailableAtConsumption;

  void clear()
  {
    m_id = INVALID_ITEM_ID;
    m_imageId = INVALID_IMAGE_ID;
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

  Item()
  {
    clear();
  }

  static bool st_isValidId(int id) { return id != INVALID_ITEM_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
};

#endif // ITEM_H

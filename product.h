#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <QString>
#include <QVector>
#include "category.h"
#include "jtablecolumn.h"

#define INVALID_PRODUCT_ID   -1

#define MAX_PRODUCT_NAME_LENGTH            35
#define MAX_PRODUCT_UNITY_LENGTH           4
#define MAX_PRODUCT_PACKAGE_UNITY_LENGTH   4
#define MAX_PRODUCT_PACKAGE_AMMOUNT_LENGTH 9999.99
#define MAX_PRODUCT_DETAILS_LENGTH         35
#define MAX_PRODUCT_MIDASCODE_LENGTH       35

struct Product
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
  bool m_bAvailableToBuy;
  bool m_bAvailableToSell;

  void clear()
  {
    m_id = INVALID_PRODUCT_ID;
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
    m_bAvailableToBuy = false;
    m_bAvailableToSell = false;
  }

  Product()
  {
    clear();
  }

  bool operator !=(const Product& other)
  {
    return
        m_imageId != other.m_imageId ||
        m_categoryId != other.m_categoryId ||
        m_name != other.m_name ||
        m_unity != other.m_unity ||
        m_packageUnity != other.m_packageUnity ||
        m_packageAmmount != other.m_packageAmmount ||
        m_details != other.m_details ||
        m_code != other.m_code ||
        m_bAvailableAtNotes != other.m_bAvailableAtNotes ||
        m_bAvailableAtShop != other.m_bAvailableAtShop ||
        m_bAvailableAtConsumption != other.m_bAvailableAtConsumption ||
        m_bAvailableToBuy != other.m_bAvailableToBuy ||
        m_bAvailableToSell != other.m_bAvailableToSell;
  }

  static bool st_isValid(const Product& product)
  {
    return
        !product.m_name.isEmpty() &&
        !product.m_unity.isEmpty();
  }

  bool isValid() const { return st_isValid(*this); }
  static bool st_isValidId(int id) { return id != INVALID_PRODUCT_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
  static QString getTableName() { return "_PRODUCTS"; }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn("_ID", QObject::tr("Id")));
    c.push_back(JTableColumn("_NAME", QObject::tr("Nome"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn("_CATEGORYID", QObject::tr("Categoria")));
    c.push_back(JTableColumn("_IMAGEID", QObject::tr("Imagem")));
    c.push_back(JTableColumn("_UNITY", QObject::tr("Unidade"), false));
    c.push_back(JTableColumn("_PACKAGE_UNITY", QObject::tr("Unidade da Embalagem")));
    c.push_back(JTableColumn("_PACKAGE_AMMOUNT", QObject::tr("Quantidade da Embalagem")));
    c.push_back(JTableColumn("_DETAILS", QObject::tr("Detalhes"), false, false, JResizeMode::Stretch));
    c.push_back(JTableColumn("_CODE", QObject::tr("Código")));
    c.push_back(JTableColumn("_AVAILABLE_AT_NOTES", QObject::tr("Notas")));
    c.push_back(JTableColumn("_AVAILABLE_AT_SHOP", QObject::tr("Compras")));
    c.push_back(JTableColumn("_AVAILABLE_AT_CONSUMPTION", QObject::tr("Consumo")));
    c.push_back(JTableColumn("_AVAILABLE_TO_BUY", QObject::tr("Compra")));
    c.push_back(JTableColumn("_AVAILABLE_TO_SELL", QObject::tr("Venda")));
    return c;
  }
};

#endif // PRODUCT_H

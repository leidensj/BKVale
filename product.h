#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jitem.h"
#include "defines.h"
#include "category.h"

struct Product : public JItem
{
  Category m_category;
  Image m_image;
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
    m_id = INVALID_ID;
    m_category.clear();
    m_image.clear();
    m_name.clear();
    m_unity.clear();
    m_packageUnity.clear();
    m_packageAmmount = 1.0;
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

  bool operator != (const JItem& other) const
  {
    const Product& another = dynamic_cast<const Product&>(other);
    return
        m_image.m_id != another.m_image.m_id ||
        m_category.m_id != another.m_category.m_id ||
        m_name != another.m_name ||
        m_unity != another.m_unity ||
        m_packageUnity != another.m_packageUnity ||
        m_packageAmmount != another.m_packageAmmount ||
        m_details != another.m_details ||
        m_code != another.m_code ||
        m_bAvailableAtNotes != another.m_bAvailableAtNotes ||
        m_bAvailableAtShop != another.m_bAvailableAtShop ||
        m_bAvailableAtConsumption != another.m_bAvailableAtConsumption ||
        m_bAvailableToBuy != another.m_bAvailableToBuy ||
        m_bAvailableToSell != another.m_bAvailableToSell;
  }

  bool operator == (const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    return
        !m_name.isEmpty() &&
        !m_unity.isEmpty();
  }

  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(PRODUCT_SQL_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL01, QObject::tr("Nome"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn(PRODUCT_SQL_COL02, QObject::tr("Categoria")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL03, QObject::tr("Imagem")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL04, QObject::tr("Unidade"), false));
    c.push_back(JTableColumn(PRODUCT_SQL_COL05, QObject::tr("Unidade da Embalagem")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL06, QObject::tr("Quantidade da Embalagem")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL07, QObject::tr("Detalhes"), false, false, JResizeMode::Stretch));
    c.push_back(JTableColumn(PRODUCT_SQL_COL08, QObject::tr("Código")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL09, QObject::tr("Notas")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL10, QObject::tr("Compras")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL11, QObject::tr("Consumo")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL12, QObject::tr("Compra")));
    c.push_back(JTableColumn(PRODUCT_SQL_COL13, QObject::tr("Venda")));
    return c;
  }
};

#endif // PRODUCT_H

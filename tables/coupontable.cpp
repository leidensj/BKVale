#include "coupontable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "controls/databaseselector.h"
#include "tableitems/doubleitem.h"
#include "tableitems/sqlitem.h"

CouponTable::CouponTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(2);
  QStringList headers;
  headers << "Quantidade" << "Produto";
  setHorizontalHeaderLabels(headers);
  setHeaderIcon((int)Column::Product, QIcon(":/icons/res/item.png"));

  horizontalHeader()->setSectionResizeMode((int)Column::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Product, QHeaderView::Stretch);
}

void CouponTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itAmmount = new DoubleItem(Data::Type::Ammount, DoubleItem::Color::Background);
  auto itProduct = new SQLItem(PRODUCT_SQL_TABLE_NAME, PRODUCT_FILTER_SELL);

  blockSignals(true);
  setItem(row, (int)Column::Ammount, itAmmount);
  setItem(row, (int)Column::Product, itProduct);
  blockSignals(false);

  setCurrentItem(itAmmount);
  setFocus();
}

void CouponTable::addRowAndActivate()
{
  addRow();
  getItem(rowCount() - 1, (int)Column::Product)->activate();
  if (!SQLItem::st_idFromVariant(getItem(rowCount() - 1, (int)Column::Product)->getValue()).isValid())
    removeItem();
}

void CouponTable::get(QVector<CouponProduct>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    CouponProduct o;
    o.m_ammount = getItem(row, (int)Column::Ammount)->getValue().toDouble();
    o.m_product.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Product)->getValue());
    o.m_product.m_name = SQLItem::st_nameFromVariant(getItem(row, (int)Column::Product)->getValue());
    v.push_back(o);
  }
}

void CouponTable::set(const QVector<CouponProduct>& v, bool bClear)
{
  if (bClear)
    removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    getItem(row, (int)Column::Product)->setValue(SQLItem::st_toVariant(v.at(i).m_product));
    getItem(row, (int)Column::Ammount)->setValue(v.at(i).m_ammount);
  }
}

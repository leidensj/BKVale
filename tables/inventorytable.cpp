#include "inventorytable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "controls/databaseselector.h"
#include "tableitems/doubleitem.h"
#include "tableitems/sqlitem.h"

InventoryTable::InventoryTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(4);
  QStringList headers;
  headers << tr("Produto") << tr("Quantidade") << tr("Ajuste") <<tr("Final");
  setHorizontalHeaderLabels(headers);
  setHeaderIcon((int)Column::Product, QIcon(":/icons/res/item.png"));

  horizontalHeader()->setSectionResizeMode((int)Column::Product, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Adjustment, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Final, QHeaderView::ResizeToContents);
}

void InventoryTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itProduct = new SQLItem(PRODUCT_SQL_TABLE_NAME, PRODUCT_FILTER_SELL);
  auto itAmmount = new DoubleItem(Data::Type::Ammount, DoubleItem::Color::Background);
  auto itAdjustment = new DoubleItem(Data::Type::Ammount, DoubleItem::Color::Background);
  auto itFinal = new DoubleItem(Data::Type::Ammount, DoubleItem::Color::Background);
  itProduct->setReadOnly(true);
  itAmmount->setReadOnly(true);
  itFinal->setReadOnly(true);

  blockSignals(true);
  setItem(row, (int)Column::Product, itProduct);
  setItem(row, (int)Column::Ammount, itAmmount);
  setItem(row, (int)Column::Adjustment, itAdjustment);
  setItem(row, (int)Column::Final, itFinal);
  blockSignals(false);

  setCurrentItem(itAdjustment);
  setFocus();
}

void InventoryTable::get(QVector<InventoryProduct>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    InventoryProduct o;
    o.m_ammount = getItem(row, (int)Column::Ammount)->getValue().toDouble();
    o.m_product.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Product)->getValue());
    o.m_product.m_name = SQLItem::st_nameFromVariant(getItem(row, (int)Column::Product)->getValue());
    v.push_back(o);
  }
}

void InventoryTable::set(const QVector<InventoryProduct>& v, bool bClear)
{
  if (bClear)
    removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    getItem(row, (int)Column::Product)->setValue(SQLItem::st_toVariant(v.at(i).m_product));
    getItem(row, (int)Column::Ammount)->setValue(v.at(i).m_ammount);
    getItem(row, (int)Column::Adjustment)->setValue(v.at(i).m_ammount);
    getItem(row, (int)Column::Ammount)->setValue(v.at(i).m_ammount);
  }
}

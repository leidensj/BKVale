#include "productcodetable.h"
#include <QHeaderView>
#include "tableitems/textitem.h"

ProductCodeTable::ProductCodeTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(1);
  QStringList headers;
  headers << "Código";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Code, QHeaderView::Stretch);
}

void ProductCodeTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto it = new TextItem(Text::Input::ASCII, true);

  blockSignals(true);
  setItem(row, (int)Column::Code, it);
  blockSignals(false);

  setCurrentItem(it);
  setFocus();
}

void ProductCodeTable::getCodes(QVector<ProductCode>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    ProductCode o;
    o.m_code = getItem(row, (int)Column::Code)->getValue().toString();
    v.push_back(o);
  }
}

void ProductCodeTable::setCodes(const QVector<ProductCode>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    getItem(i, (int)Column::Code)->setValue(v.at(i).m_code);
  }
}

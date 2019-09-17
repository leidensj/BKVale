#include "productcodetable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "widgets/jtablewidgetitem.h"

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
  setItem(row, (int)Column::Code, it);

  setCurrentItem(it);
  setFocus();
}

void ProductCodeTable::getPhones(QVector<ProductCode>& v) const
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

void ProductCodeTable::setPhones(const QVector<ProductCode>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    getItem(i, (int)Column::Code)->setValue(v.at(i).m_code);
  }
}

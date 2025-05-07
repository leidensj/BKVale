#include "cashinfotable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/textitem.h"
#include "tableitems/multitoggleitem.h"

CashInfoTable::CashInfoTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(2);
  QStringList headers;
  headers << "Nome" << "Tipo";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Name, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Type, QHeaderView::ResizeToContents);
}

void CashInfoTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itName = new TextItem(Text::Input::ASCII, true);
  auto itType = new MultiToggleItem(QStringList() << "Inteiro" << "Dinheiro" << "Quantidade" << "Porcentagem" << "Texto");

  blockSignals(true);
  setItem(row, (int)Column::Name, itName);
  setItem(row, (int)Column::Type, itType);
  blockSignals(false);

  setCurrentItem(itName);
  setFocus();
}

void CashInfoTable::addRowAndActivate()
{
  addRow();
}

void CashInfoTable::get(QVector<CashInfo>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    CashInfo o;
    o.m_name = getItem(row, (int)Column::Name)->getValue().toString();
    o.m_type = (Data::Type)getItem(row, (int)Column::Type)->getValue().toInt();
    v.push_back(o);
  }
}

void CashInfoTable::set(const QVector<CashInfo>& v, bool bClear)
{
  if (bClear)
    removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    getItem(row, (int)Column::Name)->setValue(v.at(i).m_name);
    getItem(row, (int)Column::Type)->setValue((int)v.at(i).m_type);
  }
}

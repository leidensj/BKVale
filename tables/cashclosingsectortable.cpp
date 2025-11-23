#include "cashclosingsectortable.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/textitem.h"
#include "tableitems/doubleitem.h"

CashClosingSectorTable::CashClosingSectorTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(3);
  QStringList headers;
  headers << "Setor" << "Valor" << "Quantidade";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->hideSection((int)Column::NValue);
  horizontalHeader()->setSectionResizeMode((int)Column::Sector, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Value, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::NValue, QHeaderView::Stretch);
}

void CashClosingSectorTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itSector = new TextItem(Text::Input::AlphanumericAndSpaces, true);
  auto itValue = new DoubleItem(Data::Type::Money, DoubleItem::Color::Foreground, false, false);
  auto itNValue = new DoubleItem(Data::Type::Integer, DoubleItem::Color::Foreground, false, false);

  blockSignals(true);
  setItem(row, (int)Column::Sector, itSector);
  setItem(row, (int)Column::Value, itValue);
  setItem(row, (int)Column::NValue, itNValue);
  blockSignals(false);

  setCurrentItem(itValue);
  setFocus();
}

void CashClosingSectorTable::addRowAndActivate()
{
  addRow();
}

void CashClosingSectorTable::get(QVector<CashClosingSector>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    CashClosingSector o;
    o.m_sname = getItem(row, (int)Column::Sector)->getValue().toString();
    o.m_simage.m_id.set(getItem(row, (int)Column::Sector)->data(101).toLongLong());
    o.m_value = getItem(row, (int)Column::Value)->getValue().toDouble();
    o.m_nvalue = getItem(row, (int)Column::NValue)->getValue().toInt();
    v.push_back(o);
  }
}

void CashClosingSectorTable::set(const QVector<CashClosingSector>& v, bool bClear)
{
  if (bClear)
    removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    auto its= getItem(row, (int)Column::Sector);
    its->setReadOnly(true);
    its->setValue(v.at(i).m_sname);
    its->setIcon(v.at(i).m_simage.toIcon());
    its->setData(101, v.at(i).m_simage.m_id.get());
    getItem(row, (int)Column::Value)->setValue(v.at(i).m_value);
    getItem(row, (int)Column::NValue)->setValue(v.at(i).m_nvalue);
  }
}

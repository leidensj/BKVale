#include "cashclosingcointable.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/textitem.h"
#include "tableitems/doubleitem.h"

CashClosingCoinTable::CashClosingCoinTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(3);
  QStringList headers;
  headers << "Moeda" << "Taxa" << "Valor";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Coin, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Tax, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Value, QHeaderView::Stretch);
}

void CashClosingCoinTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itCoin = new TextItem(Text::Input::AlphanumericAndSpaces, true);
  auto itTax = new DoubleItem(Data::Type::Percentage, DoubleItem::Color::Foreground);
  auto itValue = new DoubleItem(Data::Type::Money, DoubleItem::Color::Foreground2, false, false);

  blockSignals(true);
  setItem(row, (int)Column::Coin, itCoin);
  setItem(row, (int)Column::Tax, itTax);
  setItem(row, (int)Column::Value, itValue);
  blockSignals(false);

  setCurrentItem(itValue);
  setFocus();
}

void CashClosingCoinTable::addRowAndActivate()
{
  addRow();
}

void CashClosingCoinTable::get(QVector<CashClosingCoin>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    CashClosingCoin o;
    o.m_cname = getItem(row, (int)Column::Coin)->getValue().toString();
    o.m_ctax = getItem(row, (int)Column::Tax)->getValue().toDouble();
    o.m_cimage.m_id.set(getItem(row, (int)Column::Coin)->data(101).toLongLong());
    o.m_value = getItem(row, (int)Column::Value)->getValue().toDouble();
    v.push_back(o);
  }
}

void CashClosingCoinTable::set(const QVector<CashClosingCoin>& v, bool bClear)
{
  if (bClear)
    removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    auto itc = getItem(row, (int)Column::Coin);
    itc->setReadOnly(true);
    itc->setValue(v.at(i).m_cname);
    itc->setIcon(v.at(i).m_cimage.toIcon());
    itc->setData(101, v.at(i).m_cimage.m_id.get());
    auto itt = getItem(row, (int)Column::Tax);
    itt->setReadOnly(true);
    itt->setValue(v.at(i).m_ctax);
    getItem(row, (int)Column::Value)->setValue(v.at(i).m_value);
  }
}

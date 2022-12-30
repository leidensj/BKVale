#include "dayofftable.h"
#include <QHeaderView>
#include "tableitems/toggleitem.h"

DayOffTable::DayOffTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(0);
}

void DayOffTable::setStore(const Store& o, const QDate& date)
{
  m_store = o;
  m_date = date;
  update();
}

void DayOffTable::update()
{
  setColumnCount(m_date.daysInMonth());
  QStringList headers;
  for (int i = 0; i != m_date.daysInMonth(); ++i)
    headers << QString::number(i + 1) + " " + m_date.addDays(i).toString("dddd");;
  setHorizontalHeaderLabels(headers);
  headers.clear();
  for (int i = 0; i != m_store.m_vEmployee.size(); ++i)
  {
    addRow();
    headers << m_store.m_vEmployee.at(i).name();
  }
  setVerticalHeaderLabels(headers);
}

void DayOffTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  blockSignals(true);
  for (int i = 0; i != columnCount(); ++i)
  {
    auto it = new ToggleItem(ToggleItem::Color::Background);
    setItem(row, i, it);
    setCurrentItem(it);
  }
  blockSignals(false);
  setFocus();
}

void DayOffTable::getDayOff(QVector<QVector<bool>>& v) const
{
  v.clear();
  /*for (int j = 0; j != columnCount(); ++j)
  {
    QVector<bool> d;
    for (int i = 0; i != rowCount(); ++i)
    {
      int row = verticalHeader()->logicalIndex(i);
      d.push_back(getItem(row, j)->getValue().toBool());
    }
    v.push_back(d);
  }*/
}

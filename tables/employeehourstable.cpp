#include "employeehourstable.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/timeitem.h"

EmployeeHoursTable::EmployeeHoursTable(JAddRemoveButtons* btns, bool bSelector, QWidget* parent)
  : JTable(btns, bSelector, parent)
{
  setColumnCount(2);
  QStringList headers;
  headers << "Início" << "Fim";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Begin, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::End, QHeaderView::Stretch);
}

void EmployeeHoursTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itBegin = new TimeItem(QTime::fromString("00:00"));
  auto itEnd = new TimeItem(QTime::fromString("00:00"));

  blockSignals(true);
  setItem(row, (int)Column::Begin, itBegin);
  setItem(row, (int)Column::End, itEnd);
  blockSignals(false);

  setCurrentItem(itBegin);
  setFocus();
}

void EmployeeHoursTable::getHours(QVector<TimeInterval>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    TimeInterval o;
    o.m_tmBegin = getItem(row, (int)Column::Begin)->getValue().toTime();
    o.m_tmEnd = getItem(row, (int)Column::End)->getValue().toTime();
    v.push_back(o);
  }
}

void EmployeeHoursTable::setHours(const QVector<TimeInterval>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    getItem(i, (int)Column::Begin)->setValue(v.at(i).m_tmBegin);
    getItem(i, (int)Column::End)->setValue(v.at(i).m_tmEnd);
  }
}

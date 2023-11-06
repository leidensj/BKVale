#include "timecardcontroltable.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/timeitem.h"

TimeCardControlTable::TimeCardControlTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(8);
  QStringList headers;
  headers << "Início 1" << "Fim 1" << "Início 2" << "Fim 2" << "Início 3" << "Fim 3" << "Início 4" << "Fim 4";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Begin1, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::End1, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Begin2, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::End2, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Begin3, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::End3, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Begin4, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::End4, QHeaderView::Stretch);
}

void TimeCardControlTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itBegin1 = new TimeItem(QTime::fromString("00:00"));
  auto itEnd1 = new TimeItem(QTime::fromString("00:00"));
  auto itBegin2 = new TimeItem(QTime::fromString("00:00"));
  auto itEnd2 = new TimeItem(QTime::fromString("00:00"));
  auto itBegin3 = new TimeItem(QTime::fromString("00:00"));
  auto itEnd3 = new TimeItem(QTime::fromString("00:00"));
  auto itBegin4 = new TimeItem(QTime::fromString("00:00"));
  auto itEnd4 = new TimeItem(QTime::fromString("00:00"));

  blockSignals(true);
  setItem(row, (int)Column::Begin1, itBegin1);
  setItem(row, (int)Column::End1, itEnd1);
  setItem(row, (int)Column::Begin2, itBegin2);
  setItem(row, (int)Column::End2, itEnd2);
  setItem(row, (int)Column::Begin3, itBegin3);
  setItem(row, (int)Column::End3, itEnd3);
  setItem(row, (int)Column::Begin4, itBegin4);
  setItem(row, (int)Column::End4, itEnd4);
  blockSignals(false);

  setCurrentItem(itBegin1);
  setFocus();
}

void TimeCardControlTable::getHours(QVector<TimeInterval>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    TimeInterval o;
    for (int ii = 0; ii != 8; ++ii)
    {
      o.m_tmBegin = getItem(row, ii)->getValue().toTime();
      o.m_tmEnd = getItem(row, ii)->getValue().toTime();
      v.push_back(o);
    }
  }
}

void TimeCardControlTable::setHours(const QVector<TimeInterval>& v)
{
  removeAllItems();
  for (int i = 0; i < v.size(); i = i + 8)
  {
    addRow();
    for (int ii = 0; ii != 8; ++ii)
    {
      if (i + ii < v.size())
      {
        getItem(i, ii)->setValue(v.at(i + ii).m_tmBegin);
        getItem(i, ii)->setValue(v.at(i + ii).m_tmEnd);
      }
    }
  }
}

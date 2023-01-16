#include "dayofftable.h"
#include <QHeaderView>
#include <QRandomGenerator>
#include "tableitems/toggleitem.h"

DayOffTable::DayOffTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  verticalHeader()->setSectionsMovable(false);
  setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void DayOffTable::setStore(const Store& o, const QDate& date)
{
  m_store = o;
  m_date.setDate(date.year(), date.month(), 1);
  if (o.m_id.isValid())
    update();
  else
    clearAll();
}

void DayOffTable::update()
{
  setColumnCount(0);
  setRowCount(0);
  setColumnCount(m_date.daysInMonth());
  QStringList headers;
  for (int i = 0; i != m_date.daysInMonth(); ++i)
    headers << QString::number(i + 1) + " " + m_date.addDays(i).toString("dddd");
  setHorizontalHeaderLabels(headers);
  headers.clear();
  for (int i = 0; i != m_store.m_vEmployee.size(); ++i)
  {
    addRow();
    headers << m_store.m_vEmployee.at(i).firstName();
  }
  setVerticalHeaderLabels(headers);
  for (int i = 0; i != rowCount(); ++i)
    verticalHeaderItem(i)->setToolTip(m_store.m_vEmployee.at(i).name());
  setCurrentItem(item(0, 0));
}

void DayOffTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  blockSignals(true);
  for (int i = 0; i != columnCount(); ++i)
  {
    auto it = new ToggleItem(tr("FOLGA"));
    setItem(row, i, it);
    setCurrentItem(it);
  }
  blockSignals(false);
  setFocus();
}

void DayOffTable::getDayOff(QVector<QVector<bool>>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    QVector<bool> days;
    for (int j = 0; j != columnCount(); ++j)
      days.push_back(dynamic_cast<JTableItem*>(item(i, j))->getValue().toBool());
    v.push_back(days);
  }
}

QString DayOffTable::toCSV() const
{
  QString csv, line = tr("FUNCIONÁRIO/DIA;");
  for (int j = 0; j != columnCount(); ++j)
      line += QString::number(j + 1) + ";";
  line.chop(1);
  csv += line + "\n";
  for (int i = 0; i != rowCount(); ++i)
  {
    line.clear();
    line += verticalHeaderItem(i)->toolTip() + ";";
    for (int j = 0; j != columnCount(); ++j)
      line += dynamic_cast<JTableItem*>(item(i, j))->getValue().toBool() ? "F;" : ";";
    if (!line.isEmpty())
    {
      line.chop(1);
      line.replace("\n", "\t");
      csv += line + "\n";
    }
  }

  return csv;
}

QString DayOffTable::message() const
{
  auto pt = currentItem();
  if (pt == nullptr)
    return "";
  QString employee = verticalHeaderItem(pt->row())->toolTip();
  int nSundays = 0, nDayOff = 0;
  for (int j = 0; j != columnCount(); ++j)
  {
    if (dynamic_cast<JTableItem*>(item(pt->row(), j))->getValue().toBool())
    {
       nDayOff++;
       if (m_date.addDays(j).dayOfWeek() == Qt::Sunday)
         nSundays++;
    }
  }

  QString day = horizontalHeaderItem(pt->column())->text();
  int nTotal = 0;
  for (int i = 0; i != rowCount(); ++i)
  {
    if (dynamic_cast<JTableItem*>(item(i, pt->column()))->getValue().toBool())
      nTotal++;
  }

  QString msg1, msg2;
  if (nDayOff == 0)
    msg1 =  employee + tr(" não possui folgas.");
  else if (nSundays == 0)
    msg1 = employee + tr(" | %1 folga(s)").arg(QString::number(nDayOff));
  else
    msg1 = employee + tr(" | %1 folga(s) | %2 domigo(s)").arg(QString::number(nDayOff), QString::number(nSundays));

  if (nTotal == 0)
    msg2 = tr(" | Dia ") + day + tr(" não há folgas");
  else
    msg2 = tr(" | Dia ") + day + tr(" possui %1 folga(s)").arg(QString::number(nTotal));

  return msg1 + msg2;
}

void DayOffTable::autoComplete()
{
  QVector<QVector<bool>> v;
  for (int i = 0; i != rowCount(); ++i)
  {
    QVector<int> sundays, mondays;
    for (int j = 0; j != m_date.daysInMonth(); ++j)
    {
      QDate dt = QDate(m_date.year(), m_date.month(), j);
        if (dt.dayOfWeek() == Qt::Sunday)
          sundays.push_back(j - 1);
        else if (dt.dayOfWeek() == Qt::Monday)
          mondays.push_back(j - 1);
    }
    QVector<bool> dayoff(m_date.daysInMonth(), false);
    if (m_store.m_vEmployee.at(i).m_form.m_bSex)
    {
      // se for mulher, tem dois domingos intercalados e o resto segundas
      bool firstSunday = QRandomGenerator::global()->bounded(1, 3) == 1;
      for (int it = 0; it != sundays.count(); ++it)
        if (it < mondays.count())
        {
          if ((firstSunday && (it == 0 || it == 2)) || (!firstSunday && (it == 1 || it == 3)))
            dayoff[sundays.at(it)] = true;
          else
            dayoff[mondays.at(it)] = true;
        }
        else
          dayoff[mondays.back()] = true;
    }
    else
    {
      // se for homem, tem um domingo e o resto segundas
      int sunday = QRandomGenerator::global()->bounded(0, 4);
      for (int it = 0; it != mondays.count(); ++it)
        if (it == sunday)
          dayoff[sundays.at(it)] = true;
        else
          dayoff[mondays.at(it)] = true;
    }
    v.push_back(dayoff);
  }
  setDayOff(v);
}

void DayOffTable::setDayOff(const QVector<QVector<bool>>& v)
{
  if (v.size() != rowCount())
    return;
  for (int i = 0; i != rowCount(); ++i)
  {
    for (int j = 0; j != columnCount(); ++j)
    {
      auto it = dynamic_cast<JTableItem*>(item(i, j));
      if (it != nullptr)
        it->setValue(v.at(i).at(j));
    }
  }
}

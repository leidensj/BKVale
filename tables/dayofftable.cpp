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

void DayOffTable::shuffle()
{
  for (int i = 0; i != rowCount(); ++i)
    shuffleRow(i);
}

void DayOffTable::shuffleRow(int row)
{
  if (row >= rowCount())
    return;

  QVector<bool> dayoff(m_date.daysInMonth(), false);
  if (m_store.m_vEmployee.at(row).m_form.m_bSex)
  {
    // se for mulher, tem domingos intercalados com segundas
    bool firstSunday = QRandomGenerator::global()->bounded(0, 2) == 0;
    static int lastRow = -1;
    static bool blastSunday = true;
    if (lastRow == row)
      firstSunday = !blastSunday;
    lastRow = row;
    blastSunday = firstSunday;
    for (int ii = 0; ii != dayoff.size(); ++ii)
    {
      if ((firstSunday && m_date.addDays(ii).dayOfWeek() == Qt::Sunday) ||
          (!firstSunday && m_date.addDays(ii).dayOfWeek() == Qt::Monday))
      {
        bool offset = firstSunday;
        while (ii < dayoff.size())
        {
          dayoff[ii] = true;
          ii += offset ? 8 : 6;
          offset = !offset;
        }
        break;
      }
    }
  }
  else
  {
    // TODO
    // se for homem, tem um domingo e o resto segundas
    for (int ii = 0; ii != dayoff.size(); ++ii)
    {
      if (m_date.addDays(ii).dayOfWeek() == Qt::Monday)
      {
        while (ii < dayoff.size())
        {
          dayoff[ii] = true;
          ii += 7;
        }
        break;
      }
    }

    int nsundays = 0;
    for (int ii = 0; ii != dayoff.size(); ++ii)
      if (m_date.addDays(ii).dayOfWeek() == Qt::Sunday)
        nsundays++;

    for (int ii = 0; ii != dayoff.size(); ++ii)
    {
      if (m_date.addDays(ii).dayOfWeek() == Qt::Sunday)
      {
        int it = 0;
        int sunday = QRandomGenerator::global()->bounded(0, nsundays);
        static int lastRow = -1;
        static int lastSunday = -1;
        if (row == lastRow)
          sunday = lastSunday + 1 >= nsundays ? 0 : lastSunday + 1;
        lastRow = row;
        lastSunday = sunday;

        while (ii < dayoff.size())
        {
          if (it == sunday)
          {
            dayoff[ii] = true;
            if (ii + 1 < dayoff.size())
              dayoff[ii + 1] = false;
            break;
          }
          ii += 7;
          it++;
        }
        break;
      }
    }
  }

  if (dayoff.size() == columnCount())
  {
    for (int j = 0; j != columnCount(); ++j)
    {
      auto it = dynamic_cast<JTableItem*>(item(row, j));
      if (it != nullptr)
        it->setValue(dayoff.at(j));
    }
  }
}

void DayOffTable::shuffleCurrentRow()
{
  shuffleRow(currentRow());
}


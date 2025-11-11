#include "salarycalculatortable.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/textitem.h"
#include "tableitems/doubleitem.h"
#include "tableitems/dateitem.h"
#include "tableitems/timeitem.h"
#include "tableitems/sqlitem.h"

SalaryCalculatorTable::SalaryCalculatorTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(7);
  QStringList headers;
  headers << "Nome" << "Fórmula" << "Data Início" << "Hora Início" << "Data Fim" << "Hora Fim" << "Valor";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Name, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Formula, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::DateBegin, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::TimeBegin, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::DateEnd, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::TimeEnd, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Value, QHeaderView::Stretch);
}

void SalaryCalculatorTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itName = new SQLItem(EMPLOYEE_SQL_TABLE_NAME);
  auto itFormula = new SQLItem(SALARY_FORMULA_SQL_TABLE_NAME);
  auto itDtBegin = new DateItem(QDate::currentDate());
  auto itTmBegin = new TimeItem(QTime::currentTime());
  auto itDtEnd = new DateItem(QDate::currentDate());
  auto itTmEnd = new TimeItem(QTime::currentTime());
  auto itValue = new DoubleItem(Data::Type::Money, DoubleItem::Color::Foreground2, false, false);

  JTable::blockSignals(true);
  setItem(row, (int)Column::Name, itName);
  setItem(row, (int)Column::Formula, itFormula);
  setItem(row, (int)Column::DateBegin, itDtBegin);
  setItem(row, (int)Column::TimeBegin, itTmBegin);
  setItem(row, (int)Column::DateEnd, itDtEnd);
  setItem(row, (int)Column::TimeEnd, itTmEnd);
  setItem(row, (int)Column::Value, itValue);
  itName->setReadOnly(true);
  itValue->setReadOnly(true);
  JTable::blockSignals(false);

  setCurrentItem(itName);
  setFocus();
}

void SalaryCalculatorTable::addRowAndActivate()
{
  addRow();
}

void SalaryCalculatorTable::get(int row, SalaryCalculatorResult& o) const
{
  o.clear();
  if (!isValidRow(row))
    return;
  row = verticalHeader()->logicalIndex(row);
  o.eid = SQLItem::st_idFromVariant(getItem(row, (int)Column::Name)->getValue()).get();
  o.ename = SQLItem::st_nameFromVariant(getItem(row, (int)Column::Name)->getValue());
  o.sid = SQLItem::st_idFromVariant(getItem(row, (int)Column::Formula)->getValue()).get();
  o.sname = SQLItem::st_nameFromVariant(getItem(row, (int)Column::Formula)->getValue());
  o.dtBegin = getItem(row, (int)Column::DateBegin)->getValue().toDate();
  o.tmBegin = getItem(row, (int)Column::TimeBegin)->getValue().toTime();
  o.dtEnd = getItem(row, (int)Column::DateEnd)->getValue().toDate();
  o.tmEnd = getItem(row, (int)Column::TimeEnd)->getValue().toTime();
  o.value = getItem(row, (int)Column::Value)->text();
}

void SalaryCalculatorTable::get(QVector<SalaryCalculatorResult>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    SalaryCalculatorResult o;
    get(i, o);
    v.push_back(o);
  }
}

void SalaryCalculatorTable::set(int row, const SalaryCalculatorResult& o)
{
  if (!isValidRow(row))
    return;

  blockSignals(true);
  row = verticalHeader()->logicalIndex(row);

  getItem(row, (int)Column::Name)->setValue(SQLItem::st_toVariant(Id(o.eid), o.ename));
  getItem(row, (int)Column::Formula)->setValue(SQLItem::st_toVariant(Id(o.sid), o.sname));
  getItem(row, (int)Column::DateBegin)->setValue(o.dtBegin);
  getItem(row, (int)Column::TimeBegin)->setValue(o.tmBegin);
  getItem(row, (int)Column::DateEnd)->setValue(o.dtEnd);
  getItem(row, (int)Column::TimeEnd)->setValue(o.tmEnd);
  getItem(row, (int)Column::Value)->setToolTip(o.value);
  getItem(row, (int)Column::Value)->setText(o.value);
  getItem(row, (int)Column::Value)->evaluate();
  blockSignals(false);

  double d = sum((int)Column::Value);
  horizontalHeaderItem((int)Column::Value)->setText(tr("Valor %1").arg(Data::strMoney(d)));
}

void SalaryCalculatorTable::set(const QVector<SalaryCalculatorResult>& v, bool bClear)
{
  if (bClear)
    removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    set(row, v.at(i));
    emit changedSignal(row, 0);
  }
}

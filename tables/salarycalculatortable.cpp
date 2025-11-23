#include "salarycalculatortable.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/textitem.h"
#include "tableitems/doubleitem.h"
#include "tableitems/dateitem.h"
#include "tableitems/timeitem.h"
#include "tableitems/sqlitem.h"
#include "items/salaryformula.h"
#include "items/salary.h"

SalaryCalculatorTable::SalaryCalculatorTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(7);
  QStringList headers;
  headers << "Fórmula" << "Nome"  << "Data Início" << "Hora Início" << "Data Fim" << "Hora Fim" << "Valor";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Formula, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Employee, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::DateBegin, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::TimeBegin, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::DateEnd, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::TimeEnd, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Value, QHeaderView::Stretch);

  connect(this, SIGNAL(changedSignal(int,int)), this, SLOT(update(int,int)));
}

void SalaryCalculatorTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itEmployee = new SQLItem(EMPLOYEE_SQL_TABLE_NAME);
  auto itFormula = new SQLItem(SALARY_FORMULA_SQL_TABLE_NAME);
  auto itDtBegin = new DateItem(QDate::currentDate());
  QTime ct = QTime::currentTime();
  auto itTmBegin = new TimeItem(QTime(ct.hour(), ct.minute()));
  auto itDtEnd = new DateItem(QDate::currentDate());
  auto itTmEnd = new TimeItem(QTime(ct.hour(), ct.minute()));
  auto itValue = new DoubleItem(Data::Type::Money, DoubleItem::Color::Foreground2, false, false);

  JTable::blockSignals(true);
  setItem(row, (int)Column::Employee, itEmployee);
  setItem(row, (int)Column::Formula, itFormula);
  setItem(row, (int)Column::DateBegin, itDtBegin);
  setItem(row, (int)Column::TimeBegin, itTmBegin);
  setItem(row, (int)Column::DateEnd, itDtEnd);
  setItem(row, (int)Column::TimeEnd, itTmEnd);
  setItem(row, (int)Column::Value, itValue);
  itValue->setReadOnly(true);
  JTable::blockSignals(false);

  setCurrentItem(itEmployee);
  setFocus();
}

void SalaryCalculatorTable::addRow(const qlonglong fid, const QString& fname, const qlonglong eid, const QString& ename, const QDateTime& begin, const QDateTime& end)
{
  addRow();
  int row = rowCount() - 1;
  blockSignals(true);
  getItem(row, (int)Column::Formula)->setValue(SQLItem::st_toVariant(fid, fname));
  getItem(row, (int)Column::Employee)->setValue(SQLItem::st_toVariant(eid, ename));
  getItem(row, (int)Column::DateBegin)->setValue(begin.date());
  getItem(row, (int)Column::TimeBegin)->setValue(begin.time());
  getItem(row, (int)Column::DateEnd)->setValue(end.date());
  getItem(row, (int)Column::TimeEnd)->setValue(end.time());
  blockSignals(false);
  update(row, 0);
}

void SalaryCalculatorTable::addRowAndActivate()
{
  addRow();
  int row = rowCount() - 1;
  getItem(row, (int)Column::Formula)->activate();
  getItem(row, (int)Column::Employee)->activate();
}

void SalaryCalculatorTable::update(int row, int /*column*/)
{
  if (!isValidRow(row))
    return;

  QDateTime begin(getItem(row, (int)Column::DateBegin)->getValue().toDate(), getItem(row, (int)Column::TimeBegin)->getValue().toTime());
  QDateTime end(getItem(row, (int)Column::DateEnd)->getValue().toDate(), getItem(row, (int)Column::TimeEnd)->getValue().toTime());
  auto it = getItem(row, (int)Column::Value);

  if (end < begin)
  {
    blockSignals(true);
    it->setValue(0);
    it->setToolTip(tr("Data e hora iniciais maiores que as finais."));
    blockSignals(false);
  }

  QString error;
  SalaryFormula sf;
  sf.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Formula)->getValue());
  sf.SQL_select(error);

  Names snames;
  Values svalues;
  Employee e;
  e.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Employee)->getValue());
  Salary::SQL_select_all_employee_salaries(e.m_id, snames, svalues, error);
  QString formula = sf.m_formula;
  if (snames.size() == svalues.size())
    for (int i = 0; i != snames.size(); ++i)
      formula.replace(snames.at(i), Data::strFmt(svalues.at(i)));
  formula.replace("DIAS", Data::strFmt(begin.daysTo(end)));
  formula.replace("HORAS", Data::strFmt(begin.secsTo(end)/3600.0));
  formula.replace("MINUTOS", Data::strFmt(begin.secsTo(end)/60.0));
  blockSignals(true);
  it->setToolTip(formula);
  it->setText(formula);
  it->evaluate();
  blockSignals(false);

  double d = sum((int)Column::Value);
  horizontalHeaderItem((int)Column::Value)->setText(tr("Valor %1").arg(Data::strMoney(d)));
}

void SalaryCalculatorTable::row(int row, QString& fname, QString& ename, QDateTime& begin, QDateTime& end, double& value) const
{
  fname.clear();
  ename.clear();
  value = 0.0;
  if (!isValidRow(row))
    return;
  fname = SQLItem::st_nameFromVariant(getItem(row, (int)Column::Formula)->getValue());
  ename = SQLItem::st_nameFromVariant(getItem(row, (int)Column::Employee)->getValue());
  begin = QDateTime(getItem(row, (int)Column::DateBegin)->getValue().toDate(), getItem(row, (int)Column::TimeBegin)->getValue().toTime());
  end = QDateTime(getItem(row, (int)Column::DateEnd)->getValue().toDate(), getItem(row, (int)Column::TimeEnd)->getValue().toTime());
  value = getItem(row, (int)Column::Value)->getValue().toDouble();
}

bool SalaryCalculatorTable::isValidCurrentItem() const
{
  int row = currentRow();
  bool ok = isValidRow(row);
  if (ok)
  {
    ok = SQLItem::st_idFromVariant(getItem(row, (int)Column::Formula)->getValue()).isValid();
    if (ok)
      ok = SQLItem::st_idFromVariant(getItem(row, (int)Column::Employee)->getValue()).isValid();
  }
  return ok;
}

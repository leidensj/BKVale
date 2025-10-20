#include "salaryemployeestable.h"
#include <QHeaderView>
#include "tableitems/sqlitem.h"
#include "tableitems/doubleitem.h"
#include <QLabel>

SalaryEmployeesTable::SalaryEmployeesTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(2);
  QStringList headers;
  headers << "Funcionário" << "Salário";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Employee, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Salary, QHeaderView::Stretch);
  connect(this, SIGNAL(changedSignal(int,int)), this, SLOT(update()));
}

void SalaryEmployeesTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto e = new SQLItem(EMPLOYEE_SQL_TABLE_NAME);
  auto s = new DoubleItem(Data::Type::Money, DoubleItem::Color::Foreground2, false, false);

  blockSignals(true);
  setItem(row, (int)Column::Employee, e);
  setItem(row, (int)Column::Salary, s);
  blockSignals(false);

  setCurrentItem(e);
  setFocus();
}

void SalaryEmployeesTable::addRowAndActivate()
{
  addRow();
}

void SalaryEmployeesTable::get(QVector<SalaryEmployee>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    SalaryEmployee o;
    o.m_employee.m_id = SQLItem::st_idFromVariant(getItem(row, (int)Column::Employee)->getValue());
    o.m_employee.m_form.m_name = getItem(row, (int)Column::Employee)->text();
    o.m_salary = getItem(row, (int)Column::Salary)->getValue().toDouble();
    v.push_back(o);
  }
}

void SalaryEmployeesTable::set(const QVector<SalaryEmployee>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
    addRow(v.at(i));
}

void SalaryEmployeesTable::addRow(const SalaryEmployee& o)
{
  addRow();
  int row = rowCount() - 1;
  getItem(row, (int)Column::Employee)->setValue(SQLItem::st_toVariant(o.m_employee));
  getItem(row, (int)Column::Salary)->setValue(o.m_salary);
}

void SalaryEmployeesTable::order()
{
  QVector<SalaryEmployee> v;
  get(v);
  std::sort(v.begin(), v.end());
  set(v);
}

void SalaryEmployeesTable::update()
{
  double d = sum((int)Column::Salary);
  horizontalHeaderItem((int)Column::Salary)->setText(tr("Salário %1").arg(Data::strMoney(d)));
}

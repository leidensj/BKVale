#include "storeemployeetablewidget.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "jtablewidgetitem.h"

StoreEmployeeTableWidget::StoreEmployeeTableWidget(QWidget* parent)
  : JTable((int)Flags::NoFlags, parent)
{
  setColumnCount(2);
  QStringList headers;
  headers << "Nome" << "Horas";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Name, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::WorkingHours, QHeaderView::Stretch);
}

const JItem& StoreEmployeeTableWidget::getItem(int row) const
{
  m_ref.clear();
  if (isValidRow(row))
  {
    int idx = verticalHeader()->logicalIndex(row);
    m_ref.m_employee = dynamic_cast<const Person&>(((PersonTableWidgetItem*)item(idx, (int)Column::Name))->getItem());
    m_ref.m_hours = ((TimeIntervalsTableWidgetItem*)item(idx, (int)Column::WorkingHours))->getItems();
  }
  return m_ref;
}

void StoreEmployeeTableWidget::addItem()
{
  addItem(StoreEmployee());
  int row = rowCount() - 1;
  auto ptEmployeeCell = dynamic_cast<PersonTableWidgetItem*>(item(row, (int)Column::Name));
  ptEmployeeCell->selectItem(PERSON_FILTER_EMPLOYEE);
  const Person& employee = dynamic_cast<const Person&>(ptEmployeeCell->getItem());
  if (!employee.m_id.isValid())
    removeItem();

  setFocus();
}

void StoreEmployeeTableWidget::addItem(const JItem& o)
{
  const StoreEmployee& _o = dynamic_cast<const StoreEmployee&>(o);

  blockSignals(true);

  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)Column::Name, new PersonTableWidgetItem);
  setItem(row, (int)Column::WorkingHours, new TimeIntervalsTableWidgetItem);
  setCurrentCell(row, (int)Column::Name);

  ((PersonTableWidgetItem*)item(row, (int)Column::Name))->setItem(_o.m_employee);
  ((TimeIntervalsTableWidgetItem*)item(row, (int)Column::WorkingHours))->setItems(_o.m_hours);

  setCurrentCell(row, (int)Column::Name);
  blockSignals(false);
}

void StoreEmployeeTableWidget::update(int /*row*/, int /*column*/)
{
  blockSignals(true);
  blockSignals(false);
  emitChangedSignal();
}

void StoreEmployeeTableWidget::itemActivate(int row, int column)
{
  if (column == (int)Column::Name)
  {
    auto ptEmployee = (PersonTableWidgetItem*)item(row, column);
    ptEmployee->selectItem(PERSON_FILTER_EMPLOYEE);
  }
  else if (column == (int)Column::WorkingHours)
  {
    auto ptHours = (TimeIntervalsTableWidgetItem*)item(row, column);
    ptHours->selectItem();
  }
}

void StoreEmployeeTableWidget::itemDelete(int /*row*/, int /*column*/)
{

}

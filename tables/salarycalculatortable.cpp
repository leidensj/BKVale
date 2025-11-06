#include "salarycalculatortable.h".h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/textitem.h"
#include "tableitems/doubleitem.h"

SalaryCalculatorTable::SalaryCalculatorTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(2);
  QStringList headers;
  headers << "Nome" << "Valor";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Name, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Value, QHeaderView::Stretch);
}

void SalaryCalculatorTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itName = new TextItem(Text::Input::ASCII, true);
  auto itValue = new DoubleItem(Data::Type::Money, DoubleItem::Color::Foreground2, false, false);

  blockSignals(true);
  setItem(row, (int)Column::Name, itName);
  setItem(row, (int)Column::Value, itValue);
  itName->setReadOnly(true);
  itValue->setReadOnly(true);
  blockSignals(false);

  setCurrentItem(itName);
  setFocus();
}

void SalaryCalculatorTable::addRowAndActivate()
{
  addRow();
}

void SalaryCalculatorTable::get(QVector<SalaryCalculatorResult>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    SalaryCalculatorResult o;
    o.name = getItem(row, (int)Column::Name)->getValue().toString();
    o.value = getItem(row, (int)Column::Value)->text();
    v.push_back(o);
  }
}

void SalaryCalculatorTable::set(const QVector<SalaryCalculatorResult>& v, bool bClear)
{
  if (bClear)
    removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    getItem(row, (int)Column::Name)->setValue(v.at(i).name);
    getItem(row, (int)Column::Value)->setToolTip(v.at(i).value);
    getItem(row, (int)Column::Value)->setText(v.at(i).value);
    getItem(row, (int)Column::Value)->evaluate();
  }

  double d = sum((int)Column::Value);
  horizontalHeaderItem((int)Column::Value)->setText(tr("Valor %1").arg(Data::strMoney(d)));
}

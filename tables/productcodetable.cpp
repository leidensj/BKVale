#include "productcodetable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "widgets/jtablewidgetitem.h"

ProductCodeTable::ProductCodeTable(QWidget* parent)
  : JItemTable((int)Flags::NoFlags, parent)
{
  setColumnCount(1);
  QStringList headers;
  headers << "Código";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Code, QHeaderView::Stretch);
}

const JItem& ProductCodeTable::getItem(int row) const
{
  m_ref.clear();
  if (isValidRow(row))
  {
    int idx = verticalHeader()->logicalIndex(row);
    m_ref.m_code = item(idx, (int)Column::Code)->text();
  }
  return m_ref;
}

void ProductCodeTable::addItem()
{
  addItem(ProductCode());
  setFocus();
}

void ProductCodeTable::addItem(const JItem& o)
{
  const ProductCode& _o = dynamic_cast<const ProductCode&>(o);

  blockSignals(true);

  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)Column::Code, new QTableWidgetItem(_o.m_code));
  setCurrentCell(row, (int)Column::Code);

  blockSignals(false);
}

void ProductCodeTable::update(int /*row*/, int /*column*/)
{
  blockSignals(true);
  blockSignals(false);
  emitChangedSignal();
}

void ProductCodeTable::itemActivate(int /*row*/, int /*column*/)
{

}

void ProductCodeTable::itemDelete(int /*row*/, int /*column*/)
{

}

#include "phonetable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "widgets/jtablewidgetitem.h"

PhoneTable::PhoneTable(QWidget* parent)
  : JItemTable((int)Flags::NoFlags, parent)
{
  setColumnCount(1);
  QStringList headers;
  headers << "Telefone";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Phone, QHeaderView::Stretch);
}

const JItem& PhoneTable::getItem(int row) const
{
  m_ref.clear();
  if (isValidRow(row))
  {
    int idx = verticalHeader()->logicalIndex(row);
    m_ref = ((PhoneEditorTableWidgetItem*)item(idx, (int)Column::Phone))->getItem();
  }
  return m_ref;
}

void PhoneTable::addItem()
{
  addItem(Phone());
  int row = rowCount() - 1;
  auto ptPhoneCell = (PhoneEditorTableWidgetItem*)(item(row, (int)Column::Phone));
  if (!ptPhoneCell->selectItem())
    removeItem();
  setFocus();
}

void PhoneTable::addItem(const JItem& o)
{
  const Phone& _o = dynamic_cast<const Phone&>(o);

  blockSignals(true);

  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)Column::Phone, new PhoneEditorTableWidgetItem);
  setCurrentCell(row, (int)Column::Phone);

  ((PhoneEditorTableWidgetItem*)item(row, (int)Column::Phone))->setItem(_o);

  setCurrentCell(row, (int)Column::Phone);
  blockSignals(false);
}

void PhoneTable::update(int /*row*/, int /*column*/)
{
  blockSignals(true);
  blockSignals(false);
  emitChangedSignal();
}

void PhoneTable::itemActivate(int row, int column)
{
  if (column == (int)Column::Phone)
  {
    auto ptPhone = (PhoneEditorTableWidgetItem*)item(row, column);
    ptPhone->selectItem();
  }
}

void PhoneTable::itemDelete(int row, int column)
{
  if (column == (int)Column::Phone)
  {
    auto ptPhone = (PhoneEditorTableWidgetItem*)item(row, column);
    ptPhone->setItem(Phone());
  }
}

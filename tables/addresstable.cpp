#include "addresstable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>

AddressTable::AddressTable(JAddRemoveButtons* btns, QWidget* parent)
  : JItemTable(btns , parent)
{
  setColumnCount(1);
  QStringList headers;
  headers << "Endereço";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Address, QHeaderView::Stretch);
}

const JItem& AddressTable::getItem(int row) const
{
  m_ref.clear();
  if (isValidRow(row))
  {
    int idx = verticalHeader()->logicalIndex(row);
    m_ref = ((AddressEditorTableWidgetItem*)item(idx, (int)Column::Address))->getItem();
  }
  return m_ref;
}

void AddressTable::addItem()
{
  addItem(Address());
  int row = rowCount() - 1;
  auto ptAddressCell = (AddressEditorTableWidgetItem*)(item(row, (int)Column::Address));
  if (!ptAddressCell->selectItem())
    removeItem();
  setFocus();
}

void AddressTable::addItem(const JItem& o)
{
  const Address& _o = dynamic_cast<const Address&>(o);

  blockSignals(true);

  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)Column::Address, new AddressEditorTableWidgetItem);
  setCurrentCell(row, (int)Column::Address);

  ((AddressEditorTableWidgetItem*)item(row, (int)Column::Address))->setItem(_o);

  setCurrentCell(row, (int)Column::Address);
  blockSignals(false);
}

void AddressTable::update(int /*row*/, int /*column*/)
{
  blockSignals(true);
  blockSignals(false);
  emitChangedSignal();
}

void AddressTable::itemActivate(int row, int column)
{
  if (column == (int)Column::Address)
  {
    auto ptAddress = (AddressEditorTableWidgetItem*)item(row, column);
    ptAddress->selectItem();
  }
}

void AddressTable::itemDelete(int row, int column)
{
  if (column == (int)Column::Address)
  {
    auto ptPhone = (AddressEditorTableWidgetItem*)item(row, column);
    ptPhone->setItem(Address());
  }
}

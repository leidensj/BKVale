#include "addresstable.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/addressitem.h"

AddressTable::AddressTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(1);
  QStringList headers;
  headers << "Endereço";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Address, QHeaderView::Stretch);
}

void AddressTable::getAddresses(QVector<Address>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    v.push_back(AddressItem::st_fromVariant(getItem(row, (int)Column::Address)->getValue()));
  }
}

void AddressTable::setAddresses(const QVector<Address>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    getItem(i, (int)Column::Address)->setValue(AddressItem::st_toVariant(v.at(i)));
  }
}

void AddressTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto it = new AddressItem();

  blockSignals(true);
  setItem(row, (int)Column::Address, it);
  blockSignals(false);

  setCurrentItem(it);
  setFocus();
}

void AddressTable::addRowAndActivate()
{
  addRow();
  JTableItem* it = getItem(rowCount() - 1, (int)Column::Address);
  it->activate();
  if (!AddressItem::st_fromVariant(it->getValue()).isValid())
    removeItem();
}

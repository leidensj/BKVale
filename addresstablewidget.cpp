#include "addresstablewidget.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "jtablewidgetitem.h"

AddressTableWidget::AddressTableWidget(QWidget* parent)
  : JItemTable((int)Flags::NoFlags , parent)
{
  setColumnCount(1);
  QStringList headers;
  headers << "Endereço";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Address, QHeaderView::Stretch);
}

const JItem& AddressTableWidget::getItem(int row) const
{
  m_ref.clear();
  if (isValidRow(row))
  {
    int idx = verticalHeader()->logicalIndex(row);
    m_ref = ((AddressEditorTableWidgetItem*)item(idx, (int)Column::Address))->getItem();
  }
  return m_ref;
}

void AddressTableWidget::addItem()
{
  addItem(Address());
  int row = rowCount() - 1;
  auto ptAddressCell = (AddressEditorTableWidgetItem*)(item(row, (int)Column::Address));
  if (!ptAddressCell->selectItem())
    removeItem();
  setFocus();
}

void AddressTableWidget::addItem(const JItem& o)
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

void AddressTableWidget::update(int /*row*/, int /*column*/)
{
  blockSignals(true);
  blockSignals(false);
  emitChangedSignal();
}

void AddressTableWidget::itemActivate(int row, int column)
{
  if (column == (int)Column::Address)
  {
    auto ptAddress = (AddressEditorTableWidgetItem*)item(row, column);
    ptAddress->selectItem();
  }
}

void AddressTableWidget::itemDelete(int row, int column)
{
  if (column == (int)Column::Address)
  {
    auto ptPhone = (AddressEditorTableWidgetItem*)item(row, column);
    ptPhone->setItem(Address());
  }
}

#include "phonetablewidget.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "jtablewidgetitem.h"

PhoneTableWidget::PhoneTableWidget(QWidget* parent)
: JTable((int)Flags::NoFlags, parent)
{
  setColumnCount(1);
  QStringList headers;
  headers << "Telefone";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Phone, QHeaderView::Stretch);
}

const JItem& PhoneTableWidget::getItem(int row) const
{
  m_ref.clear();
  if (isValidRow(row))
  {
    int idx = verticalHeader()->logicalIndex(row);
    m_ref = ((PhoneEditorTableWidgetItem*)item(idx, (int)Column::Phone))->getItem();
  }
  return m_ref;
}

void PhoneTableWidget::addItem()
{
  addItem(Phone());
  int row = rowCount() - 1;
  auto ptPhoneCell = (PhoneEditorTableWidgetItem*)(item(row, (int)Column::Phone));
  if (!ptPhoneCell->selectItem())
    removeItem();
  setFocus();
}

void PhoneTableWidget::addItem(const JItem& o)
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

void PhoneTableWidget::update(int /*row*/, int /*column*/)
{
  blockSignals(true);
  blockSignals(false);
  emitChangedSignal();
}

void PhoneTableWidget::itemActivate(int row, int column)
{
  if (column == (int)Column::Phone)
  {
    auto ptPhone = (PhoneEditorTableWidgetItem*)item(row, column);
    ptPhone->selectItem();
  }
}

void PhoneTableWidget::itemDelete(int row, int column)
{
  if (column == (int)Column::Phone)
  {
    auto ptPhone = (PhoneEditorTableWidgetItem*)item(row, column);
    ptPhone->setItem(Phone());
  }
}

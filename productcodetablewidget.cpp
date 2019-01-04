#include "productcodetablewidget.h"
#include "databaseutils.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "jtablewidgetitem.h"

ProductCodeTableWidget::ProductCodeTableWidget(QWidget* parent)
  : JTable((int)Flags::NoFlags, parent)
{
  setColumnCount(1);
  QStringList headers;
  headers << "Código";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Code, QHeaderView::Stretch);
}

const JItem& ProductCodeTableWidget::getItem(int row) const
{
  m_ref.clear();
  if (isValidRow(row))
  {
    int idx = verticalHeader()->logicalIndex(row);
    m_ref.m_code = item(idx, (int)Column::Code)->text();
  }
  return m_ref;
}

void ProductCodeTableWidget::addItem()
{
  addItem(ProductCode());
  setFocus();
}

void ProductCodeTableWidget::addItem(const JItem& o)
{
  const ProductCode& _o = dynamic_cast<const ProductCode&>(o);

  blockSignals(true);

  insertRow(rowCount());
  int row = rowCount() - 1;
  setItem(row, (int)Column::Code, new QTableWidgetItem(_o.m_code));
  setCurrentCell(row, (int)Column::Code);

  blockSignals(false);
}

void ProductCodeTableWidget::update(int /*row*/, int /*column*/)
{
  blockSignals(true);
  blockSignals(false);
  emitChangedSignal();
}

void ProductCodeTableWidget::itemActivate(int /*row*/, int /*column*/)
{

}

void ProductCodeTableWidget::itemDelete(int /*row*/, int /*column*/)
{

}

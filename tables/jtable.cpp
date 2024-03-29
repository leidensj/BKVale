#include "jtable.h"
#include <QKeyEvent>
#include <QHeaderView>
#include <QPushButton>

JTable::JTable(JAddRemoveButtons* btns, QWidget* parent)
 : QTableWidget(parent)
{
  if (btns != nullptr)
  {
    connect(btns->m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(addRowAndActivate()));
    connect(btns->m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(emitChangedSignal()));
    connect(btns->m_btnRemove, SIGNAL(clicked(bool)), this, SLOT(removeItem()));
    connect(this, SIGNAL(changedSignal(bool)), btns->m_btnRemove, SLOT(setEnabled(bool)));
  }

  setSelectionBehavior(QAbstractItemView::SelectItems);
  setSelectionMode(QAbstractItemView::SingleSelection);

  horizontalHeader()->setHighlightSections(false);
  verticalHeader()->setSectionsMovable(true);
  verticalHeader()->setHighlightSections(false);

  QObject::connect(this, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(activate(QTableWidgetItem*)));
  QObject::connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(evaluate(QTableWidgetItem*)));
}

JTable::~JTable()
{

}

void JTable::addRowAndActivate()
{
  addRow();
}

void JTable::setLargerSize(bool b)
{
  int pointSize = b ? 12 : 8;
  QFont f = font();
  f.setPointSize(pointSize);
  setFont(f);
  f = horizontalHeader()->font();
  f.setPointSize(pointSize);
  horizontalHeader()->setFont(f);
}

void JTable::removeItem()
{
  if (isValidRow(currentRow()))
    removeRow(currentRow());
  emitChangedSignal();
}

void JTable::removeAllItems()
{
  if (hasItems())
  {
    setRowCount(0);
    emitChangedSignal();
  }
}

bool JTable::hasItems() const
{
  return rowCount() != 0;
}

void JTable::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    QKeyEvent modEvent(event->type(),
                       Qt::Key_Tab,
                       event->modifiers(),
                       event->nativeScanCode(),
                       event->nativeVirtualKey(),
                       event->nativeModifiers(),
                       event->text(),
                       event->isAutoRepeat(),
                       event->count());
    QTableWidget::keyPressEvent(&modEvent);
  }
  else if (event->key() == Qt::Key_Delete)
  {
      if (selectionMode() == QAbstractItemView::SingleSelection)
      {
        if (currentIndex().isValid())
          erase(item(currentIndex().row(), currentIndex().column()));
      }
      else
      {
        auto sm = selectionModel()->selectedIndexes();
        for (int i = 0; i != sm.size(); ++i)
          erase(item(sm.at(i).row(), sm.at(i).column()));
      }
    QTableWidget::keyPressEvent(event);
  }
  else if (event->key() == Qt::Key_Space)
  {
    if (selectionMode() == QAbstractItemView::SingleSelection)
    {
      if (currentIndex().isValid())
        activate(item(currentIndex().row(), currentIndex().column()));
    }
    else
    {
      auto sm = selectionModel()->selectedIndexes();
      for (int i = 0; i != sm.size(); ++i)
        activate(item(sm.at(i).row(), sm.at(i).column()));
    }
    QTableWidget::keyPressEvent(event);
  }
  else
  {
    QTableWidget::keyPressEvent(event);
  }
}

void JTable::emitChangedSignal()
{
  emit changedSignal(isValidCurrentRow());
}

bool JTable::isValidRow(int row) const
{
  return row >= 0 && row < rowCount();
}

bool JTable::isValidCurrentRow() const
{
  return isValidRow(currentRow());
}

void JTable::setHeaderIcon(int pos, const QIcon& icon)
{
  model()->setHeaderData(pos, Qt::Horizontal, icon, Qt::DecorationRole);
}

JTableItem* JTable::getItem(int row, int column) const
{
  JTableItem* p = nullptr;
  QTableWidgetItem* p2 = item(row, column);
  if (p2 != nullptr)
    p = dynamic_cast<JTableItem*>(p2);
  return p;
}

void JTable::activate(QTableWidgetItem* p)
{
  if (p != nullptr && editTriggers() != QAbstractItemView::NoEditTriggers)
  {
    JTableItem* p2 = dynamic_cast<JTableItem*>(p);
    p2->activate();
    emitChangedSignal();
    emit changedSignal(p2->row(), p2->column());
  }
}

void JTable::erase(QTableWidgetItem* p)
{
  if (p != nullptr && editTriggers() != QAbstractItemView::NoEditTriggers)
  {
    JTableItem* p2 = dynamic_cast<JTableItem*>(p);
    p2->erase();
    emitChangedSignal();
    emit changedSignal(p2->row(), p2->column());
  }
}

void JTable::evaluate(QTableWidgetItem* p)
{
  if (p != nullptr)
  {
    JTableItem* p2 = dynamic_cast<JTableItem*>(p);
    p2->evaluate();
    emitChangedSignal();
    emit changedSignal(p2->row(), p2->column());
  }
}

double JTable::sum(int column) const
{
  double total = 0.0;
  if (column < columnCount())
  {
    for (int i = 0; i != rowCount(); ++i)
      total += getItem(i, column)->getValue().toDouble();
  }
  return total;
}

void JTable::clearAll()
{
  setRowCount(0);
  setColumnCount(0);
}

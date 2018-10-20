#include "jitemtable.h"
#include <QTableWidget>
#include <QKeyEvent>

JTable::JTable(QWidget* parent)
 : QTableWidget(parent)
{
  QFont f = font();
  f.setPointSize(12);
  f.setCapitalization(QFont::AllUppercase);
  setFont(f);
  f = horizontalHeader()->font();
  f.setPointSize(12);
  f.setCapitalization(QFont::Capitalize);
  horizontalHeader()->setFont(f);

  setSelectionBehavior(QAbstractItemView::SelectItems);
  setSelectionMode(QAbstractItemView::SingleSelection);
  horizontalHeader()->setHighlightSections(false);
  verticalHeader()->setSectionsMovable(true);
  verticalHeader()->setHighlightSections(false);

  QObject::connect(this,
                   SIGNAL(cellChanged(int, int)),
                   this,
                   SLOT(update(int, int)));

  QObject::connect(this,
                   SIGNAL(currentCellChanged(int, int, int, int)),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(this,
                   SIGNAL(cellDoubleClicked(int, int)),
                   this,
                   SLOT(itemDoubleClicked(int, int)));
}

void JTable::removeItem()
{
  if (isValidRow(currentRow()))
    removeRow(currentRow());
}

void JTable::removeAllItems()
{
  setRowCount(0);
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
  else
  {
    QKeyEvent modEvent(event->type(),
                       event->key(),
                       event->modifiers(),
                       event->nativeScanCode(),
                       event->nativeVirtualKey(),
                       event->nativeModifiers(),
                       event->text().toUpper(),
                       event->isAutoRepeat(),
                       event->count());
    QTableWidget::keyPressEvent(&modEvent);
  }
}

void JTable::emitChangedSignal()
{
  emit changedSignal();
}

bool JTable::isValidRow(int row) const
{
  return row >= 0 && row < rowCount();
}

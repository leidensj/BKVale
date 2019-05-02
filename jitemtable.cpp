#include "jitemtable.h"
#include <QTableWidget>
#include <QKeyEvent>
#include <QLayout>
#include <QPushButton>

JTable::JTable(int flags, QWidget* parent)
 : QTableWidget(parent)
{
  if (flags & (int)Flags::BigFont)
  {
    QFont f = font();
    f.setPointSize(12);
    setFont(f);
    f = horizontalHeader()->font();
    f.setPointSize(12);
  }

  if (flags & (int)Flags::Uppercase)
  {
    QFont f = font();
    f.setCapitalization(QFont::AllUppercase);
    setFont(f);
    f.setCapitalization(QFont::Capitalize);
    horizontalHeader()->setFont(f);
  }

  setSelectionBehavior(QAbstractItemView::SelectItems);
  setSelectionMode(QAbstractItemView::SingleSelection);
  horizontalHeader()->setHighlightSections(false);
  verticalHeader()->setSectionsMovable(true);
  verticalHeader()->setHighlightSections(false);

  QObject::connect(this, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(emitChangedSignal()));
  QObject::connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(emitActivateSignal(int, int)));
}

void JTable::removeItem()
{
  if (isValidRow(currentRow()))
    removeRow(currentRow());
  emitChangedSignal();
}

void JTable::removeAllItems()
{
  setRowCount(0);
  emitChangedSignal();
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
    if (currentIndex().isValid())
      emit deleteSignal(currentIndex().row(), currentIndex().column());
    QTableWidget::keyPressEvent(event);
  }
  else if (event->key() == Qt::Key_Space)
  {
    if (currentIndex().isValid())
      emit activateSignal(currentIndex().row(), currentIndex().column());
    QTableWidget::keyPressEvent(event);
  }
  else
  {
    QTableWidget::keyPressEvent(event);
  }

}

void JTable::emitChangedSignal()
{
  emit changedSignal();
}


void JTable::emitDeleteSignal(int row, int column)
{
  emit deleteSignal(row, column);
}

void JTable::emitActivateSignal(int row, int column)
{
  emit activateSignal(row, column);
}

bool JTable::isValidRow(int row) const
{
  return row >= 0 && row < rowCount();
}

bool JTable::isValidCurrentRow() const
{
  return isValidRow(currentRow());
}

JItemTable::JItemTable(int flags, QWidget* parent)
  : JTable(flags, parent)
{
  QObject::connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(update(int, int)));
}

void JItemTable::setHeaderIcon(int pos, const QIcon& icon)
{
  model()->setHeaderData(pos,
                         Qt::Horizontal,
                         icon,
                         Qt::DecorationRole);
}

void JItemTable::setHeaderIconSearchable(int pos)
{
  setHeaderIcon(pos, QIcon(":/icons/res/binoculars.png"));
}

JTableButtons::JTableButtons(JTable *table, int maxNumberofItems, int orientation, QWidget *parent)
  : QWidget(parent)
  , m_table(table)
  , m_max(maxNumberofItems)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
{
  m_btnAdd = new QPushButton;
  m_btnAdd->setFlat(true);
  m_btnAdd->setIconSize(QSize(24, 24));
  m_btnAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAdd->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));
  m_btnAdd->setToolTip(tr("Adicionar (Alt++)"));

  m_btnRemove = new QPushButton;
  m_btnRemove->setFlat(true);
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Minus));
  m_btnRemove->setToolTip(tr("Remover (Alt+-)"));

  if (orientation == Qt::Horizontal)
  {
    QHBoxLayout* lt = new QHBoxLayout;
    lt->setAlignment(Qt::AlignRight);
    lt->setContentsMargins(0, 0, 0, 0);
    lt->addWidget(m_btnAdd);
    lt->addWidget(m_btnRemove);
    setLayout(lt);
  }
  else
  {
    QVBoxLayout* lt = new QVBoxLayout;
    lt->setAlignment(Qt::AlignTop);
    lt->setContentsMargins(0, 0, 0, 0);
    lt->addWidget(m_btnAdd);
    lt->addWidget(m_btnRemove);
    setLayout(lt);
  }

  connect(m_btnAdd, SIGNAL(clicked(bool)), table, SLOT(addItem()));
  connect(m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_btnRemove, SIGNAL(clicked(bool)), table, SLOT(removeItem()));
  connect(table, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  updateControls();
}

void JTableButtons::updateControls()
{
  if (m_table != nullptr)
  {
    m_btnAdd->setEnabled(m_max > 0 ? m_table->rowCount() < m_max : true);
    m_btnRemove->setEnabled(m_table->isValidCurrentRow());
  }
}

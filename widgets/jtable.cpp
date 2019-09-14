#include "jtable.h"
#include <QTableWidget>
#include <QKeyEvent>
#include <QLayout>
#include <QPushButton>
#include "tinyexpr.h"
#include "jregexpvalidator.h"
#include "jdatabase.h"
#include "packageeditor.h"

JTable::JTable(JAddRemoveButtons* btns, QWidget* parent)
 : QTableWidget(parent)
{
  if (btns != nullptr)
  {
    connect(btns->m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(addRow()));
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
    if (currentIndex().isValid())
      erase(item(currentIndex.row(), currentIndex.column()));
    QTableWidget::keyPressEvent(event);
  }
  else if (event->key() == Qt::Key_Space)
  {
    if (currentIndex().isValid())
      activate(item(currentIndex.row(), currentIndex.column()));
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
  if (p != nullptr)
  {
    JTableItem* p2 = dynamic_cast<JTableItem*>(p);
    p2->activate();
    emitChangedSignal();
  }
}

void JTable::erase(QTableWidgetItem* p)
{
  if (p != nullptr)
  {
    JTableItem* p2 = dynamic_cast<JTableItem*>(p);
    p2->erase();
    emitChangedSignal();
  }
}

void JTable::evaluate(QTableWidgetItem* p)
{
  if (p != nullptr)
  {
    JTableItem* p2 = dynamic_cast<JTableItem*>(p);
    p2->evaluate();
    emitChangedSignal();
  }
}

DoubleItem::DoubleItem(Data::Type type,
                       Color color,
                       bool bCheckable,
                       bool bAcceptNegative,
                       const QString& prefix,
                       const QString& sufix)
  : m_type(type)
  , m_color(color)
  , m_bCheckable(bCheckable)
  , m_bAcceptNegative(bAcceptNegative)
  , m_prefix(prefix)
  , m_sufix(sufix)
{
  if (m_bCheckable)
  {
    setFlags(flags() | Qt::ItemIsUserCheckable);
    setCheckState(Qt::Checked);
  }
}

void DoubleItem::setValue(const QVariant& v)
{
  double val = v.toDouble();
  setData(Qt::UserRole, val);
  setText(m_prefix + Data::str(val, m_type) + m_sufix);

  if (m_bCheckable)
  {
    if (checkState() == Qt::Unchecked)
      setFlags(flags() & ~Qt::ItemIsEditable);
    else
      setFlags(flags() | Qt::ItemIsEditable);
  }

  switch (m_color)
  {
    case Color::Background:
      setBackgroundColor(val == 0.0 ? QColor(255, 200, 200) : QColor(Qt::white));
      break;
    case Color::Foreground:
      setTextColor(QColor(val >= 0 ? Qt::red : Qt::darkGreen));
      break;
    case Color::None:
    default:
      break;
  }
}

void DoubleItem::evaluate()
{
  auto stdExp = text().toStdString();
  int error = 0;
  double val = te_interp(stdExp.c_str(), &error);
  bool bValid = !error;
  if (bValid)
  {
    if (!m_bAcceptNegative && val < 0)
      bValid = false;
    else
      setValue(val);
  }

  if (!bValid)
    setValue(getValue());
}

void DoubleItem::erase()
{
  setValue(0.0);
}

void DoubleItem::activate()
{

}

DateItem::DateItem(const QDate& defaultDate, Color color)
  : m_defaultDate(defaultDate)
  , m_color(color)
{
  setValue(m_defaultDate);
}

void DateItem::setValue(const QVariant& v)
{
  QDate dt = v.toDate();
  setData(Qt::UserRole, dt);
  setText(dt.toString("dd/MM/yyyy"));

  switch (m_color)
  {
    case Color::DateBeforeDefault:
      setBackgroundColor(dt < m_defaultDate ? QColor(255, 200, 200) : QColor(Qt::white));
      setToolTip(dt < m_defaultDate ? "A data é anterior a " + m_defaultDate.toString("dd/MM/yyyy") : "");
      break;
    case Color::None:
    default:
      break;
  }
}

void DateItem::evaluate()
{

  QDate dt = QDate::fromString(text(), "dd/MM/yyyy");
  if (!dt.isValid())
  {
    dt = QDate::fromString(text(), "dd/MM/yy");
    if (!dt.isValid())
    {
      dt = QDate::fromString(text(), "ddMMyyyy");
      if (!dt.isValid())
      {
        dt = QDate::fromString(text(), "ddMM");
        dt.setDate(QDate::currentDate().year(), dt.month(), dt.day());
        if (!dt.isValid())
        {
          dt = QDate::fromString(text(), "dd");
          dt.setDate(QDate::currentDate().year(), QDate::currentDate().month(), dt.day());
        }
      }
    }
  }

  if (dt.isValid())
    setValue(dt);
  else
    setValue(getValue());
}

void DateItem::erase()
{
  setValue(m_defaultDate);
}

void DateItem::activate()
{

}

TimeItem::TimeItem(const QTime& defaultTime)
  : m_defaultTime(defaultTime)
{
  setValue(m_defaultTime);
}

void TimeItem::setValue(const QVariant& v)
{
  QTime t = v.toTime();
  setData(Qt::UserRole, t);
  setText(t.toString("HH:mm"));
}

void TimeItem::evaluate()
{
  QTime t = QTime::fromString(text(), "HH:mm");
  if (!t.isValid())
    t = QTime::fromString(exp, "HHmm");

  if (t.isValid())
    setValue(t);
  else
    setValue(getValue());
}

void TimeItem::erase()
{
  setValue(m_defaultTime);
}

void TimeItem::activate()
{

}

TextItem::TextItem(Text::Input input, bool toUpper)
  : m_input(input)
  , m_toUpper(toUpper)
{

}

void TextItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, v.toString());
  setText(v.toString());
}

void TextItem::evaluate()
{
  JRegExpValidator val(m_toUpper, QRegExp(Text::getRegEx(m_input)));
  QString str = text();
  if (m_toUpper)
    str.toUpper();
  int pos = str.length();
  bool bValid = val.validate(str, pos) == QValidator::State::Acceptable;
  if (bValid)
    setValue(str);
  else
    setValue(getValue());
}

void TextItem::erase()
{
  setText("");
}

void TextItem::activate()
{

}

JItemSQLItem::JItemSQLItem(const QString& tableName)
  : m_tableName(tableName)
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void JItemSQLItem::activate()
{
  JDatabaseSelector dlg(m_tableName, false, tableWidget());
  if (dlg.exec())
  {
    JItemSQL* p = dlg.getDatabase()->getCurrentItem();
    if (p != nullptr)
    {
      setData(Qt::UserRole, p->m_id.get());
      setText(p->name());
    }
  }
}

void JItemSQLItem::evaluate()
{

}

void JItemSQLItem::erase()
{
  setData(Qt::UserRole, INVALID_ID);
  setText("");
}

void JItemSQLItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, v.toLongLong());
}

PackageItem::PackageItem()
{

}

void PackageItem::setPackage(const Package& pck, const QString& productUnity)
{
  m_pck = pck;
  m_productUnity = productUnity;
}

void PackageItem::activate()
{
  PackageEditor dlg(m_pck, m_productUnity);
  if (dlg.exec())

  JDatabaseSelector dlg(m_tableName, false, tableWidget());
  if (dlg.exec())
  {
    JItemSQL* p = dlg.getDatabase()->getCurrentItem();
    if (p != nullptr)
    {
      setData(Qt::UserRole, p->m_id.get());
      setText(p->name());
    }
  }
}

void JItemSQLItem::evaluate()
{

}

void JItemSQLItem::erase()
{
  setData(Qt::UserRole, INVALID_ID);
  setText("");
}

void JItemSQLItem::setValue(const QVariant& v)
{

}

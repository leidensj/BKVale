#include "notewidget.h"
#include "ui_notewidget.h"
#include "tinyexpr.h"
#include <QComboBox>
#include <QStringList>
#include <QKeyEvent>

NoteComboBox::NoteComboBox(Behavior behavior)
  : m_behavior(behavior)
{
  setEditable(true);
  if (m_behavior == Supplier)
  {
    auto f = font();
    f.setPointSize(12);
    setFont(f);
    lineEdit()->setPlaceholderText(tr("FORNECEDOR"));
  }

  connect(this,
          SIGNAL(editTextChanged(const QString &)),
          this,
          SLOT(toUpper()));
}

void NoteComboBox::keyPressEvent(QKeyEvent *event)
{
  switch (m_behavior)
  {
    case Supplier:
    {
      if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
      {
        emit supplierEnteredSignal();
      }
      else
      {
        QComboBox::keyPressEvent(event);
      }
    } break;
    case TableUnity:
    case TableDescription:
    {
      if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
      {
        QKeyEvent event2(event->type(),
                         Qt::Key_Tab,
                         event->modifiers(),
                         event->nativeScanCode(),
                         event->nativeVirtualKey(),
                         event->nativeModifiers(),
                         event->text(), event->isAutoRepeat(),
                         event->count());
        QComboBox::keyPressEvent(&event2);
      }
      else
      {
        QComboBox::keyPressEvent(event);
      }
    } break;
    default:
    {
      QComboBox::keyPressEvent(event);
    } break;
  }
}

void NoteComboBox::toUpper()
{
  blockSignals(true);
  setCurrentText(currentText().toUpper());
  blockSignals(false);
}

NoteTableWidget::NoteTableWidget()
{
  setColumnCount(NUMBER_OF_COLUMNS);
  QStringList headers;
  headers << "Quantidade" << "Unidade" << "Descrição" << "Valor Unitário" << "Subtotal";
  setHorizontalHeaderLabels(headers);
  auto f = font();
  f.setPointSize(12);
  setFont(f);
  setSelectionBehavior(QAbstractItemView::SelectItems);
  setSelectionMode(QAbstractItemView::SingleSelection);
  horizontalHeader()->setHighlightSections(false);
  verticalHeader()->setHighlightSections(false);
  horizontalHeader()->setSectionResizeMode((int)NoteColumn::Ammount, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)NoteColumn::Unity, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)NoteColumn::Description, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)NoteColumn::UnitValue, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)NoteColumn::SubTotal, QHeaderView::ResizeToContents);
}

void NoteTableWidget::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    QKeyEvent event2(event->type(),
                     Qt::Key_Tab,
                     event->modifiers(),
                     event->nativeScanCode(),
                     event->nativeVirtualKey(),
                     event->nativeModifiers(),
                     event->text(), event->isAutoRepeat(),
                     event->count());
    QTableWidget::keyPressEvent(&event2);
  }
  else
  {
    QTableWidget::keyPressEvent(event);
  }
}

QString NoteTableWidget::text(int row, int column) const
{
  QString str;
  switch((NoteColumn)column)
  {
    case NoteColumn::Ammount:
    case NoteColumn::UnitValue:
    case NoteColumn::SubTotal:
    {
      auto p = item(row, (int)column);
      if (p != nullptr)
        str = p->text();
    } break;
    case NoteColumn::Description:
    case NoteColumn::Unity:
    {
      QComboBox* pt = dynamic_cast<QComboBox*>(cellWidget(row, (int)column));
      if (pt != nullptr)
        str = pt->currentText();
    } break;
    default:
    {
      Q_ASSERT(false);
    } break;
  }
  return str;
}

void NoteTableWidget::setText(int row, int column, const QString& str)
{
  switch((NoteColumn)column)
  {
    case NoteColumn::Ammount:
    case NoteColumn::UnitValue:
    case NoteColumn::SubTotal:
    {
      auto p = item(row, column);
      if (p != nullptr)
        p->setText(str);
    } break;
    case NoteColumn::Description:
    case NoteColumn::Unity:
    {
      QComboBox* pt = dynamic_cast<QComboBox*>(cellWidget(row, column));
      if (pt != nullptr)
        pt->setCurrentText(str);
    } break;
    default:
    {
      Q_ASSERT(false);
    } break;
  }
}

QString NoteTableWidget::serializeItems() const
{
  QString str;
  for (int i = 0; i != rowCount(); ++i)
    for (int j = 0; j!= columnCount(); ++j)
      str += text(i, j) + ";";

  if (!str.isEmpty())
    str.chop(1);

  return str;
}

NoteWidget::NoteWidget(QWidget *parent)
  : QFrame(parent)
  , ui(new Ui::NoteWidget)
  , m_supplier(NoteComboBox::Supplier)
{
  ui->setupUi(this);
  ui->dockWidgetContents->layout()->addWidget(&m_historyWidget);
  QObject::connect(&m_table,
                   SIGNAL(cellChanged(int, int)),
                   this,
                   SLOT(updateTable(int, int)));

  QObject::connect(&m_supplier,
                   SIGNAL(editTextChanged(const QString&)),
                   this,
                   SLOT(changed()));

  QObject::connect(&m_supplier,
                   SIGNAL(supplierEnteredSignal()),
                   this,
                   SLOT(supplierEntered()));

  QObject::connect(&m_historyWidget,
                   SIGNAL(noteSelectedSignal(const Note&)),
                   this,
                   SLOT(setNote(const Note&)));

  ui->frameTable->layout()->addWidget(&m_table);
  ui->frameSupplier->layout()->addWidget(&m_supplier);
  ui->date->setDate(QDate::currentDate());
  ui->dockWidget->close();
}

NoteWidget::~NoteWidget()
{
  delete ui;
}

QString NoteWidget::computeUnitValue(int row) const
{
  Q_ASSERT(m_table.item(row, (int)NoteColumn::Ammount) != nullptr);
  Q_ASSERT(m_table.item(row, (int)NoteColumn::SubTotal) != nullptr);
  const double ammount = m_table.item(row, (int)NoteColumn::Ammount)->text().toDouble();
  const double subTotal = m_table.item(row, (int)NoteColumn::SubTotal)->text().toDouble();
  const double unitValue = ammount ? subTotal / ammount : 0.0;
  return Note::format(unitValue, false);
}

QString NoteWidget::computeSubTotal(int row) const
{
  Q_ASSERT(m_table.item(row, (int)NoteColumn::Ammount) != nullptr);
  Q_ASSERT(m_table.item(row, (int)NoteColumn::UnitValue) != nullptr);
  const double ammount = m_table.item(row, (int)NoteColumn::Ammount)->text().toDouble();
  const double unitValue = m_table.item(row, (int)NoteColumn::UnitValue)->text().toDouble();
  const double subTotal = ammount * unitValue;
  return Note::format(subTotal, false);
}

QString NoteWidget::computeTotal() const
{
  double total = 0.0;
  for (int row = 0; row != m_table.rowCount(); ++row)
  {
    Q_ASSERT(m_table.item(row, (int)NoteColumn::SubTotal) != nullptr);
    total += m_table.item(row, (int)NoteColumn::SubTotal)->text().toDouble();
  }
  return Note::format(total, false);
}

double NoteWidget::evaluate(int row, int column) const
{
  Q_ASSERT(m_table.item(row, column) != nullptr);

  auto pt = m_table.item(row, column);
  auto exp = pt->text().toStdString();
  int error = 0;
  double res = te_interp(exp.c_str(), &error);
  if (!error)
    pt->setData(Qt::UserRole, res);
  return pt->data(Qt::UserRole).toDouble();
}

void NoteWidget::updateTable(int row, int column)
{
  m_table.blockSignals(true);
  switch ((NoteColumn)column)
  {
    case NoteColumn::Ammount:
    {
      QString res(Note::format(evaluate(row, column), true));
      m_table.item(row, column)->setText(res);
      m_table.item(row, (int)NoteColumn::SubTotal)->setText(computeSubTotal(row));
      ui->total->setText(computeTotal());
    } break;
    case NoteColumn::UnitValue:
    {
      QString res(Note::format(evaluate(row, column), false));
      m_table.item(row, column)->setText(res);
      m_table.item(row, (int)NoteColumn::SubTotal)->setText(computeSubTotal(row));
      ui->total->setText(computeTotal());
    } break;
    case NoteColumn::SubTotal:
    {
      QString res(Note::format(evaluate(row, column), false));
      m_table.item(row, column)->setText(res);
      m_table.item(row, (int)NoteColumn::UnitValue)->setText(computeUnitValue(row));
      ui->total->setText(computeTotal());
    } break;
    default:
      break;
  }
  m_table.blockSignals(false);
  changed();
}

void NoteWidget::addItem()
{
  m_table.insertRow(m_table.rowCount());
  const int row = m_table.rowCount() - 1;
  auto unity = new NoteComboBox(NoteComboBox::TableUnity);
  QStringList ls;
  ls << "UN" << "KG" << "CX" << "FD" << "SC" << "ML" << "PCT";
  unity->addItems(ls);
  auto description = new NoteComboBox(NoteComboBox::TableDescription);
  description->addItems(m_descriptions);
  description->setCurrentText("");
  m_table.blockSignals(true);
  m_table.setCellWidget(row, (int)NoteColumn::Unity, unity);
  m_table.setCellWidget(row, (int)NoteColumn::Description, description);
  m_table.setItem(row, (int)NoteColumn::Ammount, new QTableWidgetItem("0.000"));
  m_table.setItem(row, (int)NoteColumn::UnitValue, new QTableWidgetItem("0.00"));
  m_table.setItem(row, (int)NoteColumn::SubTotal, new QTableWidgetItem("0.00"));
  m_table.blockSignals(false);
  m_table.setCurrentCell(row, (int)NoteColumn::Ammount);
  m_table.setFocus();
  ui->total->setText(computeTotal());
  emit changedSignal();
}

void NoteWidget::removeItem()
{
  m_table.removeRow(m_table.currentRow());
  if (m_table.rowCount() != 0)
    ui->total->setText(computeTotal());
  else
    ui->total->clear();
  changed();
}

void NoteWidget::changed()
{
  emit changedSignal();
}

bool NoteWidget::isItemSelected() const
{
  return m_table.currentRow() >= 0;
}

Note NoteWidget::getNote() const
{
  Note note;
  note.m_date = ui->date->date().toJulianDay();
  note.m_supplier = m_supplier.currentText();
  note.m_number = ui->number->text().toInt();
  note.m_total = ui->total->text().toDouble();
  note.m_items = m_table.serializeItems();
  return note;
}

void NoteWidget::setNote(const Note& note)
{
  clear();
  m_table.setRowCount(0);
  ui->date->setDate(QDate::fromJulianDay(note.m_date));
  m_supplier.setCurrentText(note.m_supplier);
  ui->number->setValue(note.m_number);
  NoteItems items(note.m_items);
  for (int row = 0; row != items.m_size; ++row)
  {
    addItem();
    m_table.setText(row, (int)NoteColumn::Ammount, items.at(row, NoteColumn::Ammount));
    m_table.setText(row, (int)NoteColumn::Unity, items.at(row, NoteColumn::Unity));
    m_table.setText(row, (int)NoteColumn::UnitValue, items.at(row, NoteColumn::UnitValue));
    m_table.setText(row, (int)NoteColumn::Description, items.at(row, NoteColumn::Description));
    m_table.setText(row, (int)NoteColumn::SubTotal, items.at(row, NoteColumn::SubTotal));
  }
}

bool NoteWidget::isValid() const
{
  return !m_supplier.currentText().isEmpty() &&
      m_table.rowCount() &&
      computeTotal().toDouble();
}

bool NoteWidget::isHistoryMode() const
{
  return false;
}

void NoteWidget::clear()
{
  ui->date->setSpecialValueText(" ");
  ui->date->setDate(ui->date->minimumDate());
  m_supplier.setCurrentText("");
  ui->number->clear();
  ui->total->setText("");
  m_table.setRowCount(0);
  while (m_supplier.count())
    m_supplier.removeItem(m_supplier.count() - 1);
}

void NoteWidget::create(int number,
                        const QStringList& suppliers,
                        const QStringList& descriptions)
{
  clear();
  ui->date->setDate(QDate::currentDate());
  ui->number->setValue(number);
  m_supplier.addItems(suppliers);
  m_supplier.setCurrentText("");
  m_supplier.setFocus();
  m_descriptions = descriptions;
}

void NoteWidget::setEnabled(bool bEnable)
{
  ui->date->setEnabled(bEnable);
  m_supplier.setEnabled(bEnable);
  ui->number->setEnabled(bEnable);
  ui->total->setEnabled(bEnable);
  m_table.setEnabled(bEnable);
}

QStringList NoteWidget::getItemDescriptions() const
{
  QStringList ls;
  for (auto row = 0; row != m_table.rowCount(); ++row)
    ls << m_table.text(row, (int)NoteColumn::Description);
  return ls;
}

void NoteWidget::supplierEntered()
{
  if (m_table.rowCount() != 0)
  {
    m_table.setCurrentCell(0, 0);
    m_table.setFocus();
  }
  else
  {
    addItem();
  }
}

void NoteWidget::setHistoryDatabase(const QSqlDatabase& sqldb)
{
  m_historyWidget.setDatabase(sqldb);
}

void NoteWidget::showHistory()
{
  if (ui->dockWidget->isHidden())
    ui->dockWidget->show();
}

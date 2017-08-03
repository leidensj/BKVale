#include "notewidget.h"
#include "ui_notewidget.h"
#include "tinyexpr.h"
#include "databaseutils.h"
#include <QComboBox>
#include <QStringList>
#include <QKeyEvent>

SupplierComboBox::SupplierComboBox()
{
  setEditable(true);
  auto f = font();
  f.setPointSize(12);
  setFont(f);
  lineEdit()->setPlaceholderText(tr("FORNECEDOR"));

  connect(this,
          SIGNAL(editTextChanged(const QString &)),
          this,
          SLOT(toUpper()));
}

void SupplierComboBox::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    emit supplierEnteredSignal();
  else
    QComboBox::keyPressEvent(event);
}

void SupplierComboBox::toUpper()
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
  {
    QFont f = font();
    f.setPointSize(12);
    f.setCapitalization(QFont::AllUppercase);
    setFont(f);
  }
  {
    QFont f = horizontalHeader()->font();
    f.setPointSize(12);
    f.setCapitalization(QFont::Capitalize);
    horizontalHeader()->setFont(f);
  }

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

QString NoteTableWidget::text(int row, int column) const
{
  QString str;
  auto p = item(row, (int)column);
  if (p != nullptr)
    str = p->text();
  return str;
}

void NoteTableWidget::setText(int row, int column, const QString& str)
{
  auto p = item(row, column);
  if (p != nullptr)
    p->setText(str);
}

NoteWidget::NoteWidget(QWidget *parent)
  : QFrame(parent)
  , ui(new Ui::NoteWidget)
  , currentNoteID(INVALID_ID)
{
  ui->setupUi(this);
  ui->dockWidgetContents->layout()->addWidget(&m_noteDatabaseWidget);
  QObject::connect(&m_table,
                   SIGNAL(cellChanged(int, int)),
                   this,
                   SLOT(updateTable(int, int)));

  QObject::connect(&m_supplier,
                   SIGNAL(editTextChanged(const QString&)),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(ui->buttonSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(showNoteDatabase()));

  QObject::connect(ui->buttonAdd,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(addItem()));

  QObject::connect(ui->buttonRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeItem()));

  QObject::connect(&m_supplier,
                   SIGNAL(supplierEnteredSignal()),
                   this,
                   SLOT(supplierEntered()));

  QObject::connect(&m_noteDatabaseWidget,
                   SIGNAL(noteSelectedSignal(const Note&)),
                   this,
                   SLOT(setNote(const Note&)));

  QObject::connect(ui->buttonNew,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));

  QObject::connect(&m_table,
                   SIGNAL(currentCellChanged(int, int, int, int)),
                   this,
                   SLOT(enableControls()));

  QObject::connect(&m_noteDatabaseWidget,
                   SIGNAL(noteRemovedSignal(int)),
                   this,
                   SLOT(noteRemoved(int)));

  ui->frameTable->layout()->addWidget(&m_table);
  ui->frameSupplier->layout()->addWidget(&m_supplier);
  ui->date->setDate(QDate::currentDate());
  ui->dockWidget->close();
  enableControls();
}

NoteWidget::~NoteWidget()
{
  delete ui;
}

QString NoteWidget::computeUnitValue(int row) const
{
  const double ammount = m_table.text(row, (int)NoteColumn::Ammount).toDouble();
  const double subTotal = m_table.text(row, (int)NoteColumn::SubTotal).toDouble();
  const double unitValue = ammount ? subTotal / ammount : 0.0;
  return Note::format(unitValue, false);
}

QString NoteWidget::computeSubTotal(int row) const
{
  const double ammount = m_table.text(row, (int)NoteColumn::Ammount).toDouble();
  const double unitValue = m_table.text(row, (int)NoteColumn::UnitValue).toDouble();
  const double subTotal = ammount * unitValue;
  return Note::format(subTotal, false);
}

QString NoteWidget::computeTotal() const
{
  double total = 0.0;
  for (int row = 0; row != m_table.rowCount(); ++row)
    total += m_table.text(row, (int)NoteColumn::SubTotal).toDouble();
  return Note::format(total, false);
}

double NoteWidget::evaluate(int row, int column) const
{
  auto pt = m_table.item(row, column);
  if (pt == nullptr)
    return 0.0;
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
    case NoteColumn::Unity:
    case NoteColumn::Description:
    {
      QString str = m_table.text(row, column);
      m_table.setText(row, column, str.toUpper());
    } break;
    case NoteColumn::Ammount:
    {
      QString res(Note::format(evaluate(row, column), true));
      m_table.setText(row, column, res);
      m_table.setText(row, (int)NoteColumn::SubTotal, computeSubTotal(row));
      ui->total->setText(computeTotal());
    } break;
    case NoteColumn::UnitValue:
    {
      QString res(Note::format(evaluate(row, column), false));
      m_table.setText(row, column, res);
      m_table.setText(row, (int)NoteColumn::SubTotal, computeSubTotal(row));
      ui->total->setText(computeTotal());
    } break;
    case NoteColumn::SubTotal:
    {
      QString res(Note::format(evaluate(row, column), false));
      m_table.setText(row, column, res);
      m_table.setText(row, (int)NoteColumn::UnitValue, computeUnitValue(row));
      ui->total->setText(computeTotal());
    } break;
    default:
      break;
  }
  m_table.blockSignals(false);
  emitChangedSignal();
}

void NoteWidget::addItem()
{
  m_table.insertRow(m_table.rowCount());
  const int row = m_table.rowCount() - 1;

  m_table.blockSignals(true);
  m_table.setItem(row, (int)NoteColumn::Ammount, new QTableWidgetItem("0.000"));
  m_table.setItem(row, (int)NoteColumn::Unity, new QTableWidgetItem(""));
  m_table.setItem(row, (int)NoteColumn::Description, new QTableWidgetItem(""));
  m_table.setItem(row, (int)NoteColumn::UnitValue, new QTableWidgetItem("0.00"));
  m_table.setItem(row, (int)NoteColumn::SubTotal, new QTableWidgetItem("0.00"));
  m_table.setCurrentCell(row, (int)NoteColumn::Ammount);
  m_table.setFocus();
  m_table.blockSignals(false);
  ui->total->setText(computeTotal());
  enableControls();
  emitChangedSignal();
}

void NoteWidget::removeItem()
{
  m_table.removeRow(m_table.currentRow());
  if (m_table.rowCount() != 0)
  {
    ui->total->setText(computeTotal());
  }
  else
  {
    ui->total->clear();
    m_supplier.setFocus();
  }
  enableControls();
  emitChangedSignal();
}

void NoteWidget::emitChangedSignal()
{
  emit changedSignal();
}

Note NoteWidget::getNote() const
{
  Note note;
  note.m_id = currentNoteID;
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

  m_supplier.addItems(NoteDatabase::suppliers(m_noteDatabaseWidget.getDatabase()));
  m_supplier.setCurrentText("");

  currentNoteID = note.m_id;
  ui->date->setDate(QDate::fromJulianDay(note.m_date));
  m_supplier.setCurrentText(note.m_supplier);
  ui->number->setValue(note.m_number);
  NoteItems items(note.m_items);
  for (int row = 0; row != items.m_size; ++row)
  {
    addItem();
    for (int column = 0; column != NUMBER_OF_COLUMNS; ++column)
      m_table.setText(row, column, items.at(row, (NoteColumn)column));
  }

  enableControls();
}

bool NoteWidget::isValid() const
{
  return !m_supplier.currentText().isEmpty() &&
      m_table.rowCount() &&
      computeTotal().toDouble() &&
      ui->number->value() > 0;
}

void NoteWidget::clear()
{
  currentNoteID = INVALID_ID;
  ui->date->setSpecialValueText(" ");
  ui->date->setDate(ui->date->minimumDate());
  m_supplier.setCurrentText("");
  ui->number->setValue(0);
  ui->number->clear();
  ui->total->setText("");
  m_table.setRowCount(0);
  while (m_supplier.count())
    m_supplier.removeItem(m_supplier.count() - 1);
  enableControls();
}

void NoteWidget::create()
{
  clear();
  ui->date->setDate(QDate::currentDate());
  ui->number->setValue(NoteDatabase::nextNumber(m_noteDatabaseWidget.getDatabase()));
  m_supplier.addItems(NoteDatabase::suppliers(m_noteDatabaseWidget.getDatabase()));
  m_supplier.setCurrentText("");
  m_supplier.setFocus();
  enableControls();
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

void NoteWidget::setHistoryDatabase(QSqlDatabase db)
{
  m_noteDatabaseWidget.setDatabase(db);
}

void NoteWidget::showNoteDatabase()
{
  if (ui->dockWidget->isHidden())
    ui->dockWidget->show();
  else
    ui->dockWidget->close();
}

void NoteWidget::enableControls()
{
  const bool bCreated = ui->number->value() > 0;
  ui->buttonRemove->setEnabled(bCreated && m_table.currentRow() >= 0);
  ui->buttonAdd->setEnabled(bCreated);
  m_supplier.setEnabled(bCreated);
  ui->number->setEnabled(bCreated);
  ui->date->setEnabled(bCreated);
  m_table.setEnabled(bCreated);
  ui->total->setEnabled(bCreated);
}

bool NoteWidget::save(QString& error)
{
  error.clear();

  Q_ASSERT(isValid());
  if (!isValid())
    return false;

  bool bSuccess = NoteDatabase::insertOrUpdate(m_noteDatabaseWidget.getDatabase(),
                                               getNote(),
                                               error);

  if (bSuccess)
    m_noteDatabaseWidget.refresh();

  return bSuccess;

}

void NoteWidget::noteRemoved(int id)
{
  if (id == currentNoteID)
    clear();
}

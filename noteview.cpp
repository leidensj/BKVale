#include "noteview.h"
#include "tinyexpr.h"
#include "databaseutils.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QLayout>
#include <QHeaderView>
#include <QDateEdit>
#include <QSpinBox>

NoteSupplierComboBox::NoteSupplierComboBox(QWidget* parent)
  : QComboBox(parent)
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

void NoteSupplierComboBox::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    emit supplierEnteredSignal();
  else
    QComboBox::keyPressEvent(event);
}

void NoteSupplierComboBox::toUpper()
{
  blockSignals(true);
  setCurrentText(currentText().toUpper());
  blockSignals(false);
}

NoteTableWidget::NoteTableWidget(QWidget* parent)
  : QTableWidget(parent)
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

NoteView::NoteView(QWidget *parent)
  : QFrame(parent)
  , m_btnCreate(nullptr)
  , m_btnOpenLast(nullptr)
  , m_btnSearch(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_snNumber(nullptr)
  , m_lblNumberStatus(nullptr)
  , m_dtDate(nullptr)
  , m_lblDateStatus(nullptr)
  , m_edTotal(nullptr)
  , m_cbSupplier(nullptr)
  , m_table(nullptr)
  , m_currentID(INVALID_NOTE_ID)
  , m_lastID(INVALID_NOTE_ID)
{
  m_btnCreate = new QPushButton();
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));

  m_btnOpenLast = new QPushButton();
  m_btnOpenLast->setFlat(true);
  m_btnOpenLast->setText("");
  m_btnOpenLast->setIconSize(QSize(24, 24));
  m_btnOpenLast->setIcon(QIcon(":/icons/res/openlast.png"));

  m_btnSearch = new QPushButton();
  m_btnSearch->setFlat(true);
  m_btnSearch->setText("");
  m_btnSearch->setIconSize(QSize(24, 24));
  m_btnSearch->setIcon(QIcon(":/icons/res/search.png"));

  m_btnAdd = new QPushButton();
  m_btnAdd->setFlat(true);
  m_btnAdd->setText("");
  m_btnAdd->setIconSize(QSize(24, 24));
  m_btnAdd->setIcon(QIcon(":/icons/res/add.png"));

  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/remove.png"));

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_btnCreate);
  hlayout1->addWidget(m_btnOpenLast);
  hlayout1->addWidget(m_btnSearch);
  hlayout1->addWidget(m_btnAdd);
  hlayout1->addWidget(m_btnRemove);

  QLabel* lblNumber = new QLabel();
  lblNumber->setText(tr("Número:"));
  {
    QFont font = lblNumber->font();
    font.setPointSize(12);
    lblNumber->setFont(font);
  }

  m_snNumber = new QSpinBox();
  m_snNumber->setReadOnly(true);
  m_snNumber->setButtonSymbols(QSpinBox::ButtonSymbols::NoButtons);
  m_snNumber->setMaximum(999999);
  m_snNumber->setMinimum(-1);
  m_snNumber->setMinimumSize(90, 0);
  m_snNumber->setAlignment(Qt::AlignRight);
  m_snNumber->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
  {
    QFont font = m_snNumber->font();
    font.setBold(true);
    m_snNumber->setFont(font);
    QPalette palette = m_snNumber->palette();
    palette.setColor(QPalette::ColorRole::Text, Qt::red);
    m_snNumber->setPalette(palette);
  }

  m_lblNumberStatus = new QLabel();
  m_lblNumberStatus->setText("");
  m_lblNumberStatus->setPixmap(QPixmap(":/icons/res/filenew.png"));
  m_lblNumberStatus->setScaledContents(true);
  m_lblNumberStatus->setMinimumSize(24, 24);
  m_lblNumberStatus->setMaximumSize(24, 24);

  QLabel* lblDate = new QLabel();
  lblDate->setText(tr("Data:"));
  {
    QFont font = lblDate->font();
    font.setPointSize(12);
    lblDate->setFont(font);
  }

  m_dtDate = new QDateEdit();
  m_dtDate->setCalendarPopup(true);
  m_dtDate->setDisplayFormat("dd/MM/yyyy");
  m_dtDate->setDate(QDate::currentDate());

  m_lblDateStatus = new QLabel();
  m_lblDateStatus->setText("");
  m_lblDateStatus->setPixmap(QPixmap(":/icons/res/calendarok.png"));
  m_lblDateStatus->setScaledContents(true);
  m_lblDateStatus->setMinimumSize(24, 24);
  m_lblDateStatus->setMaximumSize(24, 24);

  QHBoxLayout* hlayout2 = new QHBoxLayout();
  hlayout2->setContentsMargins(0, 0, 0, 0);
  hlayout2->setAlignment(Qt::AlignLeft);
  hlayout2->addWidget(lblNumber);
  hlayout2->addWidget(m_snNumber);
  hlayout2->addWidget(m_lblNumberStatus);
  hlayout2->addWidget(lblDate);
  hlayout2->addWidget(m_dtDate);
  hlayout2->addWidget(m_lblDateStatus);

  m_cbSupplier = new NoteSupplierComboBox();

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->addLayout(hlayout2);
  vlayout1->addWidget(m_cbSupplier);

  QFrame* frame = new QFrame();
  frame->setFrameShape(QFrame::Shape::StyledPanel);
  frame->setFrameShadow(QFrame::Shadow::Plain);
  frame->setLayout(vlayout1);

  m_table = new NoteTableWidget();

  m_edTotal = new QLineEdit();
  m_edTotal->setReadOnly(true);
  m_edTotal->setPlaceholderText(tr("TOTAL"));
  m_edTotal->setAlignment(Qt::AlignRight);
  m_edTotal->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  {
    QFont font = m_edTotal->font();
    font.setBold(true);
    m_edTotal->setFont(font);
    QPalette palette = m_edTotal->palette();
    palette.setColor(QPalette::ColorRole::Text, Qt::red);
    m_edTotal->setPalette(palette);
  }

  QHBoxLayout* hlayout3 = new QHBoxLayout();
  hlayout3->setContentsMargins(0, 0, 0, 0);
  hlayout3->setAlignment(Qt::AlignRight);
  hlayout3->addWidget(m_edTotal);

  QVBoxLayout* vlayout2 = new QVBoxLayout();
  vlayout2->setContentsMargins(0, 0, 0, 0);
  vlayout2->addLayout(hlayout1);
  vlayout2->addWidget(frame);
  vlayout2->addWidget(m_table);
  vlayout2->addLayout(hlayout3);
  setLayout(vlayout2);

  QObject::connect(m_table,
                   SIGNAL(cellChanged(int, int)),
                   this,
                   SLOT(updateTable(int, int)));

  QObject::connect(m_cbSupplier,
                   SIGNAL(editTextChanged(const QString&)),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(m_btnSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitShowSearchSignal()));

  QObject::connect(m_btnAdd,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(addItem()));

  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeItem()));

  QObject::connect(m_cbSupplier,
                   SIGNAL(supplierEnteredSignal()),
                   this,
                   SLOT(supplierEntered()));

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitCreateSignal()));

  QObject::connect(m_table,
                   SIGNAL(currentCellChanged(int, int, int, int)),
                   this,
                   SLOT(enableControls()));

  QObject::connect(m_btnOpenLast,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitOpenLastSignal()));

  QObject::connect(m_dtDate,
                   SIGNAL(dateChanged(const QDate&)),
                   this,
                   SLOT(enableControls()));

  create(0, QStringList());
  enableControls();
}

NoteView::~NoteView()
{

}

QString NoteView::computeUnitValue(int row) const
{
  const double ammount = m_table->text(row, (int)NoteColumn::Ammount).toDouble();
  const double subTotal = m_table->text(row, (int)NoteColumn::SubTotal).toDouble();
  const double unitValue = ammount ? subTotal / ammount : 0.0;
  return Note::format(unitValue, false);
}

QString NoteView::computeSubTotal(int row) const
{
  const double ammount = m_table->text(row, (int)NoteColumn::Ammount).toDouble();
  const double unitValue = m_table->text(row, (int)NoteColumn::UnitValue).toDouble();
  const double subTotal = ammount * unitValue;
  return Note::format(subTotal, false);
}

QString NoteView::computeTotal() const
{
  double total = 0.0;
  for (int row = 0; row != m_table->rowCount(); ++row)
    total += m_table->text(row, (int)NoteColumn::SubTotal).toDouble();
  return Note::format(total, false);
}

double NoteView::evaluate(int row, int column) const
{
  auto pt = m_table->item(row, column);
  if (pt == nullptr)
    return 0.0;
  auto exp = pt->text().toStdString();
  int error = 0;
  double res = te_interp(exp.c_str(), &error);
  if (!error)
    pt->setData(Qt::UserRole, res);
  return pt->data(Qt::UserRole).toDouble();
}

void NoteView::updateTable(int row, int column)
{
  m_table->blockSignals(true);
  switch ((NoteColumn)column)
  {
    case NoteColumn::Unity:
    case NoteColumn::Description:
    {
      QString str = m_table->text(row, column);
      m_table->setText(row, column, str.toUpper());
    } break;
    case NoteColumn::Ammount:
    {
      QString res(Note::format(evaluate(row, column), true));
      m_table->setText(row, column, res);
      m_table->setText(row, (int)NoteColumn::SubTotal, computeSubTotal(row));
      m_edTotal->setText(computeTotal());
    } break;
    case NoteColumn::UnitValue:
    {
      QString res(Note::format(evaluate(row, column), false));
      m_table->setText(row, column, res);
      m_table->setText(row, (int)NoteColumn::SubTotal, computeSubTotal(row));
      m_edTotal->setText(computeTotal());
    } break;
    case NoteColumn::SubTotal:
    {
      QString res(Note::format(evaluate(row, column), false));
      m_table->setText(row, column, res);
      m_table->setText(row, (int)NoteColumn::UnitValue, computeUnitValue(row));
      m_edTotal->setText(computeTotal());
    } break;
    default:
      break;
  }
  m_table->blockSignals(false);
  emitChangedSignal();
}

void NoteView::addItem()
{
  m_table->insertRow(m_table->rowCount());
  const int row = m_table->rowCount() - 1;

  m_table->blockSignals(true);
  m_table->setItem(row, (int)NoteColumn::Ammount, new QTableWidgetItem("0.000"));
  m_table->setItem(row, (int)NoteColumn::Unity, new QTableWidgetItem(""));
  m_table->setItem(row, (int)NoteColumn::Description, new QTableWidgetItem(""));
  m_table->setItem(row, (int)NoteColumn::UnitValue, new QTableWidgetItem("0.00"));
  m_table->setItem(row, (int)NoteColumn::SubTotal, new QTableWidgetItem("0.00"));
  m_table->setCurrentCell(row, (int)NoteColumn::Ammount);
  m_table->setFocus();
  m_table->blockSignals(false);
  m_edTotal->setText(computeTotal());
  enableControls();
  emitChangedSignal();
}

void NoteView::removeItem()
{
  m_table->removeRow(m_table->currentRow());
  if (m_table->rowCount() != 0)
  {
    m_edTotal->setText(computeTotal());
  }
  else
  {
    m_edTotal->clear();
    m_cbSupplier->setFocus();
  }
  enableControls();
  emitChangedSignal();
}

void NoteView::emitChangedSignal()
{
  emit changedSignal();
}

Note NoteView::getNote() const
{
  Note note;
  note.m_id = m_currentID;
  note.m_date = m_dtDate->date().toJulianDay();
  note.m_supplier = m_cbSupplier->currentText();
  note.m_number = m_snNumber->value();
  note.m_total = m_edTotal->text().toDouble();
  note.m_items = m_table->serializeItems();
  return note;
}

void NoteView::setNote(const Note& note, const QStringList& suppliers)
{
  m_table->setRowCount(0);
  m_cbSupplier->clear();
  while (m_cbSupplier->count())
    m_cbSupplier->removeItem(m_cbSupplier->count() - 1);
  m_cbSupplier->addItems(suppliers);
  m_cbSupplier->setCurrentText("");

  m_currentID = note.m_id;
  m_dtDate->setDate(QDate::fromJulianDay(note.m_date));
  m_cbSupplier->setCurrentText(note.m_supplier);
  m_snNumber->setValue(note.m_number);
  NoteItems items(note.m_items);
  for (int row = 0; row != items.m_size; ++row)
  {
    addItem();
    for (int column = 0; column != NUMBER_OF_COLUMNS; ++column)
      m_table->setText(row, column, items.at(row, (NoteColumn)column));
  }

  enableControls();
}

bool NoteView::isValid() const
{
  return !m_cbSupplier->currentText().isEmpty() &&
      m_table->rowCount() &&
      computeTotal().toDouble() &&
      m_snNumber->value() > 0;
}

void NoteView::create(int number, const QStringList& suppliers)
{
  m_currentID = INVALID_NOTE_ID;
  m_dtDate->setDate(QDate::currentDate());
  m_snNumber->setValue(number);
  m_edTotal->setText("");
  m_table->setRowCount(0);
  m_cbSupplier->clear();
  while (m_cbSupplier->count())
    m_cbSupplier->removeItem(m_cbSupplier->count() - 1);
  m_cbSupplier->addItems(suppliers);
  m_cbSupplier->setCurrentText("");
  m_cbSupplier->setFocus();
  enableControls();
}

QStringList NoteView::getItemDescriptions() const
{
  QStringList ls;
  for (auto row = 0; row != m_table->rowCount(); ++row)
    ls << m_table->text(row, (int)NoteColumn::Description);
  return ls;
}

void NoteView::supplierEntered()
{
  if (m_table->rowCount() != 0)
  {
    m_table->setCurrentCell(0, 0);
    m_table->setFocus();
  }
  else
  {
    addItem();
  }
}

void NoteView::enableControls()
{
  const bool bCreated = m_snNumber->value() > 0;
  m_btnRemove->setEnabled(bCreated && m_table->currentRow() >= 0);
  m_btnAdd->setEnabled(bCreated);
  m_cbSupplier->setEnabled(bCreated);
  m_snNumber->setEnabled(bCreated);
  m_dtDate->setEnabled(bCreated);
  m_table->setEnabled(bCreated);
  m_edTotal->setEnabled(bCreated);
  m_btnOpenLast->setEnabled(m_lastID != INVALID_NOTE_ID);
  m_lblNumberStatus->setPixmap(QPixmap(Note::isValidID(m_currentID)
                                     ? ":/icons/res/fileedit.png"
                                     : ":/icons/res/filenew.png"));
  m_lblDateStatus->setPixmap(QPixmap(m_dtDate->date() == QDate::currentDate()
                                     ? ":/icons/res/calendarok.png"
                                     : ":/icons/res/calendarwarning.png"));
}

void NoteView::emitShowSearchSignal()
{
  emit showSearchSignal();
}

void NoteView::emitCreateSignal()
{
  emit createSignal();
}

void NoteView::emitOpenLastSignal()
{
  if (m_lastID != INVALID_NOTE_ID)
    emit openLastSignal(m_lastID);
}

void NoteView::setLastID(int lastID)
{
  m_lastID = lastID;
  enableControls();
}

int NoteView::getLastID() const
{
  return m_lastID;
}

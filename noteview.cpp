#include "noteview.h"
#include "databaseutils.h"
#include "notetablewidget.h"
#include "form.h"
#include "jdatabasepicker.h"
#include "jdatabase.h"
#include "jlineedit.h"
#include "jdateedit.h"
#include "packageeditor.h"
#include "jdatepicker.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QSplitter>
#include <QMessageBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QRadioButton>
#include <QFormLayout>
#include "jplaintextedit.h"
#include <QDialogButtonBox>
#include "jitemhelper.h"
#include "jaddremovebuttons.h"
#include "jdoublespinbox.h"

NoteDetailsDlg::NoteDetailsDlg(QWidget* parent)
  : QDialog(parent)
  , m_teDetails(nullptr)
{
  m_teDetails = new JPlainTextEdit;
  m_teDetails->setPlaceholderText(tr("Shift+Enter para inserir uma quebra de linha"));
  QDialogButtonBox* btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(m_teDetails, SIGNAL(enterSignal()), this, SLOT(accept()));
  connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
  connect(btn, SIGNAL(rejected()), this, SLOT(reject()));

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addWidget(m_teDetails);
  lt->addWidget(btn);

  setWindowTitle(tr("Observações"));
  setWindowIcon(QIcon(":/icons/res/details.png"));

  setLayout(lt);
  m_teDetails->setFocus();
}

void NoteDetailsDlg::setDetails(const QString& str)
{
  m_teDetails->setPlainText(str);
}

QString NoteDetailsDlg::getDetails() const
{
  return m_teDetails->toPlainText();
}

PaymentDlg::PaymentDlg(QWidget* parent)
  : QDialog(parent)
  , m_noteTotal(0.0)
  , m_noteDate(QDate::currentDate())
{
  m_table = new QTableWidget;
  m_table->setColumnCount(3);
  QStringList headers;
  headers << "Número" << "Data" << "Valor";
  m_table->setHorizontalHeaderLabels(headers);
  m_table->horizontalHeader()->setSectionResizeMode((int)Column::Number, QHeaderView::ResizeMode::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)Column::Date, QHeaderView::ResizeMode::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)Column::Value, QHeaderView::ResizeMode::Stretch);

  m_rdoCredit = new QRadioButton;
  m_rdoCredit->setText(tr("Parcelado"));
  m_rdoCredit->setIcon(QIcon(":/icons/res/credit.png"));
  m_rdoCash = new QRadioButton;
  m_rdoCash->setText(tr("A vista"));
  m_rdoCash->setIcon(QIcon(":/icons/res/cash.png"));
  m_rdoBonus = new QRadioButton;
  m_rdoBonus->setText(tr("Bonificação"));
  m_rdoBonus->setIcon(QIcon(":/icons/res/bonus.png"));

  m_btnAddRemove = new JAddRemoveButtons;

  QDialogButtonBox* btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
  connect(btn, SIGNAL(rejected()), this, SLOT(reject()));

  QHBoxLayout* ltBtn = new QHBoxLayout;
  ltBtn->setContentsMargins(0, 0, 0, 0);
  ltBtn->setAlignment(Qt::AlignLeft);
  ltBtn->addWidget(m_rdoCredit);
  ltBtn->addWidget(m_rdoCash);
  ltBtn->addWidget(m_rdoBonus);

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addLayout(ltBtn);
  lt->addWidget(m_btnAddRemove);
  lt->addWidget(m_table);
  lt->addWidget(btn);

  setWindowTitle(tr("Pagamento"));
  setWindowIcon(QIcon(":/icons/res/payment.png"));

  connect(m_rdoCredit, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoCash, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoBonus, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_btnAddRemove->m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(addRow()));
  connect(m_btnAddRemove->m_btnRemove, SIGNAL(clicked(bool)), this, SLOT(removeRow()));
  connect(this, SIGNAL(isValidSignal(bool)), btn->button(QDialogButtonBox::Ok), SLOT(setEnabled(bool)));

  setLayout(lt);
  m_rdoCredit->setChecked(true);
  updateControls();
}

void PaymentDlg::updateControls()
{
  m_table->setEnabled(m_rdoCredit->isChecked());
  m_btnAddRemove->setEnabled(m_rdoCredit->isChecked());

  bool bValid = false;
  for (int i = 0; i != m_table->rowCount(); ++i)
  {
    auto dt = dynamic_cast<JDateEdit*>(m_table->cellWidget(i, (int)Column::Date));
    bValid = dt->date() >= m_noteDate;
    if (!bValid)
      break;
  }
  if (m_noteTotal != computeTotal())
    bValid = false;
  emit isValidSignal(bValid);
}

QVector<Payment> PaymentDlg::getPayments() const
{
  QVector<Payment> v;
  for (int i = 0; i != m_table->rowCount(); ++i)
  {
    auto dt = dynamic_cast<JDateEdit*>(m_table->cellWidget(i, (int)Column::Date));
    auto spn = dynamic_cast<JDoubleSpinBox*>(m_table->cellWidget(i, (int)Column::Value));
    Payment o;
    o.m_date = dt->date();
    o.m_value = spn->value();
    v.push_back(o);
  }
  return v;
}

void PaymentDlg::setPayment(const QVector<Payment>& v)
{
  m_table->setRowCount(0);
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    auto dt = dynamic_cast<JDateEdit*>(m_table->cellWidget(i, (int)Column::Date));
    auto spn = dynamic_cast<JDoubleSpinBox*>(m_table->cellWidget(i, (int)Column::Value));
    dt->setDate(v.at(i).m_date);
    spn->setValue(v.at(i).m_value);
  }
  updateControls();
}

void PaymentDlg::setNoteTotal(double total)
{
  m_noteTotal = total;
}

void PaymentDlg::setNoteDate(const QDate& date)
{
  m_noteDate = date;
}

void PaymentDlg::addRow()
{
  QTableWidgetItem* it = new QTableWidgetItem;
  it->setFlags(it->flags() & ~Qt::ItemIsEditable);
  JDoubleSpinBox* spn = new JDoubleSpinBox;
  spn->setMinimum(0.0);
  spn->setMaximum(999999999.00);
  spn->setValue(m_noteTotal > computeTotal() ? m_noteTotal - computeTotal() : 0.0);
  JDateEdit* dt = new JDateEdit;
  dt->setCalendarPopup(true);
  dt->setDate(m_noteDate.addMonths(1 + m_table->rowCount()));
  m_table->insertRow(m_table->rowCount());
  int row = m_table->rowCount() - 1;
  m_table->setItem(row, (int)Column::Number, it);
  m_table->setCellWidget(row, (int)Column::Date, dt);
  m_table->setCellWidget(row, (int)Column::Value, spn);
  for (int i = 0; i != m_table->rowCount(); ++i)
  {
    auto it = m_table->item(i, (int)Column::Number);
    it->setText(QString::number(i + 1) + "/" + QString::number(m_table->rowCount()));
  }
  updateControls();
}

void PaymentDlg::removeRow()
{
  if (m_table->currentRow() >= 0)
    m_table->removeRow(m_table->currentRow());
  updateControls();
}

double PaymentDlg::computeTotal() const
{
  double total = 0.0;
  for (int i = 0; i != m_table->rowCount(); ++i)
  {
    auto spn = dynamic_cast<JDoubleSpinBox*>(m_table->cellWidget(i, (int)Column::Value));
    total += spn->value();
  }
  return total;
}

Payment::Method PaymentDlg::getMethod() const
{
  if (m_rdoBonus->isChecked())
    return Payment::Method::Bonus;
  if (m_rdoCash->isChecked())
    return Payment::Method::Cash;
  return Payment::Method::Credit;
}

void PaymentDlg::setMethod(Payment::Method m)
{
  switch (m)
  {
    case Payment::Method::Cash:
      m_rdoCash->setChecked(true);
      break;
    case Payment::Method::Bonus:
      m_rdoBonus->setChecked(true);
      break;
    case Payment::Method::Credit:
    default:
      m_rdoCredit->setChecked(true);
      break;
  }
}

NoteView::NoteView(QWidget *parent)
  : QFrame(parent)
  , m_btnCreate(nullptr)
  , m_btnOpenLast(nullptr)
  , m_btnSearch(nullptr)
  , m_btnAddCode(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_snNumber(nullptr)
  , m_dtPicker(nullptr)
  , m_edTotal(nullptr)
  , m_supplierPicker(nullptr)
  , m_table(nullptr)
  , m_btnPayment(nullptr)
  , m_edDisccount(nullptr)
  , m_btnDetails(nullptr)
  , m_dlgDetails(nullptr)
  , m_dlgPayment(nullptr)
  , m_edEntries(nullptr)
  , m_edSum(nullptr)
  , m_dlgDb(nullptr)
{
  m_btnCreate = new QPushButton();
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
  m_btnCreate->setToolTip(tr("Novo vale (Ctrl+N)"));

  m_btnSearch = new QPushButton();
  m_btnSearch->setFlat(true);
  m_btnSearch->setText("");
  m_btnSearch->setIconSize(QSize(24, 24));
  m_btnSearch->setIcon(QIcon(":/icons/res/search.png"));
  m_btnSearch->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
  m_btnSearch->setToolTip(tr("Procurar vales (Ctrl+F)"));

  m_btnOpenLast = new QPushButton();
  m_btnOpenLast->setFlat(true);
  m_btnOpenLast->setText("");
  m_btnOpenLast->setIconSize(QSize(24, 24));
  m_btnOpenLast->setIcon(QIcon(":/icons/res/openlast.png"));
  m_btnOpenLast->setToolTip(tr("Abrir último vale"));

  QFrame* vFrame0 = new QFrame;
  vFrame0->setFrameShape(QFrame::VLine);

  m_btnAddCode = new QPushButton();
  m_btnAddCode->setFlat(true);
  m_btnAddCode->setText("");
  m_btnAddCode->setIconSize(QSize(24, 24));
  m_btnAddCode->setIcon(QIcon(":/icons/res/barcodescan.png"));
  m_btnAddCode->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Asterisk));
  m_btnAddCode->setToolTip(tr("Adicionar item (Alt+*)"));

  m_btnAdd = new QPushButton();
  m_btnAdd->setFlat(true);
  m_btnAdd->setText("");
  m_btnAdd->setIconSize(QSize(24, 24));
  m_btnAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAdd->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));
  m_btnAdd->setToolTip(tr("Adicionar item (Alt++)"));

  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Minus));
  m_btnRemove->setToolTip(tr("Remover item (Alt+-)"));

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_btnCreate);
  hlayout1->addWidget(m_btnSearch);
  hlayout1->addWidget(m_btnOpenLast);
  hlayout1->addWidget(vFrame0);
  hlayout1->addWidget(m_btnAddCode);
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
  m_snNumber->setMaximum(99999999);
  m_snNumber->setMinimum(0);
  m_snNumber->setSpecialValueText(tr("S/N"));
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

  m_dtPicker = new JDatePicker;

  QLabel* lblDate = new QLabel();
  lblDate->setText(tr("Data:"));
  {
    QFont font = lblDate->font();
    font.setPointSize(12);
    lblDate->setFont(font);
  }

  m_btnPayment = new QPushButton;
  m_btnPayment->setFlat(true);
  m_btnPayment->setToolTip(tr("Pagamento parcelado"));
  m_btnPayment->setIconSize(QSize(24,24));
  m_btnPayment->setIcon(QIcon(":/icons/res/credit.png"));

  m_dlgDetails = new NoteDetailsDlg(this);
  m_dlgPayment = new PaymentDlg(this);

  m_btnDetails = new QPushButton;
  m_btnDetails->setFlat(true);
  m_btnDetails->setText("");
  m_btnDetails->setIconSize(QSize(24, 24));
  m_btnDetails->setIcon(QIcon(":/icons/res/details.png"));
  m_btnDetails->setToolTip(tr("Observações"));

  QFrame* line1 = new QFrame;
  line1->setFrameShape(QFrame::VLine);

  QFrame* line2 = new QFrame;
  line2->setFrameShape(QFrame::VLine);

  QFrame* line3 = new QFrame;
  line3->setFrameShape(QFrame::VLine);

  QHBoxLayout* hlayout2 = new QHBoxLayout();
  hlayout2->setContentsMargins(0, 0, 0, 0);
  hlayout2->setAlignment(Qt::AlignLeft);
  hlayout2->addWidget(lblNumber);
  hlayout2->addWidget(m_snNumber);
  hlayout2->addWidget(line1);
  hlayout2->addWidget(lblDate);
  hlayout2->addWidget(m_dtPicker);
  hlayout2->addWidget(line2);
  hlayout2->addWidget(m_btnPayment);
  hlayout2->addWidget(line3);
  hlayout2->addWidget(m_btnDetails);

  m_supplierPicker = new JDatabasePicker(SUPPLIER_SQL_TABLE_NAME);
  m_supplierPicker->setPlaceholderText(true);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->addLayout(hlayout2);
  vlayout1->addWidget(m_supplierPicker);

  QFrame* frame = new QFrame();
  frame->setFrameShape(QFrame::Shape::StyledPanel);
  frame->setFrameShadow(QFrame::Shadow::Plain);
  frame->setLayout(vlayout1);
  frame->setFixedHeight(frame->sizeHint().height());

  m_table = new NoteTableWidget;

  m_edTotal = new JExpLineEdit(JItem::DataType::Money);
  m_edTotal->setReadOnly(true);
  m_edTotal->setPlaceholderText(tr("TOTAL"));
  m_edTotal->setAlignment(Qt::AlignRight);
  m_edTotal->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  {
    QFont font = m_edTotal->font();
    font.setBold(true);
    m_edTotal->setFont(font);
  }

  m_edDisccount = new JExpLineEdit(JItem::DataType::Money);
  m_edDisccount->setAlignment(Qt::AlignRight);
  m_edDisccount->setPlaceholderText(tr("Descontos ou acréscimos"));

  QHBoxLayout* totalLayout = new QHBoxLayout;
  totalLayout->setContentsMargins(0, 0, 0, 0);
  totalLayout->addWidget(m_edDisccount);
  totalLayout->addStretch();
  totalLayout->addWidget(m_edTotal);

  QHBoxLayout* headerlayout = new QHBoxLayout;
  headerlayout->setContentsMargins(0, 0, 0, 0);
  headerlayout->addWidget(frame);

  QVBoxLayout* viewLayout = new QVBoxLayout;
  viewLayout->addLayout(hlayout1);
  viewLayout->addLayout(headerlayout);
  viewLayout->addWidget(m_table);
  viewLayout->addLayout(totalLayout);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewLayout);

  m_database = new JDatabase(NOTE_SQL_TABLE_NAME);
  m_edEntries = new JLineEdit(JLineEdit::Input::All);
  m_edEntries->setReadOnly(true);
  m_edSum = new JLineEdit(JLineEdit::Input::All);
  m_edSum->setReadOnly(true);
  QFormLayout* ltDbInfo = new QFormLayout;
  ltDbInfo->addRow(tr("Número de vales:"), m_edEntries);
  ltDbInfo->addRow(tr("Soma dos vales:"), m_edSum);

  QFrame* frDbInfo = new QFrame;
  frDbInfo->setLayout(ltDbInfo);

  QTabWidget* tbdb = new QTabWidget;
  tbdb->addTab(m_database, QIcon(JItemHelper::icon(NOTE_SQL_TABLE_NAME)), JItemHelper::text(NOTE_SQL_TABLE_NAME));
  tbdb->addTab(frDbInfo, QIcon(":/icons/res/statistics.png"), tr("Estatísticas"));

  QVBoxLayout* ltdb = new QVBoxLayout;
  ltdb->addWidget(tbdb);

  m_dlgDb = new QDialog(this);
  m_dlgDb->setLayout(ltdb);
  m_dlgDb->setWindowFlags(Qt::Window);
  m_dlgDb->setWindowTitle(JItemHelper::text(NOTE_SQL_TABLE_NAME));
  m_dlgDb->setWindowIcon(QIcon(JItemHelper::icon(NOTE_SQL_TABLE_NAME)));
  m_dlgDb->setModal(true);

  setLayout(viewLayout);

  connect(m_database, SIGNAL(itemSelectedSignal(const JItemSQL&)), this, SLOT(itemSelected(const JItemSQL&)));
  connect(m_database, SIGNAL(itemsRemovedSignal(const QVector<Id>&)), this, SLOT(itemsRemoved(const QVector<Id>&)));
  connect(m_database, SIGNAL(refreshSignal()), this, SLOT(updateControls()));
  connect(m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(addProduct()));
  connect(m_btnAddCode, SIGNAL(clicked(bool)), this, SLOT(addProduct()));
  connect(m_btnRemove, SIGNAL(clicked(bool)), this, SLOT(removeItem()));
  connect(m_btnCreate, SIGNAL(clicked(bool)), this, SLOT(create()));
  connect(m_table, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_btnOpenLast, SIGNAL(clicked(bool)), this, SLOT(lastItemSelected()));
  connect(m_dtPicker, SIGNAL(dateChangedSignal()), this, SLOT(updateControls()));
  connect(m_btnPayment, SIGNAL(clicked(bool)), this, SLOT(openPaymentDialog()));
  connect(m_supplierPicker, SIGNAL(changedSignal()), this, SLOT(supplierChanged()));
  connect(m_edDisccount, SIGNAL(editingFinished()), this, SLOT(updateControls()));
  connect(m_edDisccount, SIGNAL(enterSignal()), m_table, SLOT(setFocus()));
  connect(m_btnDetails, SIGNAL(clicked(bool)), this, SLOT(openDetailsDialog()));
  connect(m_btnSearch, SIGNAL(clicked(bool)), m_dlgDb, SLOT(exec()));
  connect(m_database, SIGNAL(itemSelectedSignal(const JItemSQL&)), m_dlgDb, SLOT(accept()));

  create();
  updateControls();
}

NoteView::~NoteView()
{

}

void NoteView::removeItem()
{
  m_table->removeItem();
  if (!m_table->hasItems())
    m_supplierPicker->setFocus();
  updateControls();
}

Note NoteView::getNote() const
{
  Note note;
  note.m_id = m_currentId;
  note.m_date = m_dtPicker->getDate();
  note.m_supplier.m_id = m_supplierPicker->getId();
  note.m_paymentMethod = m_dlgPayment->getMethod();
  note.m_vPayment = m_dlgPayment->getPayments();
  note.m_observation = m_dlgDetails->getDetails();
  note.m_disccount = m_edDisccount->getValue();
  for (int i = 0; i != m_table->rowCount(); ++i)
    note.m_vNoteItem.push_back(dynamic_cast<const NoteItem&>(m_table->getItem(i)));
  return note;
}

void NoteView::setNote(const Note& note)
{
  m_currentId = note.m_id;
  m_table->removeAllItems();
  m_supplierPicker->clear();
  m_dtPicker->setDate(note.m_date);
  m_snNumber->setValue(note.m_number);
  for (int i = 0; i != note.m_vNoteItem.size(); ++i)
    m_table->addItem(note.m_vNoteItem.at(i));
  m_supplierPicker->setItem(note.m_supplier);
  m_dlgDetails->setDetails(note.m_observation);
  m_dlgPayment->setMethod(note.m_paymentMethod);
  m_dlgPayment->setPayment(note.m_vPayment);
  m_edDisccount->setText(note.m_disccount);
  updateControls();
}

void NoteView::create()
{
  Note note;
  setNote(note);
  updateControls();
}

void NoteView::supplierChanged()
{
  if (m_supplierPicker->getId().isValid())
  {
    if (m_table->hasItems())
    {
      m_table->setCurrentCell(0, 0);
      m_table->setFocus();
    }
    else if (!m_currentId.isValid())
      m_btnAdd->click();
  }
  updateControls();
}

void NoteView::updateControls()
{
  m_btnRemove->setEnabled(m_table->currentRow() >= 0);
  m_btnOpenLast->setEnabled(m_lastId.isValid());

  double total = m_table->computeTotal() + m_edDisccount->getValue();

  m_edTotal->setText(total);

  m_edEntries->setText(JItem::st_strInt(m_database->getNumberOfEntries()));
  m_edSum->setText(JItem::st_strMoney(m_database->getSum(5)));

  emit changedSignal();
}

void NoteView::addProduct()
{
  m_table->addItemAndLoadPrices(m_supplierPicker->getId(), sender() == m_btnAddCode);
}

Note NoteView::save()
{
  Note note = getNote();
  bool bSuccess = m_database->save(note);
  if (bSuccess)
  {
    m_lastId = note.m_id;
    QString error;
    note.SQL_select(error);
    create();
  }
  else
    note.clear();
  updateControls();
  return note;
}

void NoteView::lastItemSelected()
{
  if (m_lastId.isValid())
  m_database->selectItem(m_lastId);
}

void NoteView::itemSelected(const JItemSQL& jItem)
{
  const Note& note = dynamic_cast<const Note&>(jItem);
  m_lastId = note.m_id;
  setNote(note);
}

void NoteView::itemsRemoved(const QVector<Id>& ids)
{
  if (ids.contains(m_lastId))
    m_lastId.clear();
  if (ids.contains(m_currentId))
    create();
}

void NoteView::openDetailsDialog()
{
  QString str = m_dlgDetails->getDetails();
  if (!m_dlgDetails->exec())
    m_dlgDetails->setDetails(str);
}

void NoteView::openPaymentDialog()
{
  if (!m_dlgPayment->exec())
  {
    Note o = getNote();
    m_dlgPayment->setMethod(o.m_paymentMethod);
    m_dlgPayment->setPayment(o.m_vPayment);
  }
  updateControls();
}

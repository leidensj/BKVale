#include "buyview.h"
#include "databaseutils.h"
#include "note/notetablewidget.h"
#include "widgets/jdatabasepicker.h"
#include "widgets/jdatabase.h"
#include "widgets/jlineedit.h"
#include "widgets/jaddremovebuttons.h"
#include "widgets/jdoublespinbox.h"
#include "widgets/jtable.h"
#include "widgets/jtablewidgetitem.h"
#include "widgets/jdatepicker.h"
#include "packageeditor.h"
#include "widgets/jplaintextedit.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QRadioButton>
#include "items/jitemex.h"

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
  , m_rdoCash(nullptr)
  , m_rdoBonus(nullptr)
  , m_rdoCredit(nullptr)
  , m_tbCredit(nullptr)
  , m_btnAddRemove(nullptr)
  , m_noteTotal(0.0)
  , m_noteDate(QDate::currentDate())
{
  m_rdoCredit = new QRadioButton;
  m_rdoCredit->setIcon(QIcon(":/icons/res/credit.png"));
  m_rdoCredit->setText(tr("Crédito"));

  m_rdoCash = new QRadioButton;
  m_rdoCash->setIcon(QIcon(":/icons/res/cash.png"));
  m_rdoCash->setText(tr("A vista"));

  m_rdoBonus = new QRadioButton;
  m_rdoBonus->setIcon(QIcon(":/icons/res/bonus.png"));
  m_rdoBonus->setText(tr("Bonificação"));

  m_lblNoteTotal = new QLabel;
  m_lblPaymentTotal = new QLabel;

  QDialogButtonBox* btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
  connect(btn, SIGNAL(rejected()), this, SLOT(reject()));

  m_btnAddRemove = new JAddRemoveButtons;
  m_tbCredit = new JTable;
  m_tbCredit->setColumnCount(2);
  QStringList headers;
  headers << "Data" << "Valor";
  m_tbCredit->setHorizontalHeaderLabels(headers);
  m_tbCredit->horizontalHeader()->setSectionResizeMode((int)Column::Date, QHeaderView::ResizeMode::ResizeToContents);
  m_tbCredit->horizontalHeader()->setSectionResizeMode((int)Column::Value, QHeaderView::ResizeMode::Stretch);

  QHBoxLayout* ltButtons = new QHBoxLayout;
  ltButtons->addWidget(m_rdoCredit);
  ltButtons->addWidget(m_rdoCash);
  ltButtons->addWidget(m_rdoBonus);

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addLayout(ltButtons);
  lt->addWidget(m_btnAddRemove);
  lt->addWidget(m_tbCredit);
  lt->addWidget(m_lblNoteTotal);
  lt->addWidget(m_lblPaymentTotal);
  lt->addWidget(btn);

  setWindowTitle(tr("Pagamento"));

  connect(m_rdoCash, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoBonus, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoCash, SIGNAL(clicked(bool)), m_tbCredit, SLOT(removeAllItems()));
  connect(m_rdoBonus, SIGNAL(clicked(bool)), m_tbCredit, SLOT(removeAllItems()));
  connect(m_rdoCredit, SIGNAL(clicked(bool)), this, SLOT(fillCredit()));
  connect(m_btnAddRemove->m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(addRow()));
  connect(m_btnAddRemove->m_btnRemove, SIGNAL(clicked(bool)), this, SLOT(removeRow()));
  connect(m_tbCredit, SIGNAL(changedSignal(bool)), m_btnAddRemove, SLOT(enableRemoveButton(bool)));
  connect(m_tbCredit, SIGNAL(changedSignal(bool)), this, SLOT(updateControls()));
  connect(m_tbCredit, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateTable(QTableWidgetItem*)));
  connect(this, SIGNAL(isValidSignal(bool)), btn->button(QDialogButtonBox::Ok), SLOT(setEnabled(bool)));

  setLayout(lt);
  updateControls();
}

QIcon PaymentDlg::getIcon() const
{
  if (m_rdoBonus->isChecked())
    return QIcon(":/icons/res/bonus.png");
  if (m_rdoCash->isChecked())
    return QIcon(":/icons/res/cash.png");
  return QIcon(":/icons/res/credit.png");
}

QString PaymentDlg::getText() const
{
  if (m_rdoBonus->isChecked())
    return tr("Bonificação");
  if (m_rdoCash->isChecked())
    return tr("A vista");
  return tr("Crédito");
}

bool PaymentDlg::isDatesValid() const
{
  bool bValid = true;
  if (m_rdoCredit->isChecked())
    for (int i = 0; i != m_tbCredit->rowCount() && bValid; ++i)
      bValid = dynamic_cast<DateItem*>(m_tbCredit->item(i, (int)Column::Date))->getDate() >= m_noteDate;
  return bValid;
}

double PaymentDlg::computeTotal() const
{
  double total = 0.0;
  if (m_rdoCredit->isChecked())
    for (int i = 0; i != m_tbCredit->rowCount(); ++i)
      total += dynamic_cast<DoubleItem*>(m_tbCredit->item(i, (int)Column::Value))->getValue();
  return total;
}

void PaymentDlg::updateControls()
{
  m_tbCredit->setEnabled(m_rdoCredit->isChecked());
  m_btnAddRemove->setEnabled(m_rdoCredit->isChecked());
  double total = computeTotal();
  bool bValid = isDatesValid() && JItem::st_areEqual(m_noteTotal, total, JItem::DataType::Money);;
  m_lblNoteTotal->setText(tr("Total do vale: ") + JItem::st_strMoney(m_noteTotal));
  m_lblPaymentTotal->setText(("Total do pagamento: ") + JItem::st_strMoney(total));
  m_lblPaymentTotal->setVisible(m_rdoCredit->isChecked());
  setWindowIcon(getIcon());
  emit isValidSignal(bValid);
}

void PaymentDlg::updateTable(QTableWidgetItem* p)
{
  dynamic_cast<ExpItem*>(p)->evaluate();
}

void PaymentDlg::fillCredit()
{
  m_tbCredit->removeAllItems();
  addRow();
  updateControls();
}

Note::PaymentMethod PaymentDlg::getPaymentMethod() const
{
  if (m_rdoBonus->isChecked())
    return Note::PaymentMethod::Bonus;
  if (m_rdoCash->isChecked())
    return Note::PaymentMethod::Cash;
  return Note::PaymentMethod::Credit;
}

QVector<PaymentItem> PaymentDlg::getPaymentItems() const
{
  QVector<PaymentItem> v;
  if (m_rdoCredit->isChecked())
  {
    for (int i = 0; i != m_tbCredit->rowCount(); ++i)
    {
      PaymentItem o;
      o.m_value += dynamic_cast<DoubleItem*>(m_tbCredit->item(i, (int)Column::Value))->getValue();
      o.m_date = dynamic_cast<DateItem*>(m_tbCredit->item(i, (int)Column::Date))->getDate();
      v.push_back(o);
    }
  }
  return v;
}

void PaymentDlg::fillNote(Note& o) const
{
  o.m_paymentMethod = getPaymentMethod();
  o.m_vPaymentItem.clear();
  if (getPaymentMethod() == Note::PaymentMethod::Credit)
    o.m_vPaymentItem = getPaymentItems();
}

void PaymentDlg::setPaymentMethod(Note::PaymentMethod o)
{
  if (o == Note::PaymentMethod::Bonus)
    m_rdoBonus->setChecked(true);
  else if (o == Note::PaymentMethod::Cash)
    m_rdoCash->setChecked(true);
  else
    m_rdoCredit->setChecked(true);
}

void PaymentDlg::setPaymentItems(const QVector<PaymentItem>& v)
{
  m_tbCredit->removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    dynamic_cast<DoubleItem*>(m_tbCredit->item(i, (int)Column::Value))->setValue(v.at(i).m_value);
    dynamic_cast<DateItem*>(m_tbCredit->item(i, (int)Column::Date))->setDate(v.at(i).m_date);
  }
}

void PaymentDlg::setNote(const Note& o)
{
  m_noteDate = o.m_date;
  m_noteTotal = o.total();
  setPaymentMethod(o.m_paymentMethod);
  setPaymentItems(o.m_vPaymentItem);
  updateControls();
}

void PaymentDlg::addRow()
{
  m_tbCredit->insertRow(m_tbCredit->rowCount());
  int row = m_tbCredit->rowCount() - 1;

  auto itValue = new DoubleItem(JItem::DataType::Money, DoubleItem::Color::Foreground);
  auto itDate = new DateItem(m_noteDate, DateItem::Color::DateBeforeDefault);

  m_tbCredit->setItem(row, (int)Column::Date, itDate);
  m_tbCredit->setItem(row, (int)Column::Value, itValue);

  double total = computeTotal();
  double val = m_noteTotal > total ? m_noteTotal - total : 0.0;
  itValue->setValue(val);
  itDate->setDate(m_noteDate.addMonths(itDate->row() + 1));
  updateControls();
  m_tbCredit->setCurrentItem(itDate);
  m_tbCredit->setFocus();
}

void PaymentDlg::removeRow()
{
  if (m_tbCredit->currentRow() >= 0)
    m_tbCredit->removeRow(m_tbCredit->currentRow());
  updateControls();
}

BuyView::BuyView(QWidget *parent)
  : JItemView(NOTE_SQL_TABLE_NAME, parent)
  , m_btnOpenLast(nullptr)
  , m_btnAddCode(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_snNumber(nullptr)
  , m_dtPicker(nullptr)
  , m_edTotal(nullptr)
  , m_supplierPicker(nullptr)
  , m_table(nullptr)
  , m_edDisccount(nullptr)
  , m_dlgDetails(nullptr)
  , m_dlgPayment(nullptr)
  , m_edEntries(nullptr)
  , m_edSum(nullptr)
  , m_dlgDb(nullptr)
{
  m_btnSave->setEnabled(false);
  m_btnSave->hide();

  m_btnOpenLast = new QPushButton;
  m_btnOpenLast->setFlat(true);
  m_btnOpenLast->setText("");
  m_btnOpenLast->setIconSize(QSize(24, 24));
  m_btnOpenLast->setIcon(QIcon(":/icons/res/openlast.png"));
  m_btnOpenLast->setToolTip(tr("Abrir último vale"));

  m_ltButton->addWidget(m_btnOpenLast);

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

  m_dlgDetails = new NoteDetailsDlg(this);
  m_dlgPayment = new PaymentDlg(this);

  QFrame* line1 = new QFrame;
  line1->setFrameShape(QFrame::VLine);

  QFrame* line2 = new QFrame;
  line2->setFrameShape(QFrame::VLine);

  QHBoxLayout* ltCmd = new QHBoxLayout();
  ltCmd->setContentsMargins(0, 0, 0, 0);
  ltCmd->setAlignment(Qt::AlignLeft);
  ltCmd->addWidget(lblNumber);
  ltCmd->addWidget(m_snNumber);
  ltCmd->addWidget(line1);
  ltCmd->addWidget(lblDate);
  ltCmd->addWidget(m_dtPicker);
  ltCmd->addWidget(line2);
  ltCmd->addWidget(m_btnAddCode);
  ltCmd->addWidget(m_btnAdd);
  ltCmd->addWidget(m_btnRemove);

  m_supplierPicker = new JDatabasePicker(SUPPLIER_SQL_TABLE_NAME);
  m_supplierPicker->setPlaceholderText(true);

  QVBoxLayout* ltBuy = new QVBoxLayout();
  ltInfo->addLayout(ltCmd);
  ltInfo->addWidget(m_supplierPicker);

  QFrame* frInfo = new QFrame();
  frInfo->setFrameShape(QFrame::Shape::StyledPanel);
  frInfo->setFrameShadow(QFrame::Shadow::Plain);
  frInfo->setLayout(ltInfo);
  frInfo->setFixedHeight(frInfo->sizeHint().height());

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

  QHBoxLayout* ltTotal = new QHBoxLayout;
  ltTotal->setContentsMargins(0, 0, 0, 0);
  ltTotal->addWidget(m_edDisccount);
  ltTotal->addStretch();
  ltTotal->addWidget(m_edTotal);

  QVBoxLayout* ltBuy = new QVBoxLayout;
  ltBuy->addWidget(frInfo);
  ltBuy->addWidget(m_table);
  ltBuy->addLayout(ltTotal);

  QFrame* frBuy = new QFrame;
  frBuy->setLayout(ltBuy);

  m_tab->addTab(frBuy,
                QIcon(":/icons/res/details.png"),
                tr("Informações"));

  m_dlgDb->

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
  tbdb->addTab(m_database, QIcon(JItemEx::icon(NOTE_SQL_TABLE_NAME)), JItemEx::text(NOTE_SQL_TABLE_NAME));
  tbdb->addTab(frDbInfo, QIcon(":/icons/res/statistics.png"), tr("Estatísticas"));

  QVBoxLayout* ltdb = new QVBoxLayout;
  ltdb->addWidget(tbdb);

  m_dlgDb = new QDialog(this);
  m_dlgDb->setLayout(ltdb);
  m_dlgDb->setWindowFlags(Qt::Window);
  m_dlgDb->setWindowTitle(JItemEx::text(NOTE_SQL_TABLE_NAME));
  m_dlgDb->setWindowIcon(QIcon(JItemEx::icon(NOTE_SQL_TABLE_NAME)));
  m_dlgDb->setModal(true);

  setLayout(viewLayout);

  connect(m_database, SIGNAL(itemSelectedSignal(const JItemSQL&)), this, SLOT(itemSelected(const JItemSQL&)));
  connect(m_database, SIGNAL(itemsRemovedSignal(const QVector<Id>&)), this, SLOT(itemsRemoved(const QVector<Id>&)));
  connect(m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(addProduct()));
  connect(m_btnAddCode, SIGNAL(clicked(bool)), this, SLOT(addProduct()));
  connect(m_btnRemove, SIGNAL(clicked(bool)), this, SLOT(removeItem()));
  connect(m_btnCreate, SIGNAL(clicked(bool)), this, SLOT(create()));
  connect(m_table, SIGNAL(changedSignal(bool)), this, SLOT(updateControls()));
  connect(m_btnOpenLast, SIGNAL(clicked(bool)), this, SLOT(lastItemSelected()));
  connect(m_dtPicker, SIGNAL(dateChangedSignal()), this, SLOT(updateControls()));
  connect(m_btnPayment, SIGNAL(clicked(bool)), this, SLOT(openPaymentDialog()));
  connect(m_supplierPicker, SIGNAL(changedSignal()), this, SLOT(supplierChanged()));
  connect(m_edDisccount, SIGNAL(editingFinished()), this, SLOT(updateControls()));
  connect(m_edDisccount, SIGNAL(enterSignal()), m_table, SLOT(setFocus()));
  connect(m_btnDetails, SIGNAL(clicked(bool)), this, SLOT(openDetailsDialog()));
  connect(m_btnSearch, SIGNAL(clicked(bool)), m_dlgDb, SLOT(exec()));
  connect(m_database, SIGNAL(itemSelectedSignal(const JItemSQL&)), m_dlgDb, SLOT(accept()));
  connect(tbdb, SIGNAL(currentChanged(int)), this, SLOT(updateStatistics()));

  create();
  updateControls();
  updateStatistics();
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
  Note note = m_currentNote;
  note.clear(false);
  note.m_date = m_dtPicker->getDate();
  note.m_supplier.m_id = m_supplierPicker->getId();
  m_dlgPayment->fillNote(note);
  note.m_observation = m_dlgDetails->getDetails();
  note.m_disccount = m_edDisccount->getValue();
  for (int i = 0; i != m_table->rowCount(); ++i)
    note.m_vNoteItem.push_back(dynamic_cast<const NoteItem&>(m_table->getItem(i)));
  return note;
}

void NoteView::setNote(const Note& note)
{
  m_currentNote = note;
  m_table->removeAllItems();
  m_supplierPicker->clear();
  m_dtPicker->setDate(note.m_date);
  m_snNumber->setValue(note.m_number);
  for (int i = 0; i != note.m_vNoteItem.size(); ++i)
    m_table->addItem(note.m_vNoteItem.at(i));
  m_supplierPicker->setItem(note.m_supplier);
  m_dlgDetails->setDetails(note.m_observation);
  m_dlgPayment->setNote(note);
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
    else if (!m_currentNote.m_id.isValid())
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
  m_btnPayment->setIcon(m_dlgPayment->getIcon());
  m_btnPayment->setToolTip(m_dlgPayment->getText());
  emit changedSignal();
}

void NoteView::updateStatistics()
{
  m_edEntries->setText(JItem::st_strInt(m_database->getNumberOfEntries()));
  m_edSum->setText(JItem::st_strMoney(m_database->getSum(5)));
}

void NoteView::addProduct()
{
  m_table->addItemAndLoadPrices(m_supplierPicker->getId(), sender() == m_btnAddCode);
}

bool NoteView::save(Id& id)
{
  id.clear();
  Note o = getNote();

  // TODO por enquanto corrigimos o pagamento
  o.adjustPayment();

  if (!o.isPaymentValid())
  {
    QMessageBox::critical(this,
                          tr("Pagamento inconsistente"),
                          tr("O valor do pagamento é diferente do valor da nota."),
                          QMessageBox::Ok);
    return false;
  }

  bool bSuccess = m_database->save(o);
  if (bSuccess)
  {
    m_lastId = o.m_id;
    id = o.m_id;
    create();
  }
  updateControls();
  return bSuccess;
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
  if (ids.contains(m_currentNote.m_id))
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
  Note o = getNote();
  m_dlgPayment->setNote(o);
  if (!m_dlgPayment->exec())
    m_dlgPayment->setNote(o);
  updateControls();
}

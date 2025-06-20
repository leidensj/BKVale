#include "purchaseview.h"
#include "databaseutils.h"
#include "tables/purchaseproducttable.h"
#include "controls/databasepicker.h"
#include "controls/databaseviewer.h"
#include "controls/databaseselector.h"
#include "widgets/jlineedit.h"
#include "widgets/jaddremovebuttons.h"
#include "widgets/jdoublespinbox.h"
#include "widgets/jdatepicker.h"
#include "filters/purchasefilter.h"
#include "tables/paymenttable.h"
#include "escposprinter.h"
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QAction>
#include <QLabel>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QFormLayout>
#include <QRadioButton>
#include <QSettings>
#include <QToolButton>
#include <QDialogButtonBox>
#include "items/jitemhelper.h"

#define SETTINGS_PURCHASE_STORE_ID "purchase/storeid"

PaymentWidget::PaymentWidget(QWidget* parent)
  : QWidget(parent)
  , m_rdoCash(nullptr)
  , m_rdoBonus(nullptr)
  , m_rdoCredit(nullptr)
  , m_tbPayment(nullptr)
  , m_btnAddRemove(nullptr)
{
  m_rdoCredit = new QRadioButton;
  m_rdoCredit->setIcon(Purchase::st_paymentIcon(Purchase::PaymentMethod::Credit));
  m_rdoCredit->setText(Purchase::st_paymentText(Purchase::PaymentMethod::Credit));

  m_rdoCash = new QRadioButton;
  m_rdoCash->setIcon(Purchase::st_paymentIcon(Purchase::PaymentMethod::Cash));
  m_rdoCash->setText(Purchase::st_paymentText(Purchase::PaymentMethod::Cash));

  m_rdoBonus = new QRadioButton;
  m_rdoBonus->setIcon(Purchase::st_paymentIcon(Purchase::PaymentMethod::Bonus));
  m_rdoBonus->setText(Purchase::st_paymentText(Purchase::PaymentMethod::Bonus));

  m_lblPurchaseTotal = new QLabel;
  m_lblPaymentTotal = new QLabel;

  m_btnAddRemove = new JAddRemoveButtons;
  m_tbPayment = new PaymentTable(m_btnAddRemove);

  QHBoxLayout* ltButtons = new QHBoxLayout;
  ltButtons->setAlignment(Qt::AlignLeft);
  ltButtons->addWidget(m_rdoCredit);
  ltButtons->addWidget(m_rdoCash);
  ltButtons->addWidget(m_rdoBonus);

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addLayout(ltButtons);
  lt->addWidget(m_btnAddRemove);
  lt->addWidget(m_tbPayment);
  lt->addWidget(m_lblPurchaseTotal);
  lt->addWidget(m_lblPaymentTotal);

  connect(m_rdoCash, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoBonus, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoCash, SIGNAL(clicked(bool)), this, SLOT(emitMethodChangedSignal()));
  connect(m_rdoBonus, SIGNAL(clicked(bool)), this, SLOT(emitMethodChangedSignal()));
  connect(m_rdoCredit, SIGNAL(clicked(bool)), this, SLOT(emitMethodChangedSignal()));
  connect(m_rdoCash, SIGNAL(clicked(bool)), m_tbPayment, SLOT(removeAllItems()));
  connect(m_rdoBonus, SIGNAL(clicked(bool)), m_tbPayment, SLOT(removeAllItems()));
  connect(m_rdoCredit, SIGNAL(clicked(bool)), this, SLOT(fillCredit()));
  connect(m_tbPayment, SIGNAL(changedSignal(bool)), this, SLOT(updateControls()));

  setLayout(lt);
  updateControls();
}

QIcon PaymentWidget::getIcon() const
{
  if (m_rdoBonus->isChecked())
    return QIcon(":/icons/res/bonus.png");
  if (m_rdoCash->isChecked())
    return QIcon(":/icons/res/cash.png");
  return QIcon(":/icons/res/credit.png");
}

QString PaymentWidget::getText() const
{
  if (m_rdoBonus->isChecked())
    return tr("A prazo");
  if (m_rdoCash->isChecked())
    return tr("A vista");
  return tr("Crédito");
}

void PaymentWidget::updateControls()
{
  m_tbPayment->setEnabled(m_rdoCredit->isChecked());
  m_btnAddRemove->setEnabled(m_rdoCredit->isChecked());
  m_lblPurchaseTotal->setText(tr("Total da compra: ") + Data::strMoney(m_tbPayment->getPurchaseTotal()));
  m_lblPaymentTotal->setText(("Total do pagamento: ") + Data::strMoney(m_tbPayment->total()));
  m_lblPaymentTotal->setVisible(m_rdoCredit->isChecked());
  setWindowIcon(getIcon());
  emit isValidSignal(m_tbPayment->isValid());
}

void PaymentWidget::fillCredit()
{
  m_rdoCredit->setChecked(true);
  m_tbPayment->fill();
}

Purchase::PaymentMethod PaymentWidget::getPaymentMethod() const
{
  if (m_rdoBonus->isChecked())
    return Purchase::PaymentMethod::Bonus;
  if (m_rdoCash->isChecked())
    return Purchase::PaymentMethod::Cash;
  return Purchase::PaymentMethod::Credit;
}

QVector<PaymentPart> PaymentWidget::getPayments() const
{
  QVector<PaymentPart> v;
  m_tbPayment->get(v);
  return v;
}

void PaymentWidget::setPurchaseDate(const QDate& dt)
{
  m_tbPayment->setPurchaseDate(dt);
  updateControls();
}

void PaymentWidget::setPurchaseTotal(double total)
{
  m_tbPayment->setPurchaseTotal(total);
  updateControls();
}

void PaymentWidget::setPaymentMethod(Purchase::PaymentMethod o)
{
  if (o == Purchase::PaymentMethod::Bonus)
    m_rdoBonus->setChecked(true);
  else if (o == Purchase::PaymentMethod::Cash)
    m_rdoCash->setChecked(true);
  else
    m_rdoCredit->setChecked(true);
}

void PaymentWidget::setPayments(const QVector<PaymentPart>& v)
{
  m_tbPayment->set(v);
}

void PaymentWidget::emitMethodChangedSignal()
{
  emit methodChangedSignal();
}

PurchaseView::PurchaseView(QWidget *parent)
  : JItemView(PURCHASE_SQL_TABLE_NAME, parent)
  , m_btnOpenLast(nullptr)
  , m_snNumber(nullptr)
  , m_dtPicker(nullptr)
  , m_edTotal(nullptr)
  , m_supplierPicker(nullptr)
  , m_storePicker(nullptr)
  , m_table(nullptr)
  , m_edDisccount(nullptr)
  , m_wPayment(nullptr)
  , m_teObservation(nullptr)
  , m_filter(nullptr)
  , m_btnAddMenu(nullptr)
  , m_actAdd(nullptr)
  , m_actAddCode(nullptr)
  , m_actAddHistory(nullptr)
  , m_btnRemove(nullptr)
  , m_lblViewerData(nullptr)
{
  addViewButton(PRODUCT_SQL_TABLE_NAME);
  addViewButton(SUPPLIER_SQL_TABLE_NAME);

  m_btnPrint->setEnabled(true);
  m_btnPrint->show();

  m_btnOpenLast = new QPushButton;
  m_btnOpenLast->setFlat(true);
  m_btnOpenLast->setText("");
  m_btnOpenLast->setIconSize(QSize(24, 24));
  m_btnOpenLast->setIcon(QIcon(":/icons/res/openlast.png"));
  m_btnOpenLast->setToolTip(tr("Abrir último item"));

  m_ltButton->addWidget(m_btnOpenLast);

  m_btnAddMenu = new QToolButton;
  m_btnAddMenu->setIconSize(QSize(24, 24));
  m_btnAddMenu->setAutoRaise(true);
  m_btnAddMenu->setPopupMode(QToolButton::MenuButtonPopup);

  m_actAdd = new QAction(QIcon(":/icons/res/additem.png"), tr("Produto"), m_btnAddMenu);
  m_actAdd->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));
  m_actAdd->setToolTip(tr("Adicionar"));

  m_actAddCode = new QAction(QIcon(":/icons/res/barcode.png"), tr("Código"), m_btnAddMenu);
  m_actAddCode->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Asterisk));

  m_actAddHistory = new QAction(QIcon(":/icons/res/history.png"), tr("Histório"), m_btnAddMenu);
  m_actAddHistory->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Period));

  m_btnAddMenu->addAction(m_actAdd);
  m_btnAddMenu->addAction(m_actAddCode);
  m_btnAddMenu->addAction(m_actAddHistory);
  m_btnAddMenu->setDefaultAction(m_actAdd);

  m_btnRemove = new QPushButton;
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Minus));
  m_btnRemove->setToolTip(tr("Remover (Alt+-)"));

  m_table = new PurchaseProductTable;
  m_table->setLargerSize(true);

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
  m_snNumber->setSpecialValueText(tr("-"));
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
  ltCmd->addWidget(m_btnAddMenu);
  ltCmd->addWidget(m_btnRemove);

  m_supplierPicker = new DatabasePicker(SUPPLIER_SQL_TABLE_NAME);
  m_supplierPicker->setPlaceholderText(true);

  m_storePicker = new DatabasePicker(STORE_SQL_TABLE_NAME);
  m_storePicker->setPlaceholderText(true);

  QVBoxLayout* ltHeader = new QVBoxLayout;
  ltHeader->addLayout(ltCmd);
  ltHeader->addWidget(m_storePicker);
  ltHeader->addWidget(m_supplierPicker);

  QFrame* frHeader = new QFrame();
  frHeader->setFrameShape(QFrame::Shape::StyledPanel);
  frHeader->setFrameShadow(QFrame::Shadow::Plain);
  frHeader->setLayout(ltHeader);
  frHeader->setFixedHeight(frHeader->sizeHint().height());

  m_edTotal = new JExpLineEdit(Data::Type::Money);
  m_edTotal->setReadOnly(true);
  m_edTotal->setPlaceholderText(tr("TOTAL"));
  m_edTotal->setAlignment(Qt::AlignRight);
  m_edTotal->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  {
    QFont font = m_edTotal->font();
    font.setBold(true);
    m_edTotal->setFont(font);
  }

  m_edDisccount = new JExpLineEdit(Data::Type::Money);
  m_edDisccount->setAlignment(Qt::AlignRight);
  m_edDisccount->setPlaceholderText(tr("Descontos ou acréscimos"));

  QHBoxLayout* ltTotal = new QHBoxLayout;
  ltTotal->setContentsMargins(0, 0, 0, 0);
  ltTotal->addWidget(m_edDisccount);
  ltTotal->addStretch();
  ltTotal->addWidget(m_edTotal);

  QVBoxLayout* ltInfo = new QVBoxLayout;
  ltInfo->addWidget(frHeader);
  ltInfo->addWidget(m_table);
  ltInfo->addLayout(ltTotal);

  QFrame* frInfo = new QFrame;
  frInfo->setLayout(ltInfo);

  m_teObservation = new QPlainTextEdit;
  QVBoxLayout* ltObservation = new QVBoxLayout;
  ltObservation->addWidget(m_teObservation);
  QFrame* frObservation = new QFrame;
  frObservation->setLayout(ltObservation);

  m_wPayment = new PaymentWidget(this);

  m_tab->addTab(frInfo, QIcon(":/icons/res/details.png"), tr("Informações"));
  m_tab->addTab(m_wPayment, m_wPayment->getIcon(), tr("Pagamento"));
  m_tab->addTab(frObservation, QIcon(":/icons/res/pencil.png"), tr("Observações"));

  m_lblViewerData = new QLabel;
  m_viewer->layout()->addWidget(m_lblViewerData);
  m_filter = new PurchaseFilter;
  m_tabDb->addTab(m_filter, QIcon(":/icons/res/filter.png"), tr("Filtro"));

  setContentsMargins(9, 9, 9, 9);
  connect(m_actAdd, SIGNAL(triggered(bool)), m_table, SLOT(addRowAndActivate()));
  connect(m_actAddCode, SIGNAL(triggered(bool)), m_table, SLOT(addRowByCode()));
  connect(m_actAddHistory, SIGNAL(triggered(bool)), this, SLOT(showHistory()));
  connect(m_btnRemove, SIGNAL(clicked(bool)), m_table, SLOT(removeItem()));
  connect(m_table, SIGNAL(changedSignal(bool)), m_btnRemove, SLOT(setEnabled(bool)));
  connect(m_table, SIGNAL(changedSignal(bool)), this, SLOT(updateControls()));
  connect(m_btnOpenLast, SIGNAL(clicked(bool)), this, SLOT(lastItemSelected()));
  connect(m_supplierPicker, SIGNAL(changedSignal()), this, SLOT(supplierChanged()));
  connect(m_edDisccount, SIGNAL(valueChanged(double)), this, SLOT(updateControls()));
  connect(m_edDisccount, SIGNAL(enterSignal()), m_table, SLOT(setFocus()));
  connect(m_filter, SIGNAL(filterChangedSignal(const QString&)), m_viewer, SLOT(setDynamicFilter(const QString&)));
  connect(m_wPayment, SIGNAL(methodChangedSignal()), this, SLOT(updateControls()));
  connect(m_edTotal, SIGNAL(valueChanged(double)), m_wPayment, SLOT(setPurchaseTotal(double)));
  connect(m_dtPicker, SIGNAL(dateChangedSignal(const QDate&)), m_wPayment, SLOT(setPurchaseDate(const QDate&)));
  connect(this, SIGNAL(itemSelectedSignal()), SLOT(updateControls()));
  connect(m_viewer, &DatabaseViewer::refreshSignal,
          [this](){ m_lblViewerData->setText(tr("Número de compras: %1   Total das compras: %2").arg(
                                               Data::strInt(m_viewer->getRowCount()),
                                               Data::strMoney(m_viewer->getSum(5)))); });

  setFocusWidgetOnClear(m_supplierPicker);
  clear();
  m_viewer->refresh();
  updateControls();
}

PurchaseView::~PurchaseView()
{

}

void PurchaseView::getItem(JItemSQL& o) const
{
  Purchase& _o = dynamic_cast<Purchase&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_date = m_dtPicker->getDate();
  _o.m_supplier.m_id = m_supplierPicker->getFirstId();
  _o.m_paymentMethod = m_wPayment->getPaymentMethod();
  _o.m_payments = m_wPayment->getPayments();
  _o.m_observation = m_teObservation->toPlainText();
  _o.m_disccount = m_edDisccount->value();
  _o.m_store.m_id = m_storePicker->getFirstId();
  m_table->get(_o.m_products);
}

void PurchaseView::setItem(const JItemSQL& o)
{
  m_table->removeAllItems();
  m_supplierPicker->clear();
  m_storePicker->clear();
  const Purchase& _o = dynamic_cast<const Purchase&>(o);
  if (!_o.m_id.isValid() && !_o.m_store.m_id.isValid())
  {
    QSettings settings(SETTINGS_COMPANY_NAME, SETTINGS_APP_NAME);
    Store store(Id(settings.value(SETTINGS_PURCHASE_STORE_ID).toLongLong()));
    QString error;
    if (store.SQL_select(error))
      m_storePicker->addItem(store);
  }
  else
    m_storePicker->addItem(_o.m_store);
  m_dtPicker->setDate(_o.m_date);
  m_snNumber->setValue(_o.m_number);
  m_table->setNewPurchase(!_o.m_id.isValid());
  m_table->set(_o.m_products);
  m_supplierPicker->addItem(_o.m_supplier);
  m_teObservation->setPlainText(_o.m_observation);
  m_edDisccount->setValue(_o.m_disccount);

  m_wPayment->setPurchaseDate(m_dtPicker->getDate());
  m_wPayment->setPurchaseTotal(m_edTotal->value());
  m_wPayment->setPaymentMethod(_o.m_paymentMethod);
  m_wPayment->setPayments(_o.m_payments);
  updateControls();
}

void PurchaseView::supplierChanged()
{
  m_table->setSupplierId(m_supplierPicker->getFirstId());
  updateControls();
}

void PurchaseView::updateControls()
{
  m_btnOpenLast->setEnabled(m_lastId.isValid());
  double total = m_table->sum((int)PurchaseProductTable::Column::SubTotal) + m_edDisccount->value();
  m_edTotal->setValue(total);
  m_tab->setTabIcon(1, m_wPayment->getIcon());
  m_actAddHistory->setEnabled(m_supplierPicker->getFirstId().isValid());
}

void PurchaseView::save()
{
  Purchase o;
  getItem(o);

  if (o.m_date != DateTime::server().date() && !o.m_id.isValid())
  {
    if (QMessageBox::question(
          this,
          tr("Data"),
          tr("A data informada é diferente da data atual.\nDeseja usar a data atual?"),
          QMessageBox::Yes | QMessageBox::No,
          QMessageBox::Yes) == QMessageBox::Yes)
    {
       o.m_date = DateTime::server().date();
    }
  }

  // TODO por enquanto corrigimos o pagamento
  o.adjustPayment();

  if (!o.isPaymentValid())
  {
    QMessageBox::critical(this,
                          tr("Pagamento inconsistente"),
                          tr("O valor do pagamento é diferente do valor da compra."),
                          QMessageBox::Ok);
    return;
  }

  bool bSuccess = JItemHelper::save(o, this);
  if (bSuccess)
  {
    print(o);
    if (o.m_store.m_id.isValid())
    {
      QSettings settings(SETTINGS_COMPANY_NAME, SETTINGS_APP_NAME);
      settings.setValue(SETTINGS_PURCHASE_STORE_ID, o.m_store.m_id.get());
    }
    m_viewer->refresh();
    m_lastId = o.m_id;
    clear();
  }
  updateControls();
}

void PurchaseView::lastItemSelected()
{
  if (m_lastId.isValid())
    JItemView::setItem(m_lastId);
}

void PurchaseView::setItem()
{
  if (!m_lastId.isValid() || m_viewer->getFirstSelectedId().isValid())
    m_lastId = m_viewer->getFirstSelectedId();
  JItemView::setItem();
}

void PurchaseView::itemsRemoved(const Ids& ids)
{
  JItemView::itemsRemoved(ids);
  if (ids.contains(m_lastId))
    m_lastId.clear();
}

void PurchaseView::showHistory()
{
  DatabaseSelector dlg(PRODUCT_SQL_TABLE_NAME, true, this);
  QString str(PRODUCT_FILTER_BUY
              " AND "
              PRODUCT_SQL_TABLE_NAME "." SQL_COLID
              " IN (SELECT DISTINCT "
              PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PID
              " FROM " PURCHASE_ELEMENTS_SQL_TABLE_NAME
              " WHERE "
              PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_NID
              " IN (SELECT DISTINCT "
              PURCHASE_SQL_TABLE_NAME "." SQL_COLID
              " FROM " PURCHASE_SQL_TABLE_NAME
              " WHERE "
              PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_SID " = " +
              m_supplierPicker->getFirstId().str() + "))");
  dlg.getViewer()->setFixedFilter(str);
  if (dlg.exec())
  {
    Ids ids = dlg.getViewer()->getSelectedIds();
    QVector<PurchaseProduct> v;
    for (int i = 0; i != ids.size(); ++i)
    {
      PurchaseProduct o;
      o.SQL_select_last(m_supplierPicker->getFirstId(), ids.at(i));
      o.m_ammount = 0;
      v.push_back(o);
    }
    m_table->set(v, false);
  }
}

void PurchaseView::print(Purchase& o)
{
  if (!m_btnPrint->isChecked() || !JItemHelper::select(o, this))
    return;
  JItemHelper::print(o, 0, this);
  if (o.m_paymentMethod == Purchase::PaymentMethod::Credit)
    JItemHelper::print(o, 0, this);
}

#include "purchasefilter.h"
#include "controls/databasepicker.h"
#include "controls/databaseviewer.h"
#include "widgets/jdateedit.h"
#include "items/jitemex.h"
#include "items/purchase.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QCheckBox>

PurchaseFilter::PurchaseFilter(QWidget* parent)
  : JFilter(parent)
  , m_cbDate(nullptr)
  , m_dtBegin(nullptr)
  , m_dtEnd(nullptr)
  , m_supplierPicker(nullptr)
  , m_productPicker(nullptr)
  , m_storePicker(nullptr)
  , m_cbPaymentCredit(nullptr)
  , m_cbPaymentCash(nullptr)
  , m_cbPaymentBonus(nullptr)
{
  m_cbDate = new QGroupBox;
  m_cbDate->setFlat(true);
  m_cbDate->setCheckable(true);
  m_cbDate->setTitle(tr("Data"));
  m_dtBegin = new JDateEdit;
  m_dtBegin->setCalendarPopup(true);
  m_dtEnd = new JDateEdit;
  m_dtEnd->setCalendarPopup(true);
  m_supplierPicker = new DatabasePicker(SUPPLIER_SQL_TABLE_NAME, true);
  m_productPicker = new DatabasePicker(PRODUCT_SQL_TABLE_NAME, true);
  m_productPicker->getViewer()->setFixedFilter(PRODUCT_FILTER_BUY);
  m_storePicker = new DatabasePicker(STORE_SQL_TABLE_NAME, true);
  m_cbPaymentCredit = new QCheckBox(Purchase::st_paymentText(Purchase::PaymentMethod::Credit));
  m_cbPaymentCash = new QCheckBox(Purchase::st_paymentText(Purchase::PaymentMethod::Cash));
  m_cbPaymentBonus = new QCheckBox(Purchase::st_paymentText(Purchase::PaymentMethod::Bonus));
  m_cbPaymentCredit->setIcon(Purchase::st_paymentIcon(Purchase::PaymentMethod::Credit));
  m_cbPaymentCash->setIcon(Purchase::st_paymentIcon(Purchase::PaymentMethod::Cash));
  m_cbPaymentBonus->setIcon(Purchase::st_paymentIcon(Purchase::PaymentMethod::Bonus));

  QHBoxLayout* ltPayment = new QHBoxLayout;
  ltPayment->setContentsMargins(0, 0, 0, 0);
  ltPayment->setAlignment(Qt::AlignLeft);
  ltPayment->addWidget(m_cbPaymentCredit);
  ltPayment->addWidget(m_cbPaymentCash);
  ltPayment->addWidget(m_cbPaymentBonus);

  QFormLayout* ltfr = new QFormLayout;
  ltfr->setContentsMargins(0, 0, 0, 0);
  ltfr->addRow(tr("Data inicial:"), m_dtBegin);
  ltfr->addRow(tr("Data final:"), m_dtEnd);
  ltfr->addRow(JItemEx::text(SUPPLIER_SQL_TABLE_NAME) + ":", m_supplierPicker);
  ltfr->addRow(JItemEx::text(PRODUCT_SQL_TABLE_NAME) + ":", m_productPicker);
  ltfr->addRow(JItemEx::text(STORE_SQL_TABLE_NAME) + ":", m_storePicker);
  ltfr->addRow(tr("Pagamento:"), ltPayment);

  QVBoxLayout* ltv = new QVBoxLayout;
  ltv->setAlignment(Qt::AlignTop);
  ltv->addWidget(m_cbDate);
  ltv->addLayout(ltfr);
  m_fr->setLayout(ltv);

  connect(m_cbDate, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_cbDate, SIGNAL(toggled(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_dtBegin, SIGNAL(dateChanged(const QDate&)), this, SLOT(emitFilterChangedSignal()));
  connect(m_dtEnd, SIGNAL(dateChanged(const QDate&)), this, SLOT(emitFilterChangedSignal()));
  connect(m_supplierPicker, SIGNAL(changedSignal()), this, SLOT(emitFilterChangedSignal()));
  connect(m_productPicker, SIGNAL(changedSignal()), this, SLOT(emitFilterChangedSignal()));
  connect(m_storePicker, SIGNAL(changedSignal()), this, SLOT(emitFilterChangedSignal()));
  connect(m_cbPaymentCredit, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_cbPaymentCash, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_cbPaymentBonus, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  clear();
}

QString PurchaseFilter::getDateFilter() const
{
  QString str;
  if (m_cbDate->isChecked())
    str += " " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DTE " BETWEEN '" +
           m_dtBegin->date().toString(Qt::ISODate) + "' AND '" +
           m_dtEnd->date().toString(Qt::ISODate) + "' ";
  return str;
}

QString PurchaseFilter::getSupplierFilter() const
{
  QString str;
  QVector<Id> vSupplier = m_supplierPicker->getIds();
  if (!vSupplier.isEmpty())
  {
    str += " " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_SPL " IN (";
    for (int i = 0; i != vSupplier.size(); ++i)
      str += vSupplier.at(i).str() + ",";
    str.chop(1);
    str += ") ";
  }
  return str;
}

QString PurchaseFilter::getProductFilter() const
{
  QString str;
  QVector<Id> vProduct = m_productPicker->getIds();
  if (!vProduct.isEmpty())
  {
    str += " " PURCHASE_SQL_TABLE_NAME "." SQL_COLID
           " = ANY (SELECT " PURCHASE_ELEMENTS_SQL_COL_NID " FROM "
           PURCHASE_ELEMENTS_SQL_TABLE_NAME " WHERE "
           PURCHASE_ELEMENTS_SQL_COL_PID " IN (";
    for (int i = 0; i != vProduct.size(); ++i)
      str += vProduct.at(i).str() + ",";
    str.chop(1);
    str += ")) ";
  }
  return str;
}

QString PurchaseFilter::getStoreFilter() const
{
  QString str;
  QVector<Id> vStore = m_storePicker->getIds();
  if (!vStore.isEmpty())
  {
    str += " " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_STR
                 " IN (";
    for (int i = 0; i != vStore.size(); ++i)
      str += vStore.at(i).str() + ",";
    str.chop(1);
    str += ") ";
  }
  return str;
}

QString PurchaseFilter::getPaymentFilter() const
{
  QString str;

  if (m_cbPaymentCredit->isChecked() ||
      m_cbPaymentCash->isChecked() ||
      m_cbPaymentBonus->isChecked())
  {
    str += " " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_MTH
           " IN (";
    if (m_cbPaymentCredit->isChecked())
      str += QString::number((int)Purchase::PaymentMethod::Credit) + ",";
    if (m_cbPaymentCash->isChecked())
      str += QString::number((int)Purchase::PaymentMethod::Cash) + ",";
    if (m_cbPaymentBonus->isChecked())
      str += QString::number((int)Purchase::PaymentMethod::Bonus) + ",";
    str.chop(1);
    str += ") ";
  }
  else
  {
    str += " " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_MTH
           " NOT IN (";
    str += QString::number((int)Purchase::PaymentMethod::Credit) + "," +
           QString::number((int)Purchase::PaymentMethod::Cash) + "," +
           QString::number((int)Purchase::PaymentMethod::Bonus) + ") ";
  }
  return str;
}

QString PurchaseFilter::getFilter() const
{
  QString strFilter = getDateFilter();

  QString str = getSupplierFilter();
  if (!str.isEmpty() && !strFilter.isEmpty())
    strFilter += " AND ";
  strFilter += str;

  str = getProductFilter();
  if (!str.isEmpty() && !strFilter.isEmpty())
    strFilter += " AND ";
  strFilter += str;

  str = getStoreFilter();
  if (!str.isEmpty() && !strFilter.isEmpty())
    strFilter += " AND ";
  strFilter += str;

  str = getPaymentFilter();
  if (!str.isEmpty() && !strFilter.isEmpty())
    strFilter += " AND ";
  strFilter += str;

  return strFilter;
}

void PurchaseFilter::clear()
{
  m_cbDate->setChecked(false);
  m_dtBegin->setDate(QDate::currentDate());
  m_dtEnd->setDate(QDate::currentDate());
  m_supplierPicker->clear();
  m_productPicker->clear();
  m_storePicker->clear();
  m_cbPaymentCredit->setChecked(true);
  m_cbPaymentCash->setChecked(true);
  m_cbPaymentBonus->setChecked(true);
  updateControls();
}

void PurchaseFilter::updateControls()
{
  m_dtBegin->setEnabled(m_cbDate->isChecked());
  m_dtEnd->setEnabled(m_cbDate->isChecked());
}

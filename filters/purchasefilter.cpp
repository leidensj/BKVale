#include "purchasefilter.h"
#include "controls/databasepicker.h"
#include "controls/databaseviewer.h"
#include "items/jitemex.h"
#include "items/purchase.h"
#include "widgets/jdateinterval.h"
#include <QFormLayout>
#include <QCheckBox>

PurchaseFilter::PurchaseFilter(QWidget* parent)
  : JFilter(parent)
  , m_dtInt(nullptr)
  , m_supplierPicker(nullptr)
  , m_productPicker(nullptr)
  , m_storePicker(nullptr)
  , m_cbPaymentCredit(nullptr)
  , m_cbPaymentCash(nullptr)
  , m_cbPaymentBonus(nullptr)
{
  m_dtInt = new JDateInterval;
  m_dtInt->layout()->setContentsMargins(0, 9, 0, 0);
  m_dtInt->setFlat(true);
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
  ltfr->addRow(JItemEx::text(SUPPLIER_SQL_TABLE_NAME) + ":", m_supplierPicker);
  ltfr->addRow(JItemEx::text(PRODUCT_SQL_TABLE_NAME) + ":", m_productPicker);
  ltfr->addRow(JItemEx::text(STORE_SQL_TABLE_NAME) + ":", m_storePicker);
  ltfr->addRow(tr("Pagamento:"), ltPayment);

  QVBoxLayout* ltv = new QVBoxLayout;
  ltv->setAlignment(Qt::AlignTop);
  ltv->addWidget(m_dtInt);
  ltv->addLayout(ltfr);
  m_fr->setLayout(ltv);

  connect(m_dtInt, SIGNAL(toggled(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_dtInt, SIGNAL(initialDateChangedSignal(const QDate&)), this, SLOT(emitFilterChangedSignal()));
  connect(m_dtInt, SIGNAL(finalDateChangedSignal(const QDate&)), this, SLOT(emitFilterChangedSignal()));
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
  if (m_dtInt->isChecked())
    str += " " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DAT " BETWEEN '" +
           m_dtInt->getInitialDate().toString(Qt::ISODate) + "' AND '" +
           m_dtInt->getFinalDate().toString(Qt::ISODate) + "' ";
  return str;
}

QString PurchaseFilter::getSupplierFilter() const
{
  QString str;
  Ids ids = m_supplierPicker->getIds();
  if (!ids.isEmpty())
  {
    str += " " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_SID " IN (";
    for (auto id : ids)
      str += id.str() + ",";
    str.chop(1);
    str += ") ";
  }
  return str;
}

QString PurchaseFilter::getProductFilter() const
{
  QString str;
  Ids ids = m_productPicker->getIds();
  if (!ids.isEmpty())
  {
    str += " " PURCHASE_SQL_TABLE_NAME "." SQL_COLID
           " = ANY (SELECT " PURCHASE_ELEMENTS_SQL_COL_NID " FROM "
           PURCHASE_ELEMENTS_SQL_TABLE_NAME " WHERE "
           PURCHASE_ELEMENTS_SQL_COL_PID " IN (";
    for (auto id : ids)
      str += id.str() + ",";
    str.chop(1);
    str += ")) ";
  }
  return str;
}

QString PurchaseFilter::getStoreFilter() const
{
  QString str;
  Ids ids = m_storePicker->getIds();
  if (!ids.isEmpty())
  {
    str += " " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_TID
                 " IN (";
    for (auto id : ids)
      str += id.str() + ",";
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
    str += " " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_PAY
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
    str += " " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_PAY
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
  m_dtInt->setChecked(false);
  m_dtInt->day();
  m_supplierPicker->clear();
  m_productPicker->clear();
  m_storePicker->clear();
  m_cbPaymentCredit->setChecked(true);
  m_cbPaymentCash->setChecked(true);
  m_cbPaymentBonus->setChecked(true);
}

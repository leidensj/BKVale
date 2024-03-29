#include "couponfilter.h"
#include "defines.h"
#include "items/coupon.h"
#include "widgets/jdateinterval.h"
#include <QFormLayout>
#include <QCheckBox>
#include <QFrame>

CouponFilter::CouponFilter(QWidget* parent)
  : JFilter(parent)
  , m_cbExpired(nullptr)
  , m_cbRedeemed(nullptr)
  , m_cbPercentage(nullptr)
  , m_cbValue(nullptr)
  , m_cbProduct(nullptr)
  , m_dtCreation(nullptr)
  , m_dtRedemption(nullptr)
{
  m_cbExpired = new QCheckBox(tr("Expirados"));
  m_cbExpired->setTristate();
  m_cbRedeemed = new QCheckBox(tr("Resgatados"));
  m_cbRedeemed->setTristate();
  m_cbPercentage = new QCheckBox(tr("Porcentagem"));
  m_cbValue = new QCheckBox(tr("Valor"));
  m_cbProduct = new QCheckBox(tr("Produto"));
  m_dtCreation = new JDateInterval;
  m_dtCreation->setTitle(tr("Data de criação"));
  m_dtCreation->layout()->setContentsMargins(0, 9, 0, 0);
  m_dtCreation->setFlat(true);
  m_dtCreation->setChecked(false);
  m_dtRedemption = new JDateInterval;
  m_dtRedemption->setTitle(tr("Data de resgate"));
  m_dtRedemption->layout()->setContentsMargins(0, 9, 0, 0);
  m_dtRedemption->setFlat(true);
  m_dtRedemption->setChecked(false);

  QHBoxLayout* ltType = new QHBoxLayout;
  ltType->setContentsMargins(0, 0, 0, 0);
  ltType->addWidget(m_cbPercentage);
  ltType->addWidget(m_cbValue);
  ltType->addWidget(m_cbProduct);
  ltType->setAlignment(Qt::AlignLeft);

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->setContentsMargins(0, 0, 0, 0);
  ltMain->addWidget(m_cbExpired);
  ltMain->addWidget(m_cbRedeemed);
  ltMain->addLayout(ltType);
  ltMain->addWidget(m_dtCreation);
  ltMain->addWidget(m_dtRedemption);
  ltMain->setAlignment(Qt::AlignTop);

  m_fr->setLayout(ltMain);

  connect(m_cbExpired, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_cbRedeemed, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_cbPercentage, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_cbValue, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_cbProduct, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_dtCreation, SIGNAL(changedSignal(bool, const QDate&, const QDate&)), this, SLOT(emitFilterChangedSignal()));
  connect(m_dtRedemption, SIGNAL(changedSignal(bool, const QDate&, const QDate&)), this, SLOT(emitFilterChangedSignal()));
  CouponFilter::clear();
}

QString CouponFilter::getFilter() const
{
  QString str;
  Qt::CheckState cs = m_cbExpired->checkState();
  if (cs == Qt::Checked)
    str += COUPON_SQL_COL_TEMP_EXP " = TRUE";
  else if (cs == Qt::PartiallyChecked)
    str += COUPON_SQL_COL_TEMP_EXP " = FALSE";

  cs = m_cbRedeemed->checkState();
  if (cs != Qt::Unchecked && !str.isEmpty())
    str += " AND ";
  if (cs == Qt::Checked)
    str += COUPON_SQL_COL_RED " = TRUE";
  else if (cs == Qt::PartiallyChecked)
    str += COUPON_SQL_COL_RED " = FALSE";

  if (!str.isEmpty())
    str += " AND ";

  if (m_cbPercentage->isChecked() ||
      m_cbValue->isChecked() ||
      m_cbProduct->isChecked())
  {
    str += COUPON_SQL_COL_TYP
           " IN (";
    if (m_cbPercentage->isChecked())
      str += QString::number((int)Coupon::Type::Percentage) + ",";
    if (m_cbValue->isChecked())
      str += QString::number((int)Coupon::Type::Value) + ",";
    if (m_cbProduct->isChecked())
      str += QString::number((int)Coupon::Type::Product) + ",";
    str.chop(1);
    str += ") ";
  }
  else
  {
    str += COUPON_SQL_COL_TYP
           " NOT IN (";
    str += QString::number((int)Coupon::Type::Percentage) + "," +
           QString::number((int)Coupon::Type::Value) + "," +
           QString::number((int)Coupon::Type::Product) + ") ";
  }

  if (m_dtCreation->isChecked())
  {
    if (!str.isEmpty())
      str += " AND ";
    str += " DATE(" COUPON_SQL_COL_CDT ") BETWEEN '" +
           m_dtCreation->getInitialDate().toString(Qt::ISODate) + "' AND '" +
           m_dtCreation->getFinalDate().toString(Qt::ISODate) + "' ";
  }

  if (m_dtRedemption->isChecked())
  {
    if (!str.isEmpty())
      str += " AND ";
    str += " " COUPON_SQL_COL_RED " = TRUE AND DATE(" COUPON_SQL_COL_RDT ") BETWEEN '" +
           m_dtRedemption->getInitialDate().toString(Qt::ISODate) + "' AND '" +
           m_dtRedemption->getFinalDate().toString(Qt::ISODate) + "' ";
  }

  return str;
}

void CouponFilter::clear()
{
  m_cbExpired->setChecked(false);
  m_cbRedeemed->setChecked(false);
  m_cbPercentage->setChecked(true);
  m_cbValue->setChecked(true);
  m_cbProduct->setChecked(true);
  m_dtCreation->day();
  m_dtCreation->setChecked(false);
  m_dtRedemption->day();
  m_dtRedemption->setChecked(false);
}

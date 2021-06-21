#include "couponfilter.h"
#include "defines.h"
#include <QFormLayout>
#include <QCheckBox>
#include <QFrame>

CouponFilter::CouponFilter(QWidget* parent)
  : JFilter(parent)
  , m_cbExpired(nullptr)
  , m_cbRedeemed(nullptr)
{
  m_cbExpired = new QCheckBox(tr("Expirados"));
  m_cbExpired->setTristate();
  m_cbRedeemed = new QCheckBox(tr("Resgatados"));
  m_cbRedeemed->setTristate();

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->setContentsMargins(0, 0, 0, 0);
  ltMain->addWidget(m_cbExpired);
  ltMain->addWidget(m_cbRedeemed);
  ltMain->setAlignment(Qt::AlignTop);

  m_fr->setLayout(ltMain);

  connect(m_cbExpired, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_cbRedeemed, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  CouponFilter::clear();
}

QString CouponFilter::getFilter() const
{
  QString strFilter;
  Qt::CheckState cs = m_cbExpired->checkState();
  if (cs == Qt::Checked)
    strFilter += COUPON_SQL_COL_RED " = FALSE AND "
                 COUPON_SQL_COL_EXP " = TRUE AND "
                 COUPON_SQL_COL_EDT " < CURRENT_DATE";
  else if (cs == Qt::PartiallyChecked)
    strFilter += COUPON_SQL_COL_RED " = TRUE OR "
                 COUPON_SQL_COL_EXP " = FALSE OR "
                 COUPON_SQL_COL_EDT " > CURRENT_DATE";

  cs = m_cbRedeemed->checkState();
  if (cs != Qt::Unchecked && !strFilter.isEmpty())
    strFilter += " AND ";
  if (cs == Qt::Checked)
    strFilter += COUPON_SQL_COL_RED " = TRUE";
  else if (cs == Qt::PartiallyChecked)
    strFilter += COUPON_SQL_COL_RED " = FALSE";

  return strFilter;
}

void CouponFilter::clear()
{
  m_cbExpired->setChecked(false);
  m_cbRedeemed->setChecked(false);
}

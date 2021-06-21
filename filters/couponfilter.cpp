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
  m_cbRedeemed = new QCheckBox(tr("Resgatados"));

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->setContentsMargins(0, 0, 0, 0);
  ltMain->addWidget(m_cbExpired);
  ltMain->addWidget(m_cbRedeemed);
  ltMain->setAlignment(Qt::AlignTop);

  m_fr->setLayout(ltMain);

  connect(m_cbExpired, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  connect(m_cbRedeemed, SIGNAL(clicked(bool)), this, SLOT(emitFilterChangedSignal()));
  clear();
}

QString CouponFilter::getFilter() const
{
  QString strFilter;
  if (m_cbExpired->isChecked())
    strFilter += COUPON_SQL_COL_RED " = FALSE AND "
                 COUPON_SQL_COL_EXP " = TRUE AND "
                 COUPON_SQL_COL_EDT " < CURRENT_DATE";

  if (m_cbRedeemed->isChecked())
  {
    if (!strFilter.isEmpty())
      strFilter += " AND ";
    strFilter += COUPON_SQL_COL_RED " = TRUE";
  }

  return strFilter;
}

void CouponFilter::clear()
{
  m_cbExpired->setChecked(false);
  m_cbRedeemed->setChecked(false);
}

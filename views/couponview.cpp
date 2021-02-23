#include "couponview.h"
#include "widgets/jlineedit.h"
#include "widgets/jdatepicker.h"
#include "widgets/jspinbox.h"
#include "widgets/jdoublespinbox.h"
#include "controls/databasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>
#include <QRadioButton>

CouponView::CouponView(QWidget* parent)
  : JItemView(COUPON_SQL_TABLE_NAME, parent)
  , m_imagePicker(nullptr)
  , m_cbExpiration(nullptr)
  , m_dtExpiration(nullptr)
  , m_rdoPercentage(nullptr)
  , m_rdoValue(nullptr)
  , m_spnPercentage(nullptr)
  , m_spnValue(nullptr)
{
  m_cbExpiration = new QCheckBox("Expira em:");
  m_dtExpiration = new JDatePicker;

  m_rdoPercentage = new QRadioButton(tr("Porcentagem:"));
  m_spnPercentage = new JSpinBox;
  m_spnPercentage->setMinimum(0);
  m_spnPercentage->setMaximum(100);
  m_spnPercentage->setSuffix("%");

  m_rdoValue = new QRadioButton(tr("Valor:"));
  m_spnValue = new JDoubleSpinBox;
  m_spnValue->setMinimum(0.0);
  m_spnValue->setPrefix("R$");

  QFormLayout* ltMain = new QFormLayout;
  ltMain->addRow(m_rdoPercentage, m_spnPercentage);
  ltMain->addRow(m_rdoValue, m_spnValue);
  ltMain->addRow(m_cbExpiration, m_dtExpiration);
  ltMain->setAlignment(Qt::AlignTop);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltMain);

  m_tab->addTab(tabframe, QIcon(":/icons/res/coupon.png"), tr("Cupom"));
  clear();
}

void CouponView::getItem(JItemSQL& o) const
{
  Coupon& _o = dynamic_cast<Coupon&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_type = m_rdoPercentage->isChecked() ? Coupon::Type::Percentage : Coupon::Type::Value;
  _o.m_dtCreation = DateTime::server().date();
  _o.m_bExpires = m_cbExpiration->isChecked();
  _o.m_dtExpiration = m_dtExpiration->getDate();
  _o.m_percentage = m_spnPercentage->value();
  _o.m_value = m_spnValue->value();
}

void CouponView::setItem(const JItemSQL& o)
{
  const Coupon& _o = static_cast<const Coupon&>(o);
  m_rdoPercentage->setChecked(_o.m_type == Coupon::Type::Percentage);
  m_rdoValue->setChecked(_o.m_type == Coupon::Type::Value);
  m_cbExpiration->setChecked(_o.m_bExpires);
  m_dtExpiration->setDate(_o.m_dtExpiration);
  m_spnPercentage->setValue(_o.m_percentage);
  m_spnValue->setValue(_o.m_value);
}

void CouponView::updateControls()
{
  m_spnPercentage->setEnabled(m_rdoValue->isChecked());
  m_spnValue->setEnabled(m_rdoValue->isChecked());
}

#include "couponview.h"
#include "widgets/jlineedit.h"
#include "widgets/jdatepicker.h"
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
  , m_edPercentage(nullptr)
  , m_edValue(nullptr)
{
  m_cbExpiration = new QCheckBox("Expira em:");
  m_dtExpiration = new JDatePicker;

  m_rdoPercentage = new QRadioButton(tr("Porcentagem:"));
  m_edPercentage = new JExpLineEdit(Data::Type::Percentage);
  m_edPercentage->setMaximum(100.0);
  m_edPercentage->setMinimum(0.0);

  m_rdoValue = new QRadioButton(tr("Valor:"));
  m_edValue = new JExpLineEdit(Data::Type::Money);
  m_edValue->setMinimum(0.0);

  QFormLayout* ltMain = new QFormLayout;
  ltMain->addRow(m_rdoPercentage, m_edPercentage);
  ltMain->addRow(m_rdoValue, m_edValue);
  ltMain->addRow(m_cbExpiration, m_dtExpiration);
  ltMain->setAlignment(Qt::AlignTop);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltMain);
  m_tab->addTab(tabframe, QIcon(":/icons/res/coupon.png"), tr("Cupom"));

  connect(m_cbExpiration, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoPercentage, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoValue, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoPercentage, &QRadioButton::clicked, [this]() { m_edPercentage->setFocus(); });
  connect(m_rdoValue, &QRadioButton::clicked, [this]() { m_edValue->setFocus(); });

  setFocusWidgetOnClear(m_edPercentage);
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
  _o.m_percentage = m_edPercentage->value();
  _o.m_value = m_edValue->value();
}

void CouponView::setItem(const JItemSQL& o)
{
  const Coupon& _o = static_cast<const Coupon&>(o);
  m_rdoPercentage->setChecked(_o.m_type == Coupon::Type::Percentage);
  m_rdoValue->setChecked(_o.m_type == Coupon::Type::Value);
  m_cbExpiration->setChecked(_o.m_bExpires);
  m_dtExpiration->setDate(_o.m_dtExpiration);
  m_edPercentage->setValue(_o.m_percentage);
  m_edValue->setValue(_o.m_value);
  updateControls();
}

void CouponView::updateControls()
{
  m_dtExpiration->setEnabled(m_cbExpiration->isChecked());
  m_edPercentage->setEnabled(m_rdoPercentage->isChecked());
  m_edValue->setEnabled(m_rdoValue->isChecked());
}

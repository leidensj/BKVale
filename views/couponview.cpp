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
  : JItemView("", parent)
  , m_imagePicker(nullptr)
  , m_cbExpiration(nullptr)
  , m_dtExpiration(nullptr)
  , m_rdoPercent(nullptr)
  , m_rdoValue(nullptr)
  , m_spnPercent(nullptr)
  , m_spnValue(nullptr)
{
  m_cbExpiration = new QCheckBox;
  QLabel* lblExpiration = new QLabel(tr("Expira em: "));
  m_dtExpiration = new JDatePicker;
  QHBoxLayout* ltExpiration = new QHBoxLayout;
  ltExpiration->setContentsMargins(0, 0, 0, 0);
  ltExpiration->setAlignment(Qt::AlignLeft);
  ltExpiration->addWidget(m_cbExpiration);
  ltExpiration->addWidget(lblExpiration);
  ltExpiration->addWidget(m_dtExpiration);

  m_rdoPercent = new QRadioButton(tr("Porcentagem"));
  m_spnPercent = new JSpinBox;
  m_spnPercent->setMinimum(0);
  m_spnPercent->setMaximum(100);
  m_spnPercent->setSuffix("%");
  QHBoxLayout* ltPercent = new QHBoxLayout;
  ltPercent->setContentsMargins(0, 0, 0, 0);
  ltPercent->setAlignment(Qt::AlignLeft);
  ltPercent->addWidget(m_rdoPercent);
  ltPercent->addWidget(m_spnPercent);

  m_rdoValue = new QRadioButton(tr("Valor"));
  m_spnValue = new JDoubleSpinBox;
  m_spnValue->setMinimum(0.0);
  m_spnValue->setPrefix("R$");
  QHBoxLayout* ltValue = new QHBoxLayout;
  ltValue->setContentsMargins(0, 0, 0, 0);
  ltValue->setAlignment(Qt::AlignLeft);
  ltValue->addWidget(m_rdoValue);
  ltValue->addWidget(m_spnValue);

  m_rdoPercent = new QRadioButton
  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(ltExpiration);
  ltMain->addLayout(ltPercent);
  ltMain->addLayout(ltValue);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltMain);

  m_tab->addTab(tabframe, QIcon(":/icons/res/category.png"), tr("Cupom"));
  setFocusWidgetOnClear(m_edName);
  clear();
}

void CategoryView::getItem(JItemSQL& o) const
{
  Category& _o = dynamic_cast<Category&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_image.m_id = m_imagePicker->getFirstId();
  _o.m_name = m_edName->text();
}

void CategoryView::setItem(const JItemSQL& o)
{
  const Category& _o = static_cast<const Category&>(o);
  m_edName->setText(_o.m_name);
  m_imagePicker->addItem(_o.m_image);
}

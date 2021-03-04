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
#include <QInputDialog>

CouponView::CouponView(QWidget* parent)
  : JItemView(COUPON_SQL_TABLE_NAME, parent)
  , m_lblRedeemed(nullptr)
  , m_edCode(nullptr)
  , m_cbExpiration(nullptr)
  , m_dtExpiration(nullptr)
  , m_rdoPercentage(nullptr)
  , m_rdoValue(nullptr)
  , m_edPercentage(nullptr)
  , m_edValue(nullptr)
{
  m_edCode = new JLineEdit(Text::Input::Alpha, true);
  m_edCode->setPlaceholderText(tr("Gerar código automaticamente"));

  m_lblRedeemed = new QLabel;

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
  ltMain->addRow(tr("Código:"), m_edCode);
  ltMain->addRow(m_rdoPercentage, m_edPercentage);
  ltMain->addRow(m_rdoValue, m_edValue);
  ltMain->addRow(m_cbExpiration, m_dtExpiration);
  ltMain->setAlignment(Qt::AlignTop);
  ltMain->addRow(tr("Resgate:"), m_lblRedeemed);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltMain);
  m_tab->addTab(tabframe, QIcon(":/icons/res/coupon.png"), tr("Cupom"));

  connect(m_cbExpiration, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoPercentage, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoValue, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoPercentage, SIGNAL(clicked(bool)), m_edPercentage, SLOT(setFocus()));
  connect(m_rdoValue, SIGNAL(clicked(bool)), m_edValue, SLOT(setFocus()));

  setFocusWidgetOnClear(m_edPercentage);
  clear();
}

void CouponView::getItem(JItemSQL& o) const
{
  Coupon& _o = dynamic_cast<Coupon&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_code = m_edCode->text();
  _o.m_type = m_rdoPercentage->isChecked() ? Coupon::Type::Percentage : Coupon::Type::Value;
  _o.m_dtCreation = DateTime::server();
  _o.m_bExpires = m_cbExpiration->isChecked();
  _o.m_dtExpiration = m_dtExpiration->getDate();
  _o.m_percentage = m_edPercentage->value();
  _o.m_value = m_edValue->value();
}

void CouponView::setItem(const JItemSQL& o)
{
  const Coupon& _o = static_cast<const Coupon&>(o);
  m_lblRedeemed->setText(tr("Código resgatado no dia %1").arg(_o.m_dtRedeemed.toString("dd/MM/yyyy hh:mm:ss")));
  m_lblRedeemed->setVisible(_o.m_bRedeemed);
  m_edCode->setReadOnly(_o.m_id.isValid());
  m_edCode->setText(_o.m_code);
  m_rdoPercentage->setChecked(_o.m_type == Coupon::Type::Percentage);
  m_rdoValue->setChecked(_o.m_type == Coupon::Type::Value);
  m_cbExpiration->setChecked(_o.m_bExpires);
  m_dtExpiration->setDate(_o.m_dtExpiration);
  m_edPercentage->setValue(_o.m_percentage);
  m_edValue->setValue(_o.m_value);
  m_tab->setTabEnabled(0, !_o.m_bRedeemed);
  m_btnSave->setEnabled(!_o.m_bRedeemed);
  updateControls();
}

void CouponView::updateControls()
{
  m_dtExpiration->setEnabled(m_cbExpiration->isChecked());
  m_edPercentage->setEnabled(m_rdoPercentage->isChecked());
  m_edValue->setEnabled(m_rdoValue->isChecked());
  if (!m_rdoPercentage->isChecked())
    m_edPercentage->setValue(0.0);
  if (!m_rdoValue->isChecked())
    m_edValue->setValue(0.0);
}

bool CouponView::save(Id& id)
{
  bool ok = false;
  int n = QInputDialog::getInt(this, tr("Gerar Cupons"), tr("Informe o número de cupons a serem gerados:"), 0, 1, 9999, 1, &ok);
  if (ok)
  {
    QString lst;
    Coupon o;
    getItem(o);
    for (int i = 0; i != n; ++i)
    {
      setItem(o);
      QString code = m_edCode->text().isEmpty()
        ? Coupon::st_newCode()
        : m_edCode->text() + Data::strInt(i + 1);
      m_edCode->setText(code);
      if (JItemView::save(id))
        lst.append(code + "\n");
    }
    if (!lst.isEmpty())
      QInputDialog::getMultiLineText(this, tr("Códigos Gerados"), tr("Lista dos códigos que foram gerados"), lst, &ok);
  }
  return true;
}

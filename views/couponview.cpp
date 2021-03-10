#include "couponview.h"
#include "printer.h"
#include "widgets/jlineedit.h"
#include "widgets/jdatepicker.h"
#include "controls/databasepicker.h"
#include "items/jitemex.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>
#include <QRadioButton>
#include <QInputDialog>
#include <QSettings>
#include <QPlainTextEdit>
#include <QDialogButtonBox>

#define SETTINGS_COUPON_STORE_ID "coupon/storeid"

CouponConfirmation::CouponConfirmation(const QVector<Coupon>& coupons, QWidget* parent)
  : QDialog(parent)
{
  QPlainTextEdit* teCodes = new QPlainTextEdit;
  QDialogButtonBox* btns = new QDialogButtonBox(QDialogButtonBox::Yes | QDialogButtonBox::No);
  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addWidget(new QLabel(tr("Deseja imprimir os códigos gerados?")));
  ltMain->addWidget(teCodes);
  ltMain->addWidget(btns);
  setLayout(ltMain);
  teCodes->setReadOnly(true);
  for (const Coupon& o : coupons)
    teCodes->appendPlainText(o.m_code);
  connect(btns, SIGNAL(accepted()), this, SLOT(accept()));
  connect(btns, SIGNAL(rejected()), this, SLOT(reject()));
}

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
  , m_storePicker(nullptr)
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

  m_storePicker = new DatabasePicker(STORE_SQL_TABLE_NAME);

  QFormLayout* ltMain = new QFormLayout;
  ltMain->addRow(JItemEx::text(STORE_SQL_TABLE_NAME) + ":", m_storePicker);
  ltMain->addRow(tr("Código:"), m_edCode);
  ltMain->addRow(m_rdoPercentage, m_edPercentage);
  ltMain->addRow(m_rdoValue, m_edValue);
  ltMain->addRow(m_cbExpiration, m_dtExpiration);
  ltMain->setAlignment(Qt::AlignTop);
  ltMain->addRow(tr(""), m_lblRedeemed);

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
  _o.m_store.m_id = m_storePicker->getFirstId();
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
  m_storePicker->addItem(_o.m_store);
  if (!_o.m_id.isValid() && !_o.m_store.m_id.isValid())
  {
    QSettings settings(SETTINGS_COMPANY_NAME, SETTINGS_APP_NAME);
    Store store(Id(settings.value(SETTINGS_COUPON_STORE_ID).toLongLong()));
    QString error;
    if (store.SQL_select(error))
      m_storePicker->addItem(store);
  }
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
  id.clear();
  bool ok = true;
  int n = 1;
  if (!m_id.isValid())
    n = QInputDialog::getInt(this, tr("Gerar Cupons"), tr("Informe o número de cupons a serem gerados:"), 0, 1, 9999, 1, &ok);
  if (ok)
  {
    QVector<Coupon> coupons;
    Coupon o;
    getItem(o);
    for (int i = 0; i != n; ++i)
    {
      o.m_id.clear();
      o.m_code.clear();
      o.m_code = m_edCode->text().isEmpty() ? Coupon::st_newCode() : m_edCode->text() + Data::strInt(i + 1);
      if (JItemEx::save(o, m_viewer->getTableName(), this))
      {
        QString error;
        if (o.m_store.m_id.isValid())
          o.m_store.SQL_select(error);
        coupons.append(o);
      }
    }
    clear();
    CouponConfirmation dlg(coupons, this);
    if (dlg.exec())
      for (const Coupon& o : coupons)
        JItemEx::print(o, nullptr, this);
  }
  return true;
}

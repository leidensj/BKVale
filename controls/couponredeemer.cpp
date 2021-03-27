#include "couponredeemer.h"
#include "items/coupon.h"
#include "items/jitemhelper.h"
#include "widgets/jlineedit.h"
#include <QLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox>

CouponRedeemer::CouponRedeemer(QWidget* parent)
  : QDialog(parent)
  , m_edCode(nullptr)
{
  setWindowTitle(tr("Resgatar Cupom"));
  setWindowIcon(QIcon(":/icons/res/redeem.png"));
  m_edCode = new JLineEdit(Text::Input::Alphanumeric, true);
  QFont f = m_edCode->font();
  f.setPointSize(36);
  f.setBold(true);
  m_edCode->setFont(f);
  m_edCode->setPlaceholderText(tr("Insira o código"));
  QDialogButtonBox* btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);
  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addWidget(m_edCode);
  ltMain->addWidget(btns);
  setLayout(ltMain);
  connect(btns->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)), this, SLOT(accept()));
  connect(btns->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(redeem()));
  connect(m_edCode, SIGNAL(enterSignal()), this, SLOT(redeem()));
  m_edCode->setFocus();
}

void CouponRedeemer::redeem()
{
  QString code = m_edCode->text();
  if (!code.isEmpty())
  {
    Coupon o;
    o.m_code = code;
    QString error;
    if (o.SQL_redeem(error))
    {
      m_edCode->setText(o.strCoupon());
      m_edCode->selectAll();
      JItemHelper::print(o, nullptr, this);
    }
    else
    {
      QMessageBox::information(this, tr("Ops..."), tr("Não foi possível resgatar o cupom:\n%1").arg(error), QMessageBox::Ok);
    }
  }
  m_edCode->setFocus();
}

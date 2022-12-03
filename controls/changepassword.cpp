#include "changepassword.h"
#include "widgets/jlineedit.h"
#include <QLayout>
#include <QPushButton>

ChangePassword::ChangePassword(QWidget* parent)
  : QDialog(parent)
  , m_currentPassword(nullptr)
  , m_newPassword1(nullptr)
  , m_newPassword2(nullptr)
  , m_btnChange(nullptr)
{
  m_currentPassword = new JLineEdit(Text::Input::All, false);
  m_currentPassword->setPlaceholderText(tr("Senha atual"));
  m_currentPassword->setEchoMode(QLineEdit::EchoMode::Password);
  {
    QFont f = m_currentPassword->font();
    f.setPointSize(12);
    m_currentPassword->setFont(f);
  }

  m_newPassword1 = new JLineEdit(Text::Input::All, false);
  m_newPassword1->setPlaceholderText(tr("Nova senha"));
  m_newPassword1->setEchoMode(QLineEdit::EchoMode::Password);
  {
    QFont f = m_newPassword1->font();
    f.setPointSize(12);
    m_newPassword1->setFont(f);
  }

  m_newPassword2 = new JLineEdit(Text::Input::All, false);
  m_newPassword2->setPlaceholderText(tr("Senha atual"));
  m_newPassword2->setEchoMode(QLineEdit::EchoMode::Password);
  {
    QFont f = m_newPassword2->font();
    f.setPointSize(12);
    m_newPassword2->setFont(f);
  }

  m_btnChange = new QPushButton;
  m_btnChange->setText(tr("Alterar senha"));
  m_btnChange->setDefault(true);

  QVBoxLayout* v = new QVBoxLayout;
  v->setContentsMargins(0, 0, 0, 0);
  v->addWidget(m_currentPassword);
  v->addWidget(m_newPassword1);
  v->addWidget(m_newPassword2);

  setLayout(v);
  setWindowFlags(Qt::WindowCloseButtonHint);
  layout()->setSizeConstraint(QLayout::SetFixedSize);

  connect(m_btnChange, SIGNAL(clicked(bool)), this, SLOT(changePassword()));

  setWindowTitle(tr("Alterar Senha"));
  setWindowIcon(QIcon(":/icons/res/password.png"));
  setModal(true);
}

void ChangePassword::changePassword()
{

}

#include "logindialog.h"
#include "jlineedit.h"
#include "user.h"
#include <QLayout>
#include <QIcon>
#include <QLabel>
#include <QDialogButtonBox>
#include <QRegExpValidator>
#include <QPushButton>

#ifdef Q_OS_WIN32
#include <windows.h>
#else
#include <X11/XKBlib.h>
#endif

namespace
{
  bool checkCapsLock()
  {
  #ifdef Q_OS_WIN32
  return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
  #else
    return false;
  #endif
  }
}

LoginDialog::LoginDialog(UserLoginSQL& userLogin,
                         QWidget* parent)
  : QDialog(parent)
  , m_userLogin(userLogin)
  , m_user(nullptr)
  , m_password(nullptr)
  , m_capsLock(nullptr)
{
  QLabel* lblBaita = new QLabel();
  lblBaita->setPixmap(QIcon(":/icons/res/baita.png").pixmap(QSize(64, 64)));
  lblBaita->setMinimumSize(64, 64);
  lblBaita->setMaximumSize(64, 64);
  lblBaita->setScaledContents(true);
  QLabel* lblUser = new QLabel();
  lblUser->setPixmap(QIcon(":/icons/res/user.png").pixmap(QSize(24, 24)));
  lblUser->setMinimumSize(24, 24);
  lblUser->setMaximumSize(24, 24);
  lblUser->setScaledContents(true);
  QLabel* lblPassword = new QLabel();
  lblPassword->setPixmap(QIcon(":/icons/res/password.png").pixmap(QSize(24, 24)));
  lblPassword->setMinimumSize(24, 24);
  lblPassword->setMaximumSize(24, 24);
  lblPassword->setScaledContents(true);
  m_user = new JLineEdit(JLineEdit::Input::Alphanumeric, JLineEdit::st_defaultFlags1);
  m_user->setPlaceholderText(tr("Usuário"));
  m_user->setMinimumHeight(24);
  m_user->setMaximumHeight(24);
  {
    QFont f = m_user->font();
    f.setPointSize(12);
    m_user->setFont(f);
  }

  m_password = new JLineEdit(JLineEdit::Input::All, JLineEdit::st_defaultFlags2);
  m_password->setPlaceholderText(tr("Senha"));
  m_password->setEchoMode(QLineEdit::EchoMode::Password);
  {
    QFont f = m_password->font();
    f.setPointSize(12);
    m_password->setFont(f);
  }
  m_capsLock = new QLabel();
  m_capsLock->setPixmap(QIcon(":/icons/res/capslockon.png").pixmap(QSize(24, 24)));
  m_capsLock->setMinimumSize(24, 24);
  m_capsLock->setMaximumSize(24, 24);
  m_capsLock->setScaledContents(true);
  m_capsLock->setToolTip("Caps Lock");

  QHBoxLayout *h0 = new QHBoxLayout();
  h0->setContentsMargins(0, 0, 0, 0);
  h0->setAlignment(Qt::AlignCenter);
  h0->addWidget(lblBaita);
  QHBoxLayout *h1 = new QHBoxLayout();
  h1->setContentsMargins(0, 0, 0, 0);
  h1->addWidget(lblUser);
  h1->addWidget(m_user);
  QHBoxLayout *h2 = new QHBoxLayout();
  h2->setContentsMargins(0, 0, 0, 0);
  h2->addWidget(lblPassword);
  h2->addWidget(m_password);
  h2->addWidget(m_capsLock);

  m_status = new QLabel();

  QPushButton* btnLogin = new QPushButton(tr("Login"));
  btnLogin->setDefault(true);

  QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal);
  buttonBox->addButton(btnLogin, QDialogButtonBox::ActionRole);

  QVBoxLayout *v1 = new QVBoxLayout();
  v1->addLayout(h0);
  v1->addLayout(h1);
  v1->addLayout(h2);
  v1->addWidget(m_status);
  v1->addWidget(buttonBox);

  setLayout(v1);
  setWindowFlags(Qt::WindowCloseButtonHint);
  layout()->setSizeConstraint(QLayout::SetFixedSize);

  QObject::connect(btnLogin,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(login()));

  setWindowTitle(tr("Baita Assistente Login"));
  setWindowIcon(QIcon(":/icons/res/login.png"));
  updateCapsLock();
  setModal(true);
}

void LoginDialog::updateCapsLock()
{
  QString icon = checkCapsLock()
                 ? ":/icons/res/capslockon.png"
                 : ":/icons/res/capslockoff.png";
  m_capsLock->setPixmap(QIcon(icon).pixmap(QSize(24, 24)));
}

void LoginDialog::keyPressEvent(QKeyEvent* event)
{
  QDialog::keyPressEvent(event);
  updateCapsLock();
}

void LoginDialog::login()
{
  QString error;
  if (m_userLogin.login(m_user->text(),
                        m_password->text(),
                        error))
  {
    accept();
  }
  else
  {
    m_status->setText(error);
    m_password->selectAll();
    m_password->setFocus();
  }
}

#include "logindialog.h"
#include "jlineedit.h"
#include "user.h"
#include "databaseutils.h"
#include <QLayout>
#include <QIcon>
#include <QLabel>
#include <QDialogButtonBox>
#include <QRegExpValidator>
#include <QPushButton>
#include <QPropertyAnimation>
#include "settings.h"
#include "jspinbox.h"
#include <QApplication>

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
  , m_hostName(nullptr)
  , m_port(nullptr)
  , m_capsLock(nullptr)
  , m_status(nullptr)
  , m_btnDatabaseOpt(nullptr)
  , m_frDatabase(nullptr)
  , m_btnLogin(nullptr)
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
  QLabel* lblDatabase = new QLabel();
  lblDatabase->setPixmap(QIcon(":/icons/res/databasebackup.png").pixmap(QSize(24, 24)));
  lblDatabase->setMinimumSize(24, 24);
  lblDatabase->setMaximumSize(24, 24);
  lblDatabase->setScaledContents(true);
  QLabel* lblPort = new QLabel();
  lblPort->setPixmap(QIcon(":/icons/res/networkcable.png").pixmap(QSize(24, 24)));
  lblPort->setMinimumSize(24, 24);
  lblPort->setMaximumSize(24, 24);
  lblPort->setScaledContents(true);
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

  m_hostName = new JLineEdit(JLineEdit::Input::All, JLineEdit::st_defaultFlags2);
  m_hostName->setPlaceholderText(tr("Host"));
  {
    QFont f = m_hostName->font();
    f.setPointSize(12);
    m_hostName->setFont(f);
  }

  m_port = new JSpinBox(true);
  m_port->setRange(0, 65535);
  m_port->setSpecialValueText(tr("Porta"));
  {
    QFont f = m_port->font();
    f.setPointSize(12);
    m_port->setFont(f);
  }

  Settings settings;
  settings.load();
  m_hostName->setText(settings.m_databaseHostName);
  m_port->setValue(settings.m_databasePort);

  m_btnDatabaseOpt = new QPushButton;
  m_btnDatabaseOpt->setFlat(true);
  m_btnDatabaseOpt->setIconSize(QSize(24, 24));
  m_btnDatabaseOpt->setIcon(QIcon(":/icons/res/arrowdropdown.png"));
  m_btnDatabaseOpt->setMaximumWidth(24);

  QFrame* line1 = new QFrame;
  line1->setFrameShape(QFrame::HLine);

  QFrame* line2 = new QFrame;
  line2->setFrameShape(QFrame::HLine);

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
  QHBoxLayout *h3 = new QHBoxLayout();
  h3->setContentsMargins(0, 0, 0, 0);
  h3->addWidget(line1);
  h3->addWidget(m_btnDatabaseOpt);
  h3->addWidget(line2);

  m_frDatabase = new QFrame;
  QHBoxLayout *h4 = new QHBoxLayout();
  h4->setContentsMargins(0, 0, 0, 0);
  h4->addWidget(lblDatabase);
  h4->addWidget(m_hostName);

  QHBoxLayout *h5 = new QHBoxLayout();
  h5->setContentsMargins(0, 0, 0, 0);
  h5->addWidget(lblPort);
  h5->addWidget(m_port);

  QVBoxLayout* vDatabase = new QVBoxLayout;
  vDatabase->setContentsMargins(0, 0, 0, 0);
  vDatabase->addLayout(h4);
  vDatabase->addLayout(h5);
  m_frDatabase->setLayout(vDatabase);

  m_status = new QLabel;

  m_btnLogin = new QPushButton(tr("Login"));
  m_btnLogin->setDefault(true);

  QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal);
  buttonBox->addButton(m_btnLogin, QDialogButtonBox::ActionRole);

  QVBoxLayout *v1 = new QVBoxLayout();
  v1->addLayout(h0);
  v1->addLayout(h1);
  v1->addLayout(h2);
  v1->addLayout(h3);
  v1->addWidget(m_frDatabase);
  v1->addWidget(m_status);
  v1->addWidget(buttonBox);

  m_frDatabase->setMaximumHeight(0);
  m_status->setMaximumHeight(0);

  setLayout(v1);
  setWindowFlags(Qt::WindowCloseButtonHint);
  layout()->setSizeConstraint(QLayout::SetFixedSize);

  QObject::connect(m_btnLogin,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(login()));

  QObject::connect(m_btnDatabaseOpt,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(showDatabaseOpt()));

  QObject::connect(m_password,
                   SIGNAL(enterSignal()),
                   this,
                   SLOT(focusLogin()));

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
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_status->setText("");
  m_status->setMaximumHeight(0);
  QString error;
  bool bSuccess = BaitaSQL::init(m_hostName->text(), m_port->value(), error);
  if (bSuccess)
  {
    bSuccess = m_userLogin.login(m_user->text(), m_password->text(), error);
    if (bSuccess)
    {
      Settings settings;
      settings.load();
      settings.m_databaseHostName = m_hostName->text();
      settings.m_databasePort = m_port->value();
      settings.save();
      accept();
    }
    else
    {
      m_password->selectAll();
      m_password->setFocus();
    }
  }
  else
  {
    m_hostName->selectAll();
    m_hostName->setFocus();
  }

  if (!bSuccess)
  {
    m_status->setText(error);
    m_status->setMaximumHeight(m_status->sizeHint().height());
  }

  QApplication::restoreOverrideCursor();
}

void LoginDialog::showDatabaseOpt()
{
  if (m_frDatabase->maximumHeight() == 0)
  {
    QPropertyAnimation* an = new QPropertyAnimation(m_frDatabase, "maximumHeight");
    an->setDuration(500);
    an->setStartValue(0);
    an->setEndValue(m_frDatabase->sizeHint().height());
    an->start();
    m_btnDatabaseOpt->setIcon(QIcon(":/icons/res/arrowdropup.png"));
  }
  else
  {
    QPropertyAnimation *an = new QPropertyAnimation(m_frDatabase, "maximumHeight");
    an->setDuration(500);
    an->setStartValue(m_frDatabase->sizeHint().height());
    an->setEndValue(0);
    an->start();
    m_btnDatabaseOpt->setIcon(QIcon(":/icons/res/arrowdropdown.png"));
  }
}

void LoginDialog::focusLogin()
{
  m_btnLogin->setFocus();
}

#include "logindialog.h"
#include "jlineedit.h"
#include <QLayout>
#include <QIcon>
#include <QLabel>
#include <QDialogButtonBox>
#include <QRegExpValidator>

LoginDialog::LoginDialog(QWidget* parent)
  : QDialog(parent)
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
  m_user = new JLineEdit(true, true);
  m_user->setPlaceholderText(tr("Usuário"));
  m_user->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]*"), this));
  m_user->setMinimumHeight(24);
  m_user->setMaximumHeight(24);
  {
    QFont f = m_user->font();
    f.setPointSize(12);
    m_user->setFont(f);
  }

  m_password = new JLineEdit(false, true);
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
  m_status->setText("Mensagem de status");

  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                     | QDialogButtonBox::Close);

  QVBoxLayout *v1 = new QVBoxLayout();
  v1->addLayout(h0);
  v1->addLayout(h1);
  v1->addLayout(h2);
  v1->addWidget(m_status);
  v1->addWidget(buttonBox);

  setLayout(v1);
  setWindowFlags(Qt::Window);
  layout()->setSizeConstraint(QLayout::SetFixedSize);

  QObject::connect(buttonBox,
                   SIGNAL(accepted()),
                   this,
                   SLOT(accept()));

  QObject::connect(buttonBox,
                   SIGNAL(rejected()),
                   this,
                   SLOT(reject()));

  setWindowTitle(tr("Baita Assistente Login"));
  setWindowIcon(QIcon(":/icons/res/login.png"));
  setModal(true);
}

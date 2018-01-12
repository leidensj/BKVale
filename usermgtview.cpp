#include "usermgtview.h"
#include <QCheckBox>
#include "jlineedit.h"
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QRegExpValidator>
#include <QGroupBox>
#include <QMessageBox>

UserMgtView::UserMgtView(QWidget* parent)
  : QFrame(parent)
  , m_currentID(INVALID_USER_ID)
  , m_user(nullptr)
  , m_lblPasswordMsg(nullptr)
  , m_password(nullptr)
  , m_viewPassword(nullptr)
  , m_accessNote(nullptr)
  , m_accessReminder(nullptr)
  , m_accessCalculator(nullptr)
  , m_accessShop(nullptr)
  , m_accessConsumption(nullptr)
  , m_accessUser(nullptr)
  , m_accessItem(nullptr)
  , m_accessSettings(nullptr)
{
  m_create = new QPushButton();
  m_create->setFlat(true);
  m_create->setText("");
  m_create->setIconSize(QSize(24, 24));
  m_create->setIcon(QIcon(":/icons/res/file.png"));
  m_create->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

  m_save = new QPushButton();
  m_save->setFlat(true);
  m_save->setText("");
  m_save->setIconSize(QSize(24, 24));
  m_save->setIcon(QIcon(":/icons/res/save.png"));
  m_save->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

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
  m_user->setMaxLength(USER_MAX_USERNAME_LENGTH);
  m_user->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]*"), this));

  m_lblPasswordMsg = new QLabel();
  m_lblPasswordMsg->setText(tr("Redefinir a senha:"));

  m_password = new JLineEdit(false, true);
  m_password->setPlaceholderText(tr("Senha"));
  m_user->setMaxLength(USER_MAX_PASSWORD_LENGTH);
  m_password->setEchoMode(QLineEdit::EchoMode::Password);

  m_viewPassword = new QPushButton();
  m_viewPassword->setFlat(true);
  m_viewPassword->setText("");
  m_viewPassword->setIconSize(QSize(16, 16));
  m_viewPassword->setIcon(QIcon(":/icons/res/view.png"));
  m_viewPassword->setCheckable(true);

  m_accessNote = new QCheckBox();
  m_accessNote->setIcon(QIcon(":/icons/res/note.png"));
  m_accessNote->setText(tr("Vales"));
  m_accessReminder = new QCheckBox();
  m_accessReminder->setIcon(QIcon(":/icons/res/postit.png"));
  m_accessReminder->setText(tr("Lembretes"));
  m_accessCalculator = new QCheckBox();
  m_accessCalculator->setIcon(QIcon(":/icons/res/calculator.png"));
  m_accessCalculator->setText(tr("Calculadora"));
  m_accessShop = new QCheckBox();
  m_accessShop->setIcon(QIcon(":/icons/res/shop.png"));
  m_accessShop->setText(tr("Compras"));
  m_accessConsumption = new QCheckBox();
  m_accessConsumption->setIcon(QIcon(":/icons/res/stock.png"));
  m_accessConsumption->setText(tr("Consumo"));

  m_accessUser = new QCheckBox();
  m_accessUser->setIcon(QIcon(":/icons/res/user.png"));
  m_accessUser->setText(tr("Usuários"));
  m_accessItem = new QCheckBox();
  m_accessItem->setIcon(QIcon(":/icons/res/item.png"));
  m_accessItem->setText(tr("Itens"));
  m_accessSettings = new QCheckBox();
  m_accessSettings->setIcon(QIcon(":/icons/res/settings.png"));
  m_accessSettings->setText(tr("Configurações"));

  QHBoxLayout* h0 = new QHBoxLayout();
  h0->setContentsMargins(0, 0, 0, 0);
  h0->addWidget(m_create);
  h0->addWidget(m_save);
  h0->setAlignment(Qt::AlignLeft);

  QHBoxLayout* h1 = new QHBoxLayout();
  h1->setContentsMargins(0, 0, 0, 0);
  h1->addWidget(lblUser);
  h1->addWidget(m_user);

  QHBoxLayout* h2 = new QHBoxLayout();
  h2->setContentsMargins(0, 0, 0, 0);
  h2->addWidget(lblPassword);
  h2->addWidget(m_password);
  h2->addWidget(m_viewPassword);

  QVBoxLayout* v0 = new QVBoxLayout();
  v0->addWidget(m_accessNote);
  v0->addWidget(m_accessReminder);
  v0->addWidget(m_accessCalculator);
  v0->addWidget(m_accessShop);
  v0->addWidget(m_accessConsumption);
  v0->addWidget(m_accessUser);
  v0->addWidget(m_accessItem);
  v0->addWidget(m_accessSettings);

  QGroupBox* gbox = new QGroupBox();
  gbox->setTitle(tr("Permissões"));
  gbox->setLayout(v0);

  QVBoxLayout* v1 = new QVBoxLayout();
  v1->addLayout(h0);
  v1->addLayout(h1);
  v1->addWidget(m_lblPasswordMsg);
  v1->addLayout(h2);
  v1->addWidget(gbox);
  setLayout(v1);

  QObject::connect(m_create,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));

  QObject::connect(m_save,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSaveSignal()));

  QObject::connect(m_viewPassword,
                   SIGNAL(toggled(bool)),
                   this,
                   SLOT(viewPassword(bool)));
  create();
}

User UserMgtView::getUser() const
{
  User user;
  user.m_id = m_currentID;
  user.m_strUser = m_user->text();
  user.m_bAccessNote = m_accessNote->isChecked();
  user.m_bAccessReminder = m_accessReminder->isChecked();
  user.m_bAccessCalculator = m_accessCalculator->isChecked();
  user.m_bAccessShop = m_accessShop->isChecked();
  user.m_bAccessConsumption = m_accessConsumption->isChecked();
  user.m_bAccessUser = m_accessUser->isChecked();
  user.m_bAccessItem = m_accessItem->isChecked();
  user.m_bAccessSettings = m_accessSettings->isChecked();
  return user;
}

void UserMgtView::setUser(const User& user)
{
  m_lblPasswordMsg->show();
  m_currentID = user.m_id;
  m_user->setText(user.m_strUser);
  m_password->setText("");
  m_accessNote->setChecked(user.m_bAccessNote);
  m_accessReminder->setChecked(user.m_bAccessReminder);
  m_accessCalculator->setChecked(user.m_bAccessCalculator);
  m_accessShop->setChecked(user.m_bAccessShop);
  m_accessConsumption->setChecked(user.m_bAccessConsumption);
  m_accessUser->setChecked(user.m_bAccessUser);
  m_accessItem->setChecked(user.m_bAccessItem);
  m_accessSettings->setChecked(user.m_bAccessSettings);
}

void UserMgtView::create()
{
  m_lblPasswordMsg->hide();
  m_currentID = INVALID_USER_ID;
  m_user->setText("");
  m_password->setText("");
  m_accessNote->setChecked(false);
  m_accessReminder->setChecked(false);
  m_accessCalculator->setChecked(false);
  m_accessShop->setChecked(false);
  m_accessConsumption->setChecked(false);
  m_accessUser->setChecked(false);
  m_accessItem->setChecked(false);
  m_accessSettings->setChecked(false);
  m_user->setFocus();
}

void UserMgtView::emitSaveSignal()
{
  if (m_user->text().isEmpty())
  {

    QMessageBox::warning(this,
                          tr("Usuário inválido"),
                          tr("O nome de usuário é obrigatório."),
                          QMessageBox::Ok);
  }
  else
  {
    emit saveSignal();
  }
}

QString UserMgtView::getPassword() const
{
  return m_password->text();
}

void UserMgtView::viewPassword(bool b)
{
  m_password->setEchoMode( b ? QLineEdit::EchoMode::Normal
                             : QLineEdit::EchoMode::Password);
}

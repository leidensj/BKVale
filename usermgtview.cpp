#include "usermgtview.h"
#include <QCheckBox>
#include "jlineedit.h"
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QRegExpValidator>
#include <QGroupBox>
#include <QMessageBox>
#include <QTabWidget>

UserMgtView::UserMgtView(QWidget* parent)
  : QFrame(parent)
  , m_currentID(INVALID_ID)
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
  , m_accessProduct(nullptr)
  , m_accessPerson(nullptr)
  , m_accessCategory(nullptr)
  , m_accessImage(nullptr)
  , m_accessSettings(nullptr)
{
  m_create = new QPushButton;
  m_create->setFlat(true);
  m_create->setText("");
  m_create->setIconSize(QSize(24, 24));
  m_create->setIcon(QIcon(":/icons/res/file.png"));
  m_create->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

  m_save = new QPushButton;
  m_save->setFlat(true);
  m_save->setText("");
  m_save->setIconSize(QSize(24, 24));
  m_save->setIcon(QIcon(":/icons/res/save.png"));
  m_save->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

  QLabel* lblUser = new QLabel;
  lblUser->setPixmap(QIcon(":/icons/res/user.png").pixmap(QSize(24, 24)));
  lblUser->setMinimumSize(24, 24);
  lblUser->setMaximumSize(24, 24);
  lblUser->setScaledContents(true);
  QLabel* lblPassword = new QLabel;
  lblPassword->setPixmap(QIcon(":/icons/res/password.png").pixmap(QSize(24, 24)));
  lblPassword->setMinimumSize(24, 24);
  lblPassword->setMaximumSize(24, 24);
  lblPassword->setScaledContents(true);

  m_user = new JLineEdit(JValidatorType::Alphanumeric, true, true);
  m_user->setPlaceholderText(tr("Usuário"));
  m_user->setMaxLength(USER_MAX_USERNAME_LENGTH);

  m_lblPasswordMsg = new QLabel;
  m_lblPasswordMsg->setText(tr("Redefinir a senha:"));

  m_password = new JLineEdit(JValidatorType::All, false, true);
  m_password->setPlaceholderText(tr("Senha"));
  m_user->setMaxLength(USER_MAX_PASSWORD_LENGTH);
  m_password->setEchoMode(QLineEdit::EchoMode::Password);

  m_viewPassword = new QPushButton;
  m_viewPassword->setFlat(true);
  m_viewPassword->setText("");
  m_viewPassword->setIconSize(QSize(16, 16));
  m_viewPassword->setIcon(QIcon(":/icons/res/view.png"));
  m_viewPassword->setCheckable(true);

  m_accessNote = new QCheckBox;
  m_accessNote->setIcon(QIcon(":/icons/res/note.png"));
  m_accessNote->setText(tr("Vales"));
  m_accessReminder = new QCheckBox;
  m_accessReminder->setIcon(QIcon(":/icons/res/postit.png"));
  m_accessReminder->setText(tr("Lembretes"));
  m_accessCalculator = new QCheckBox;
  m_accessCalculator->setIcon(QIcon(":/icons/res/calculator.png"));
  m_accessCalculator->setText(tr("Calculadora"));
  m_accessShop = new QCheckBox;
  m_accessShop->setIcon(QIcon(":/icons/res/shop.png"));
  m_accessShop->setText(tr("Compras"));
  m_accessConsumption = new QCheckBox;
  m_accessConsumption->setIcon(QIcon(":/icons/res/stock.png"));
  m_accessConsumption->setText(tr("Consumo"));

  m_accessUser = new QCheckBox;
  m_accessUser->setIcon(QIcon(":/icons/res/user.png"));
  m_accessUser->setText(tr("Usuários"));

  m_accessProduct = new QCheckBox;
  m_accessProduct->setIcon(QIcon(":/icons/res/item.png"));
  m_accessProduct->setText(tr("Produtos"));

  m_accessPerson = new QCheckBox;
  m_accessPerson->setIcon(QIcon(":/icons/res/person.png"));
  m_accessPerson->setText(tr("pessoas"));

  m_accessCategory = new QCheckBox;
  m_accessCategory->setIcon(QIcon(":/icons/res/category.png"));
  m_accessCategory->setText(tr("Itens"));

  m_accessImage = new QCheckBox;
  m_accessImage->setIcon(QIcon(":/icons/res/image.png"));
  m_accessImage->setText(tr("Imagens"));

  m_accessSettings = new QCheckBox;
  m_accessSettings->setIcon(QIcon(":/icons/res/settings.png"));
  m_accessSettings->setText(tr("Configurações"));

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->addWidget(m_create);
  buttonlayout->addWidget(m_save);
  buttonlayout->setAlignment(Qt::AlignLeft);

  QHBoxLayout* userlayout = new QHBoxLayout;
  userlayout->setContentsMargins(0, 0, 0, 0);
  userlayout->addWidget(lblUser);
  userlayout->addWidget(m_user);

  QHBoxLayout* passwordlayout = new QHBoxLayout;
  passwordlayout->setContentsMargins(0, 0, 0, 0);
  passwordlayout->addWidget(lblPassword);
  passwordlayout->addWidget(m_password);
  passwordlayout->addWidget(m_viewPassword);

  QVBoxLayout* grouplayout = new QVBoxLayout;
  grouplayout->addWidget(m_accessNote);
  grouplayout->addWidget(m_accessReminder);
  grouplayout->addWidget(m_accessCalculator);
  grouplayout->addWidget(m_accessShop);
  grouplayout->addWidget(m_accessConsumption);
  grouplayout->addWidget(m_accessUser);
  grouplayout->addWidget(m_accessProduct);
  grouplayout->addWidget(m_accessPerson);
  grouplayout->addWidget(m_accessCategory);
  grouplayout->addWidget(m_accessImage);
  grouplayout->addWidget(m_accessSettings);

  QGroupBox* groupbox = new QGroupBox;
  groupbox->setTitle(tr("Permissões"));
  groupbox->setLayout(grouplayout);

  QVBoxLayout* tablayout = new QVBoxLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addLayout(userlayout);
  tablayout->addWidget(m_lblPasswordMsg);
  tablayout->addLayout(passwordlayout);
  tablayout->addWidget(groupbox);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  QTabWidget* tabWidget = new QTabWidget;
  tabWidget->addTab(tabframe,
                    QIcon(":/icons/res/user.png"),
                    tr("Usuário"));

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->addLayout(buttonlayout);
  mainlayout->addWidget(tabWidget);
  setLayout(mainlayout);

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
  user.m_bAccessProduct = m_accessProduct->isChecked();
  user.m_bAccessPerson = m_accessPerson->isChecked();
  user.m_bAccessCategory = m_accessCategory->isChecked();
  user.m_bAccessImage = m_accessImage->isChecked();
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
  m_accessProduct->setChecked(user.m_bAccessProduct);
  m_accessPerson->setChecked(user.m_bAccessProduct);
  m_accessCategory->setChecked(user.m_bAccessCategory);
  m_accessImage->setChecked(user.m_bAccessImage);
  m_accessSettings->setChecked(user.m_bAccessSettings);
}

void UserMgtView::create()
{
  m_lblPasswordMsg->hide();
  m_currentID = INVALID_ID;
  m_user->setText("");
  m_password->setText("");
  m_accessNote->setChecked(false);
  m_accessReminder->setChecked(false);
  m_accessCalculator->setChecked(false);
  m_accessShop->setChecked(false);
  m_accessConsumption->setChecked(false);
  m_accessUser->setChecked(false);
  m_accessProduct->setChecked(false);
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

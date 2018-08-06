#include "usermgtview.h"
#include <QCheckBox>
#include "jlineedit.h"
#include "jdatabase.h"
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QRegExpValidator>
#include <QGroupBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QSplitter>

UserMgtView::UserMgtView(qlonglong currentLoggedId, QWidget* parent)
  : QFrame(parent)
  , m_currentLoggedId(currentLoggedId)
  , m_currentId(INVALID_ID)
  , m_bHasLoggedUserChanged(false)
  , m_user(nullptr)
  , m_lblPasswordMsg(nullptr)
  , m_password(nullptr)
  , m_viewPassword(nullptr)
  , m_accessNote(nullptr)
  , m_accessReminder(nullptr)
  , m_accessCalculator(nullptr)
  , m_accessShoppingList(nullptr)
  , m_accessShop(nullptr)
  , m_accessUser(nullptr)
  , m_accessProduct(nullptr)
  , m_accessPerson(nullptr)
  , m_accessEmployee(nullptr)
  , m_accessSupplier(nullptr)
  , m_accessCategory(nullptr)
  , m_accessImage(nullptr)
  , m_accessReservation(nullptr)
  , m_accessSettings(nullptr)
  , m_database(nullptr)
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

  m_user = new JLineEdit(JLineEdit::Input::Alphanumeric, JLineEdit::st_defaultFlags1);
  m_user->setPlaceholderText(tr("Usuário"));
  m_user->setMaxLength(USER_MAX_USERNAME_LENGTH);

  m_lblPasswordMsg = new QLabel;
  m_lblPasswordMsg->setText(tr("Redefinir a senha:"));

  m_password = new JLineEdit(JLineEdit::Input::All, JLineEdit::st_defaultFlags2);
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
  m_accessShoppingList = new QCheckBox;
  m_accessShoppingList->setIcon(QIcon(":/icons/res/shopmgt.png"));
  m_accessShoppingList->setText(tr("Listas de Compras"));

  m_accessUser = new QCheckBox;
  m_accessUser->setIcon(QIcon(":/icons/res/user.png"));
  m_accessUser->setText(tr("Usuários"));

  m_accessProduct = new QCheckBox;
  m_accessProduct->setIcon(QIcon(":/icons/res/item.png"));
  m_accessProduct->setText(tr("Produtos"));

  m_accessPerson = new QCheckBox;
  m_accessPerson->setIcon(QIcon(":/icons/res/person.png"));
  m_accessPerson->setText(tr("Pessoas"));

  m_accessEmployee = new QCheckBox;
  m_accessEmployee->setIcon(QIcon(":/icons/res/employee.png"));
  m_accessEmployee->setText(tr("Funcionários"));

  m_accessSupplier = new QCheckBox;
  m_accessSupplier->setIcon(QIcon(":/icons/res/supplier.png"));
  m_accessSupplier->setText(tr("Fornecedores"));

  m_accessCategory = new QCheckBox;
  m_accessCategory->setIcon(QIcon(":/icons/res/category.png"));
  m_accessCategory->setText(tr("Categorias"));

  m_accessImage = new QCheckBox;
  m_accessImage->setIcon(QIcon(":/icons/res/icon.png"));
  m_accessImage->setText(tr("Imagens"));

  m_accessReservation = new QCheckBox;
  m_accessReservation->setIcon(QIcon(":/icons/res/reservation.png"));
  m_accessReservation->setText(tr("Reservas"));

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

  QVBoxLayout* tabPermissionslayout = new QVBoxLayout;
  tabPermissionslayout->addWidget(m_accessNote);
  tabPermissionslayout->addWidget(m_accessReminder);
  tabPermissionslayout->addWidget(m_accessCalculator);
  tabPermissionslayout->addWidget(m_accessShop);
  tabPermissionslayout->addWidget(m_accessReservation);
  tabPermissionslayout->addWidget(m_accessUser);
  tabPermissionslayout->addWidget(m_accessProduct);
  tabPermissionslayout->addWidget(m_accessPerson);
  tabPermissionslayout->addWidget(m_accessEmployee);
  tabPermissionslayout->addWidget(m_accessSupplier);
  tabPermissionslayout->addWidget(m_accessCategory);
  tabPermissionslayout->addWidget(m_accessImage);
  tabPermissionslayout->addWidget(m_accessShoppingList);
  tabPermissionslayout->addWidget(m_accessSettings);

  QVBoxLayout* tabUserlayout = new QVBoxLayout;
  tabUserlayout->setAlignment(Qt::AlignTop);
  tabUserlayout->addLayout(userlayout);
  tabUserlayout->addWidget(m_lblPasswordMsg);
  tabUserlayout->addLayout(passwordlayout);

  QFrame* tabUserFrame = new QFrame;
  tabUserFrame->setLayout(tabUserlayout);

  QFrame* tabPermissionsFrame = new QFrame;
  tabPermissionsFrame->setLayout(tabPermissionslayout);

  QTabWidget* tabWidget = new QTabWidget;
  tabWidget->addTab(tabUserFrame,
                    QIcon(":/icons/res/user.png"),
                    tr("Usuário"));

  tabWidget->addTab(tabPermissionsFrame,
                    QIcon(":/icons/res/usershield.png"),
                    tr("Permissões"));

  m_database = new JDatabase(USER_SQL_TABLE_NAME);
  m_database->setContentsMargins(0, 0, 9, 0);

  QVBoxLayout* viewLayout = new QVBoxLayout;
  viewLayout->addLayout(buttonlayout);
  viewLayout->addWidget(tabWidget);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewLayout);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(viewFrame);

  QHBoxLayout* mainlayout = new QHBoxLayout();
  mainlayout->addWidget(splitter);
  mainlayout->setContentsMargins(0, 0, 0, 0);
  setLayout(mainlayout);

  QObject::connect(m_create,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));

  QObject::connect(m_save,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save()));

  QObject::connect(m_viewPassword,
                   SIGNAL(toggled(bool)),
                   this,
                   SLOT(viewPassword(bool)));

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));

  QObject::connect(m_database,
                   SIGNAL(itemRemovedSignal(qlonglong)),
                   this,
                   SLOT(itemRemoved(qlonglong)));

  create();
}

User UserMgtView::getUser() const
{
  User user;
  user.m_id = m_currentId;
  user.m_strUser = m_user->text();
  user.m_password = m_password->text();
  user.m_bAccessNote = m_accessNote->isChecked();
  user.m_bAccessReminder = m_accessReminder->isChecked();
  user.m_bAccessCalculator = m_accessCalculator->isChecked();
  user.m_bAccessShop = m_accessShop->isChecked();
  user.m_bAccessUser = m_accessUser->isChecked();
  user.m_bAccessProduct = m_accessProduct->isChecked();
  user.m_bAccessPerson = m_accessPerson->isChecked();
  user.m_bAccessEmployee = m_accessEmployee->isChecked();
  user.m_bAccessSupplier = m_accessSupplier->isChecked();
  user.m_bAccessCategory = m_accessCategory->isChecked();
  user.m_bAccessImage = m_accessImage->isChecked();
  user.m_bAccessSettings = m_accessSettings->isChecked();
  user.m_bAccessReservation = m_accessReservation->isChecked();
  user.m_bAccessShoppingList = m_accessShoppingList->isChecked();
  return user;
}

void UserMgtView::setUser(const User& user)
{
  m_save->setIcon(QIcon(user.isValidId()
                        ? ":/icons/res/saveas.png"
                        : ":/icons/res/save.png"));
  if (user.isValidId())
    m_lblPasswordMsg->show();
  m_currentId = user.m_id;
  m_user->setText(user.m_strUser);
  m_password->setText("");
  m_accessNote->setChecked(user.m_bAccessNote);
  m_accessReminder->setChecked(user.m_bAccessReminder);
  m_accessCalculator->setChecked(user.m_bAccessCalculator);
  m_accessShop->setChecked(user.m_bAccessShop);
  m_accessUser->setChecked(user.m_bAccessUser);
  m_accessProduct->setChecked(user.m_bAccessProduct);
  m_accessPerson->setChecked(user.m_bAccessPerson);
  m_accessEmployee->setChecked(user.m_bAccessEmployee);
  m_accessSupplier->setChecked(user.m_bAccessSupplier);
  m_accessCategory->setChecked(user.m_bAccessCategory);
  m_accessImage->setChecked(user.m_bAccessImage);
  m_accessSettings->setChecked(user.m_bAccessSettings);
  m_accessReservation->setChecked(user.m_bAccessReservation);
  m_accessShoppingList->setChecked(user.m_bAccessShoppingList);
  m_user->setFocus();
}

void UserMgtView::create()
{
  m_lblPasswordMsg->hide();
  User user;
  setUser(user);
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

void UserMgtView::itemSelected(const JItem& jItem)
{
  const User& user = dynamic_cast<const User&>(jItem);
  if (user.isValidId())
    setUser(user);
}

void UserMgtView::itemRemoved(qlonglong id)
{
  if (!m_bHasLoggedUserChanged)
    m_bHasLoggedUserChanged = m_bHasLoggedUserChanged == id;
  if (m_currentId == id)
    create();
}

void UserMgtView::save()
{
  if (m_database->save(getUser()))
  {
    if (!m_bHasLoggedUserChanged)
      m_bHasLoggedUserChanged = m_currentId == m_currentLoggedId;
    create();
  }
}

bool UserMgtView::hasLoggedUserChanged() const
{
  return m_bHasLoggedUserChanged;
}

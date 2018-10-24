#include "usermgtview.h"
#include <QCheckBox>
#include "jlineedit.h"
#include <QLabel>
#include <QLayout>
#include <QRegExpValidator>
#include <QGroupBox>
#include <QMessageBox>

UserMgtView::UserMgtView(Id currentLoggedId, QWidget* parent)
  : JItemView(USER_SQL_TABLE_NAME, parent)
  , m_currentLoggedId(currentLoggedId)
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
  , m_accessProductBarcode(nullptr)
{
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

  m_accessProductBarcode = new QCheckBox;
  m_accessProductBarcode->setIcon(QIcon(":/icons/res/barcode.png"));
  m_accessProductBarcode->setText(tr("Códigos"));

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
  tabPermissionslayout->addWidget(m_accessProductBarcode);
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

  m_tab->addTab(tabUserFrame,
                QIcon(":/icons/res/user.png"),
                tr("Usuário"));

  m_tab->addTab(tabPermissionsFrame,
                QIcon(":/icons/res/usershield.png"),
                tr("Permissões"));

  QObject::connect(m_viewPassword,
                   SIGNAL(toggled(bool)),
                   this,
                   SLOT(viewPassword(bool)));

  create();
}

const JItem& UserMgtView::getItem() const
{
  static User o;
  o.m_id = m_currentId;
  o.m_strUser = m_user->text();
  o.m_password = m_password->text();
  o.m_bAccessNote = m_accessNote->isChecked();
  o.m_bAccessReminder = m_accessReminder->isChecked();
  o.m_bAccessCalculator = m_accessCalculator->isChecked();
  o.m_bAccessShop = m_accessShop->isChecked();
  o.m_bAccessUser = m_accessUser->isChecked();
  o.m_bAccessProduct = m_accessProduct->isChecked();
  o.m_bAccessPerson = m_accessPerson->isChecked();
  o.m_bAccessEmployee = m_accessEmployee->isChecked();
  o.m_bAccessSupplier = m_accessSupplier->isChecked();
  o.m_bAccessCategory = m_accessCategory->isChecked();
  o.m_bAccessImage = m_accessImage->isChecked();
  o.m_bAccessSettings = m_accessSettings->isChecked();
  o.m_bAccessReservation = m_accessReservation->isChecked();
  o.m_bAccessShoppingList = m_accessShoppingList->isChecked();
  o.m_bAccessProductBarcode = m_accessProductBarcode->isChecked();
  return o;
}

void UserMgtView::setItem(const JItem& o)
{
  auto _o = dynamic_cast<const User&>(o);
  if (_o.m_id.isValid())
    m_lblPasswordMsg->show();
  m_currentId = _o.m_id;
  m_user->setText(_o.m_strUser);
  m_password->setText("");
  m_accessNote->setChecked(_o.m_bAccessNote);
  m_accessReminder->setChecked(_o.m_bAccessReminder);
  m_accessCalculator->setChecked(_o.m_bAccessCalculator);
  m_accessShop->setChecked(_o.m_bAccessShop);
  m_accessUser->setChecked(_o.m_bAccessUser);
  m_accessProduct->setChecked(_o.m_bAccessProduct);
  m_accessPerson->setChecked(_o.m_bAccessPerson);
  m_accessEmployee->setChecked(_o.m_bAccessEmployee);
  m_accessSupplier->setChecked(_o.m_bAccessSupplier);
  m_accessCategory->setChecked(_o.m_bAccessCategory);
  m_accessImage->setChecked(_o.m_bAccessImage);
  m_accessSettings->setChecked(_o.m_bAccessSettings);
  m_accessReservation->setChecked(_o.m_bAccessReservation);
  m_accessShoppingList->setChecked(_o.m_bAccessShoppingList);
  m_accessProductBarcode->setChecked(_o.m_bAccessProductBarcode);
}

void UserMgtView::create()
{
  selectItem(User());
  m_lblPasswordMsg->hide();
  m_user->setFocus();
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

void UserMgtView::itemsRemoved(const QVector<Id>& ids)
{
  if (!m_bHasLoggedUserChanged)
    m_bHasLoggedUserChanged = ids.contains(m_bHasLoggedUserChanged);
  JItemView::itemsRemoved(ids);
}

void UserMgtView::save()
{
  if (!m_bHasLoggedUserChanged)
    m_bHasLoggedUserChanged = m_currentId == m_currentLoggedId;
  JItemView::save();
}

bool UserMgtView::hasLoggedUserChanged() const
{
  return m_bHasLoggedUserChanged;
}

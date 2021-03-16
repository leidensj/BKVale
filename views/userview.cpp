#include "userview.h"
#include "items/jitemex.h"
#include "items/login.h"
#include "widgets/jlineedit.h"
#include <QLabel>
#include <QLayout>
#include <QRegExpValidator>
#include <QGroupBox>
#include <QMessageBox>
#include <QListWidget>

UserView::UserView(QWidget* parent)
  : JItemView(USER_SQL_TABLE_NAME, parent)
  , m_bHasLoggedUserChanged(false)
  , m_user(nullptr)
  , m_lblPasswordMsg(nullptr)
  , m_password(nullptr)
  , m_viewPassword(nullptr)
  , m_list(nullptr)
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

  m_user = new JLineEdit(Text::Input::Alphanumeric, true);
  m_user->setPlaceholderText(tr("Usuário"));
  m_user->setMaxLength(USER_MAX_USERNAME_LENGTH);

  m_lblPasswordMsg = new QLabel;
  m_lblPasswordMsg->setText(tr("Redefinir a senha:"));

  m_password = new JLineEdit(Text::Input::All, false);
  m_password->setPlaceholderText(tr("Senha"));
  m_user->setMaxLength(USER_MAX_PASSWORD_LENGTH);
  m_password->setEchoMode(QLineEdit::EchoMode::Password);

  m_viewPassword = new QPushButton;
  m_viewPassword->setFlat(true);
  m_viewPassword->setIconSize(QSize(16, 16));
  m_viewPassword->setIcon(QIcon(":/icons/res/view.png"));
  m_viewPassword->setCheckable(true);

  m_list = new QListWidget;
  for (int i = 0; i != (int)Functionality::Idx::_END; ++i)
  {
    QListWidgetItem* p = new QListWidgetItem;
    p->setText(JItemEx::text((Functionality::Idx)i));
    p->setIcon(QIcon(JItemEx::icon((Functionality::Idx)i)));
    p->setFlags(p->flags() | Qt::ItemIsUserCheckable);
    p->setCheckState(Qt::Unchecked);
    m_list->addItem(p);
  }

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
  tabPermissionslayout->addWidget(m_list);

  QVBoxLayout* tabUserlayout = new QVBoxLayout;
  tabUserlayout->setAlignment(Qt::AlignTop);
  tabUserlayout->addLayout(userlayout);
  tabUserlayout->addWidget(m_lblPasswordMsg);
  tabUserlayout->addLayout(passwordlayout);

  QFrame* tabUserFrame = new QFrame;
  tabUserFrame->setLayout(tabUserlayout);

  QFrame* tabPermissionsFrame = new QFrame;
  tabPermissionsFrame->setLayout(tabPermissionslayout);

  m_tab->addTab(tabUserFrame, QIcon(":/icons/res/user.png"), tr("Usuário"));
  m_tab->addTab(tabPermissionsFrame, QIcon(":/icons/res/usershield.png"),tr("Permissões"));

  connect(m_viewPassword, SIGNAL(toggled(bool)), this, SLOT(viewPassword(bool)));

  setFocusWidgetOnClear(m_user);
  clear();
}

void UserView::getItem(JItemSQL& o) const
{
  User& _o = dynamic_cast<User&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_strUser = m_user->text();
  _o.m_password = m_password->text();
  _o.m_bPurchase = m_list->item((int)Idx::Purchase)->checkState() == Qt::Checked ? true : false;
  _o.m_bReminder = m_list->item((int)Idx::Reminder)->checkState() == Qt::Checked ? true : false;
  _o.m_bCalculator = m_list->item((int)Idx::Calculator)->checkState() == Qt::Checked ? true : false;
  _o.m_bShop = m_list->item((int)Idx::Shop)->checkState() == Qt::Checked ? true : false;
  _o.m_bUser = m_list->item((int)Idx::User)->checkState() == Qt::Checked ? true : false;
  _o.m_bProduct = m_list->item((int)Idx::Product)->checkState() == Qt::Checked ? true : false;
  _o.m_bSettings = m_list->item((int)Idx::Settings)->checkState() == Qt::Checked ? true : false;
  _o.m_bEmployee = m_list->item((int)Idx::Employee)->checkState() == Qt::Checked ? true : false;
  _o.m_bSupplier = m_list->item((int)Idx::Supplier)->checkState() == Qt::Checked ? true : false;
  _o.m_bCategory = m_list->item((int)Idx::Category)->checkState() == Qt::Checked ? true : false;
  _o.m_bImage = m_list->item((int)Idx::Image)->checkState() == Qt::Checked ? true : false;
  _o.m_bShoppingList = m_list->item((int)Idx::ShoppingList)->checkState() == Qt::Checked ? true : false;
  _o.m_bStore = m_list->item((int)Idx::Store)->checkState() == Qt::Checked ? true : false;
  _o.m_bTimeCard = m_list->item((int)Idx::TimeCard)->checkState() == Qt::Checked ? true : false;
  _o.m_bCoupon = m_list->item((int)Idx::Coupon)->checkState() == Qt::Checked ? true : false;
}

void UserView::setItem(const JItemSQL& o)
{
  const User& _o = dynamic_cast<const User&>(o);
  if (_o.m_id.isValid())
    m_lblPasswordMsg->show();
  m_user->setText(_o.m_strUser);
  m_password->setText("");
  m_list->item((int)Idx::User)->setCheckState(_o.m_bUser ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Category)->setCheckState(_o.m_bCategory ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Product)->setCheckState(_o.m_bProduct ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Image)->setCheckState(_o.m_bImage ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Employee)->setCheckState(_o.m_bEmployee ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Supplier)->setCheckState(_o.m_bSupplier ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Store)->setCheckState(_o.m_bStore ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Purchase)->setCheckState(_o.m_bPurchase ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Calculator)->setCheckState(_o.m_bCalculator ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Reminder)->setCheckState(_o.m_bReminder ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::ShoppingList)->setCheckState(_o.m_bShoppingList ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Shop)->setCheckState(_o.m_bShop ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Settings)->setCheckState(_o.m_bSettings ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::TimeCard)->setCheckState(_o.m_bTimeCard ? Qt::Checked : Qt::Unchecked);
  m_list->item((int)Idx::Coupon)->setCheckState(_o.m_bCoupon ? Qt::Checked : Qt::Unchecked);
}

QString UserView::getPassword() const
{
  return m_password->text();
}

void UserView::viewPassword(bool b)
{
  m_password->setEchoMode( b ? QLineEdit::EchoMode::Normal
                             : QLineEdit::EchoMode::Password);
}

void UserView::itemsRemoved(const Ids& ids)
{
  if (!m_bHasLoggedUserChanged)
    m_bHasLoggedUserChanged = ids.contains(m_bHasLoggedUserChanged);
  JItemView::itemsRemoved(ids);
}

void UserView::save()
{
  Login login(true);
  if (!m_bHasLoggedUserChanged)
    m_bHasLoggedUserChanged = m_id == login.getUser().m_id;
  JItemView::save();
}

bool UserView::hasLoggedUserChanged() const
{
  return m_bHasLoggedUserChanged;
}

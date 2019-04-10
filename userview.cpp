#include "userview.h"
#include "jitemhelper.h"
#include "jlineedit.h"
#include "jitemhelper.h"
#include <QLabel>
#include <QLayout>
#include <QRegExpValidator>
#include <QGroupBox>
#include <QMessageBox>
#include <QListWidget>

UserView::UserView(Id currentLoggedId, QWidget* parent)
  : JItemView(USER_SQL_TABLE_NAME, parent)
  , m_currentLoggedId(currentLoggedId)
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

  m_list = new QListWidget;
  for (int i = 0; i != (int)Idx::Settings; ++i)
  {
    QListWidgetItem* p = new QListWidgetItem;
    QString strIcon;
    QString strText;
    switch (i)
    {
      case (int)Idx::Calculator:
        strIcon = ":/icons/res/calculator.png";
        strText = "Calculadora";
        break;
      case (int)Idx::Shop:
        strIcon = ":/icons/res/shop.png";
        strText = "Compras";
        break;
      case (int)Idx::Settings:
        strIcon = ":/icons/res/settings.png";
        strText = "Configurações";
        break;
      default:
        strIcon = JItemHelper::icon(idxToTableName((Idx)i));
        strText = JItemHelper::text(idxToTableName((Idx)i));
        break;
    }
    p->setText(strText);
    p->setIcon(QIcon(strIcon));
    p->setFlags(p->flags() | Qt::ItemIsUserCheckable);
    p->setCheckState(Qt::Checked);
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

const JItemSQL& UserView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
  m_ref.m_strUser = m_user->text();
  m_ref.m_password = m_password->text();
  return m_ref;
}

void UserView::setItem(const JItemSQL& o)
{
  auto _o = dynamic_cast<const User&>(o);
  if (_o.m_id.isValid())
    m_lblPasswordMsg->show();
  m_currentId = _o.m_id;
  m_user->setText(_o.m_strUser);
  m_password->setText("");
}

void UserView::create()
{
  selectItem(User());
  m_lblPasswordMsg->hide();
  m_user->setFocus();
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

void UserView::itemsRemoved(const QVector<Id>& ids)
{
  if (!m_bHasLoggedUserChanged)
    m_bHasLoggedUserChanged = ids.contains(m_bHasLoggedUserChanged);
  JItemView::itemsRemoved(ids);
}

void UserView::save()
{
  if (!m_bHasLoggedUserChanged)
    m_bHasLoggedUserChanged = m_currentId == m_currentLoggedId;
  JItemView::save();
}

bool UserView::hasLoggedUserChanged() const
{
  return m_bHasLoggedUserChanged;
}

QString UserView::idxToTableName(Idx idx)
{
  switch (idx)
  {
    case Idx::User:
      return USER_SQL_TABLE_NAME;
    case Idx::Category:
      return CATEGORY_SQL_TABLE_NAME;
    case Idx::Product:
      return PRODUCT_SQL_TABLE_NAME;
    case Idx::Image:
      return IMAGE_SQL_TABLE_NAME;
    case Idx::Form:
      return FORM_SQL_TABLE_NAME;
    case Idx::Employee:
      return EMPLOYEE_SQL_TABLE_NAME;
    case Idx::Supplier:
      return SUPPLIER_SQL_TABLE_NAME;
    case Idx::Store:
      return STORE_SQL_TABLE_NAME;
    case Idx::Note:
      return NOTE_SQL_TABLE_NAME;
    case Idx::Reminder:
      return REMINDER_SQL_TABLE_NAME;
    case Idx::ShoppinList:
      return SHOPPING_LIST_SQL_TABLE_NAME;
    case Idx::Reservation:
      return RESERVATION_SQL_TABLE_NAME;
    case Idx::Discount:
      return DISCOUNT_SQL_TABLE_NAME;
    default:
      return "";
  }
}

#include "usermgtwidget.h"
#include "usermgtview.h"
#include "jdatabase.h"
#include "databaseutils.h"
#include "jtablecolumn.h"
#include <QLayout>
#include <QSplitter>
#include <QMessageBox>

UserMgtWidget::UserMgtWidget(qlonglong currentUserID, QWidget* parent)
  : QFrame(parent)
  , m_currentUserID(currentUserID)
  , m_view(nullptr)
  , m_database(nullptr)
  , m_bHasAnyUserChanged(false)
{
  m_view = new UserMgtView();
  m_view->layout()->setAlignment(Qt::AlignTop);
  m_view->layout()->setContentsMargins(9, 0, 0, 0);

  m_database = new JDatabase();
  m_database->layout()->setContentsMargins(0, 0, 9, 0);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(m_view);

  QHBoxLayout* h1 = new QHBoxLayout();
  h1->addWidget(splitter);
  h1->setContentsMargins(0, 0, 0, 0);
  setLayout(h1);

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));

  QObject::connect(m_database,
                   SIGNAL(itemRemovedSignal(qlonglong)),
                   this,
                   SLOT(itemRemoved(qlonglong)));

  QObject::connect(m_view,
                   SIGNAL(saveSignal()),
                   this,
                   SLOT(save()));

  m_view->create();
}

void UserMgtWidget::setDatabase(QSqlDatabase db)
{
  m_database->setDatabase(db, USER_SQL_TABLE_NAME, User::getColumns());
}

void UserMgtWidget::itemSelected(const JItem& jItem)
{
  const User& user = dynamic_cast<const User&>(jItem);
  if (user.isValidId())
    m_view->setUser(user);
}

void UserMgtWidget::itemRemoved(qlonglong id)
{
  if (m_view->getUser().m_id == id)
    m_view->create();
}

void UserMgtWidget::save()
{
  User user = m_view->getUser();
  if (m_database->save(user))
  {
    m_bHasAnyUserChanged = true;
    m_view->create();
  }
}

bool UserMgtWidget::hasAnyUserChanged() const
{
  return m_bHasAnyUserChanged;
}

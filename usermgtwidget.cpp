#include "usermgtwidget.h"
#include "usermgtview.h"
#include "jdatabase.h"
#include "databaseutils.h"
#include "jtablecolumn.h"
#include <QLayout>
#include <QSplitter>
#include <QMessageBox>

UserMgtWidget::UserMgtWidget(int currentUserID, QWidget* parent)
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
                   SIGNAL(itemSelectedSignal(int)),
                   this,
                   SLOT(userSelected(int)));

  QObject::connect(m_database,
                   SIGNAL(itemRemoveSignal(int)),
                   this,
                   SLOT(removeUser(int)));

  QObject::connect(m_view,
                   SIGNAL(saveSignal()),
                   this,
                   SLOT(saveUser()));

  m_view->create();
}

void UserMgtWidget::setDatabase(QSqlDatabase db)
{
  QSqlTableModel* model = new QSqlTableModel(m_database, db);
  m_database->set(model, USER_SQL_TABLE_NAME, User::getColumns());
}

void UserMgtWidget::userSelected(int id)
{
  User user;
  user.m_id = id;
  QString error;
  if (UserSQL::select(m_database->get(), user, error))
  {
    m_view->setUser(user);
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao abrir usuário com ID '%2'.").arg(error, id),
                          QMessageBox::Ok);
  }
}

void UserMgtWidget::removeUser(int id)
{
  if (id == m_currentUserID)
  {
    QMessageBox::warning(this,
                         tr("Impossível remover usuário"),
                         tr("Você não pode remover o usuário logado atualmente."),
                         QMessageBox::Ok);
    return;
  }

  User user;
  user.m_id = id;
  QString error;
  UserSQL::select(m_database->get(), user, error);
  if (QMessageBox::question(this,
                            tr("Remover usuário"),
                            tr("Tem certeza que deseja remover o usuário '%1'?").arg(user.m_strUser),
                            QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
  {
    if (UserSQL::remove(m_database->get(), user.m_id, error))
    {
      User userview = m_view->getUser();
      if (user.m_id == userview.m_id)
        m_view->create();
    }
    else
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("Erro '%1' ao remover usuário com ID '%2'.").arg(error, id),
                            QMessageBox::Ok);
    }
  }
}

void UserMgtWidget::saveUser()
{
  QString error;
  User user = m_view->getUser();
  bool bSuccess = user.isValidId()
                  ? UserSQL::update(m_database->get(), user, m_view->getPassword(), error)
                  : UserSQL::insert(m_database->get(), user, m_view->getPassword(), error);

  if (bSuccess)
  {
    m_bHasAnyUserChanged = true;
    m_view->create();
    m_database->refresh();
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao salvar usuário.").arg(error),
                          QMessageBox::Ok);
  }
}

bool UserMgtWidget::hasAnyUserChanged() const
{
  return m_bHasAnyUserChanged;
}

#include "usermgtwidget.h"
#include "usermgtview.h"
#include "usermgtdatabase.h"
#include "databaseutils.h"
#include <QLayout>
#include <QSplitter>
#include <QMessageBox>

UserMgtWidget::UserMgtWidget(int currentUserID, QWidget* parent)
  : QFrame(parent)
  , m_currentUserID(currentUserID)
  , m_view(nullptr)
  , m_database(nullptr)
{
  m_view = new UserMgtView();
  m_view->layout()->setAlignment(Qt::AlignTop);
  m_view->setContentsMargins(9, 0, 0, 0);
  m_database = new UserMgtDatabase();
  m_database->setContentsMargins(0, 0, 0, 0);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(m_view);

  QHBoxLayout* h1 = new QHBoxLayout();
  h1->addWidget(splitter);
  h1->setContentsMargins(0, 0, 0, 0);
  setLayout(h1);

  QObject::connect(m_database,
                   SIGNAL(userSelectedSignal(int)),
                   this,
                   SLOT(setUser(int)));

  QObject::connect(m_database,
                   SIGNAL(userRemoveSignal(int)),
                   this,
                   SLOT(removeUser(int)));
}

void UserMgtWidget::setDatabase(QSqlDatabase db)
{
  m_database->set(db);
}

void UserMgtWidget::setUser(int id)
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
      m_database->refresh();
    }
    else
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("Erro '%1' ao remover a nota com ID '%2'.").arg(error, id),
                            QMessageBox::Ok);
    }
  }
}


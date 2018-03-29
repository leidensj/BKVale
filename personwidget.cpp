#include "personwidget.h"
#include "personview.h"
#include "jdatabase.h"
#include "databaseutils.h"
#include <QMessageBox>
#include <QLayout>
#include <QSplitter>

PersonWidget::PersonWidget(QWidget *parent)
  : QFrame(parent)
  , m_database(nullptr)
  , m_view(nullptr)
{
  m_database = new JDatabase;
  m_database->layout()->setContentsMargins(0, 0, 9, 0);
  m_view = new PersonView;
  m_view->layout()->setContentsMargins(9, 0, 0, 0);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(m_view);

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->addWidget(splitter);
  setLayout(mainlayout);

  QObject::connect(m_view,
                   SIGNAL(saveSignal()),
                   this,
                   SLOT(save()));

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));

  QObject::connect(m_database,
                   SIGNAL(itemRemovedSignal(qlonglong)),
                   this,
                   SLOT(itemRemoved(qlonglong)));
}

PersonWidget::~PersonWidget()
{

}

void PersonWidget::setDatabase(QSqlDatabase db)
{
  m_database->setDatabase(db, PERSON_SQL_TABLE_NAME);
  m_view->setDatabase(db);
}

void PersonWidget::itemSelected(const JItem& jItem)
{
  const Person& person = dynamic_cast<const Person&>(jItem);
  m_view->setPerson(person);
}

void PersonWidget::itemRemoved(qlonglong id)
{
  if (id == m_view->getPerson().m_id)
    m_view->create();
}

void PersonWidget::save()
{
  Person person;
  m_view->getPerson(person);
  if (m_database->save(person))
    m_view->create();
}

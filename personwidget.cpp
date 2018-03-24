#include "personwidget.h"
#include "personview.h"
#include "jdatabase.h"
#include "databaseutils.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QLayout>
#include <QSplitter>

/*void setColumnIcon(QSqlTableModel* model,
                   ItemTableIndex idx)
{
  if (model != nullptr)
  {
    model->setHeaderData((int)idx,
                         Qt::Horizontal,
                         QVariant::fromValue(QIcon(columnIcon(idx))),
                         Qt::DecorationRole);
  }
}*/

class PersonTableModel : public QSqlTableModel
{

public:
  PersonTableModel(QObject *parent, QSqlDatabase db)
   : QSqlTableModel(parent, db)
  {

  }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
  {
    if (!index.isValid())
      return QModelIndex();

    QVariant value = QSqlTableModel::data(index, role);
    if (role == Qt::DisplayRole)
    {
      //TODO
    }

    return value;
  }
};

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
  PersonTableModel* model = new PersonTableModel(m_database, db);
  m_database->set(model, PERSON_SQL_TABLE_NAME, Person::getColumns());
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

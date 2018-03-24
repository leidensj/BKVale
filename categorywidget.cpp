#include "categorywidget.h"
#include "categoryview.h"
#include "databaseutils.h"
#include "jdatabase.h"
#include "imagetablemodel.h"
#include <QSplitter>
#include <QLayout>
#include <QMessageBox>

CategoryWidget::CategoryWidget(QWidget* parent)
  : QFrame(parent)
  , m_view(nullptr)
  , m_database(nullptr)
{
  m_view = new CategoryView();
  m_view->layout()->setContentsMargins(9, 0, 0, 0);
  m_database = new JDatabase();
  m_database->setContentsMargins(0, 0, 9, 0);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(m_view);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setContentsMargins(0, 0, 0, 0);
  vlayout0->addWidget(splitter);
  setLayout(vlayout0);

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

  QObject::connect(m_view,
                   SIGNAL(searchImageSignal()),
                   this,
                   SLOT(searchImage()));
}

void CategoryWidget::setDatabase(QSqlDatabase db)
{
  QSqlTableModel* model = new QSqlTableModel(m_database, db);
  m_database->set(model, CATEGORY_SQL_TABLE_NAME, Category::getColumns());
}

void CategoryWidget::itemSelected(const JItem& jItem)
{
  const Category& category = dynamic_cast<const Category&>(jItem);
  if (category.isValidId())
    m_view->setCategory(category);
}

void CategoryWidget::itemRemoved(qlonglong id)
{
  if (m_view->getCategory().m_id == id)
    m_view->create();
}

void CategoryWidget::save()
{
  Category category = m_view->getCategory();
  if (m_database->save(category))
    m_view->create();
}

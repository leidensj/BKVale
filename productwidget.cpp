#include "productwidget.h"
#include "productview.h"
#include "jdatabase.h"
#include "databaseutils.h"
#include "imagetablemodel.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QLayout>
#include <QSplitter>

ProductWidget::ProductWidget(QWidget *parent)
  : QFrame(parent)
  , m_database(nullptr)
  , m_view(nullptr)
{
  m_database = new JDatabase();
  m_database->layout()->setContentsMargins(0, 0, 9, 0);
  m_view = new ProductView();
  m_view->layout()->setContentsMargins(9, 0, 0, 0);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(m_view);

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->addWidget(splitter);
  setLayout(vlayout);

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

ProductWidget::~ProductWidget()
{

}

void ProductWidget::setDatabase(QSqlDatabase db)
{
  QSqlTableModel* model = new QSqlTableModel(m_database, db);
  m_database->set(model, PRODUCT_SQL_TABLE_NAME, Product::getColumns());
  m_view->setDatabase(db);
}

void ProductWidget::itemSelected(const JItem& jItem)
{
  const Product& product = dynamic_cast<const Product&>(jItem);
  if (product.isValidId())
    m_view->setProduct(product);
}

void ProductWidget::itemRemoved(qlonglong id)
{
    if (id == m_view->getProduct().m_id)
      m_view->create();
}

void ProductWidget::save()
{
  Product product = m_view->getProduct();
  if (m_database->save(product))
    m_view->create();
}

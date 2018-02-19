#include "productwidget.h"
#include "productview.h"
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

class ProductTableModel : public QSqlTableModel
{

public:
  ProductTableModel(QObject *parent, QSqlDatabase db)
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
                   SLOT(saveProduct()));

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(int)),
                   this,
                   SLOT(productSelected(int)));

  QObject::connect(m_database,
                   SIGNAL(itemRemoveSignal(int)),
                   this,
                   SLOT(removeProduct(int)));

  QObject::connect(m_view,
                   SIGNAL(searchCategorySignal()),
                   this,
                   SLOT(searchCategory()));
}

ProductWidget::~ProductWidget()
{

}

void ProductWidget::setDatabase(QSqlDatabase db)
{
  ProductTableModel* model = new ProductTableModel(m_database, db);
  m_database->set(model, Product::getTableName(), Product::getColumns());
}

void ProductWidget::productSelected(int id)
{
  Product product;
  product.m_id = id;
  QString error;
  if (ProductSQL::select(m_database->get(), product, error))
  {
    Category category;
    category.m_id = product.m_categoryId;
    if (category.isValidId())
      CategorySQL::select(m_database->get(), category, error);
    m_view->setProduct(product, category.m_name);
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao abrir o produto com ID '%2'.").arg(error,
                                                                              QString::number(product.m_id)),
                          QMessageBox::Ok);
  }
}

void ProductWidget::removeProduct(int id)
{
  QString error;
  if (QMessageBox::question(this,
                            tr("Remover produto"),
                            tr("Tem certeza que deseja remover o produto selecionado?"),
                            QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
  {
    if (ProductSQL::remove(m_database->get(), id, error))
    {
      Product product = m_view->getProduct();
      if (id == product.m_id)
        m_view->create();
      m_database->refresh();
    }
    else
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("Erro '%1' ao remover o produto com ID '%2'.").arg(error,
                                                                                  QString::number(id)),
                            QMessageBox::Ok);
    }
  }
}

void ProductWidget::saveProduct()
{
  QString error;
  Product product = m_view->getProduct();

  if (product.isValidId()
      ? ProductSQL::update(m_database->get(), product, error)
      : ProductSQL::insert(m_database->get(), product, error))
  {
    m_database->refresh();
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao salvar produto.").arg(error),
                          QMessageBox::Ok);
  }
}

void ProductWidget::searchCategory()
{
  QSqlTableModel* model = new QSqlTableModel(0, m_database->get());
  JDatabaseSelector dlg(tr("Selecionar Categoria"),
                        QIcon(":/icons/res/category.png"),
                        INVALID_CATEGORY_ID);
  dlg.set(model, Category::getTableName(), Category::getColumns());
  dlg.exec();
  if (Category::st_isValidId(dlg.getCurrentId()))
  {
    Category category;
    category.m_id = dlg.getCurrentId();
    QString error;
    CategorySQL::select(m_database->get(), category, error);
    m_view->setCategory(category.m_id, category.m_name);
  }
}

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
  setLayout(vlayout);

  QObject::connect(m_view,
                   SIGNAL(saveSignal()),
                   this,
                   SLOT(savePerson()));

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(int)),
                   this,
                   SLOT(personSelected(int)));

  QObject::connect(m_database,
                   SIGNAL(itemRemoveSignal(int)),
                   this,
                   SLOT(removePerson(int)));

  QObject::connect(m_view,
                   SIGNAL(searchImageSignal()),
                   this,
                   SLOT(searchImage()));
}

PersonWidget::~PersonWidget()
{

}

void PersonWidget::setDatabase(QSqlDatabase db)
{
  PersonTableModel* model = new PersonTableModel(m_database, db);
  m_database->set(model, Person::getTableName(), Person::getColumns());
}

void PersonWidget::personSelected(int id)
{
  Person person;
  person.m_id = id;
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
                          tr("Erro '%1' ao abrir a pessoa com ID '%2'.").arg(error,
                                                                             QString::number(id)),
                          QMessageBox::Ok);
  }
}

void PersonWidget::removePerson(int id)
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

void PersonWidget::savePerson()
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

void PersonWidget::searchImage()
{
  QSqlTableModel* model = new QSqlTableModel(0, m_database->get());
  JDatabaseSelector dlg(tr("Selecionar Imagem"),
                        QIcon(":/icons/res/image.png"),
                        INVALID_IMAGE_ID);
  dlg.set(model, Image::getTableName(), Image::getColumns());
  dlg.exec();
  if (Image::st_isValidId(dlg.getCurrentId()))
  {
    Image image;
    image.m_id = dlg.getCurrentId();
    QString error;
    ImageSQL::select(m_database->get(), image, error);
    m_view->setImage(image.m_id, image.m_name, image.m_image);
  }
}

#include "itemwidget.h"
#include "itemview.h"
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

class ItemTableModel : public QSqlTableModel
{

public:
  ItemTableModel(QObject *parent, QSqlDatabase db)
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

ItemWidget::ItemWidget(QWidget *parent)
  : QFrame(parent)
  , m_database(nullptr)
  , m_view(nullptr)
{
  m_database = new JDatabase();
  m_view = new ItemView();

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(m_view);

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->addWidget(splitter);
  setLayout(vlayout);

  QObject::connect(m_view,
                   SIGNAL(saveSignal()),
                   this,
                   SLOT(saveItem()));

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(int)),
                   this,
                   SLOT(itemSelected(int)));

  QObject::connect(m_database,
                   SIGNAL(itemRemoveSignal(int)),
                   this,
                   SLOT(removeItem(int)));

  QObject::connect(m_view,
                   SIGNAL(searchCategorySignal()),
                   this,
                   SLOT(searchCategory()));
}

ItemWidget::~ItemWidget()
{

}

void ItemWidget::setDatabase(QSqlDatabase db)
{
  ItemTableModel* model = new ItemTableModel(m_database, db);
  QVector<SqlTableColumn> columns;
  columns.push_back(SqlTableColumn(true, false, "_ID", tr("Id"), QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(false, true, "_NAME", tr("Nome"), QHeaderView::ResizeMode::Stretch));
  columns.push_back(SqlTableColumn(true, false, "_CATEGORYID", tr("Categoria"), QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_IMAGEID", tr("Imagem"), QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(false, false, "_UNITY", tr("Unidade"), QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_PACKAGE_UNITY", tr("Unidade da Embalagem"), QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_PACKAGE_AMMOUNT", tr("Quantidade da Embalagem"), QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(false, false, "_DETAILS", tr("Detalhes"), QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_CODE", tr("Código"), QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_AVAILABLE_AT_NOTES", tr("Notas"), QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_AVAILABLE_AT_SHOP", tr("Compras"), QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_AVAILABLE_AT_CONSUMPTION", tr("Consumo"), QHeaderView::ResizeMode::ResizeToContents));
  m_database->set(model, "_ITEMS", columns);
}

void ItemWidget::itemSelected(int id)
{
  Item item;
  item.m_id = id;
  QString error;
  if (ItemSQL::select(m_database->get(), item, error))
  {
    Category category;
    category.m_id = item.m_categoryId;
    if (category.isValidId())
      CategorySQL::select(m_database->get(), category, error);
    m_view->setItem(item, category.m_name);
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao abrir o vale com ID '%2'.").arg(error, QString::number(item.m_id)),
                          QMessageBox::Ok);
  }
}

void ItemWidget::removeItem(int id)
{
  QString error;
  if (QMessageBox::question(this,
                            tr("Remover item"),
                            tr("Tem certeza que deseja remover o item selecionado?"),
                            QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
  {
    if (ItemSQL::remove(m_database->get(), id, error))
    {
      Item item = m_view->getItem();
      if (id == item.m_id)
        m_view->create();
      m_database->refresh();
    }
    else
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("Erro '%1' ao remover o item com ID '%2'.").arg(error,
                                                                               QString::number(id)),
                            QMessageBox::Ok);
    }
  }
}

void ItemWidget::saveItem()
{
  QString error;
  Item item = m_view->getItem();

  if (item.isValidId()
      ? ItemSQL::update(m_database->get(), item, error)
      : ItemSQL::insert(m_database->get(), item, error))
  {
    m_database->refresh();
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao salvar item.").arg(error),
                          QMessageBox::Ok);
  }
}

void ItemWidget::searchCategory()
{
  QVector<SqlTableColumn> columns;
  columns.push_back(SqlTableColumn(true, false, "_ID", "Id", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(false, true, "_NAME", "Nome", QHeaderView::ResizeMode::Stretch));
  columns.push_back(SqlTableColumn(true, false, "_IMAGEID", "Imagem", QHeaderView::ResizeMode::ResizeToContents));
  QSqlTableModel* model = new QSqlTableModel(0, m_database->get());
  JDatabaseSelector dlg(tr("Escolher Categoria"),
                        QIcon(":/icons/res/category.png"),
                        INVALID_CATEGORY_ID);
  dlg.set(model, "_CATEGORIES", columns);
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

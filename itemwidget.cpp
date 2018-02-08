#include "itemwidget.h"
#include "itemview.h"
#include "jdatabase.h"
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
      switch ((ItemTableIndex)index.column())
      {
        default:
          break;
      }
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
}

ItemWidget::~ItemWidget()
{

}

void ItemWidget::setDatabase(QSqlDatabase db)
{
  ItemTableModel* model = new ItemTableModel(m_database, db);
  QVector<SqlTableColumn> columns;
  columns.push_back(SqlTableColumn(true, false, "_ID", "Id", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(false, true, "_NAME", "Nome", QHeaderView::ResizeMode::Stretch));
  columns.push_back(SqlTableColumn(true, false, "_CATEGORYID", "Ícone", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(false, false, "_UNITY", "Ícone", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_PACKAGE_UNITY", "Ícone", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_PACKAGE_AMMOUNT", "Ícone", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(false, false, "_DETAILS", "Ícone", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_CODE", "Ícone", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_ICON", "Ícone", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_AVAILABLE_AT_NOTES", "Ícone", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_AVAILABLE_AT_SHOP", "Ícone", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(true, false, "_AVAILABLE_AT_CONSUMPTION", "Ícone", QHeaderView::ResizeMode::ResizeToContents));
  m_database->set(model, "_ITEMS", columns);
}

void ItemWidget::itemSelected(int id)
{

}

void ItemWidget::removeItem(int id)
{

}

void ItemWidget::saveItem()
{

}

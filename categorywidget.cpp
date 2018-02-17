#include "categorywidget.h"
#include "categoryview.h"
#include "databaseutils.h"
#include "jdatabase.h"
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
                   SIGNAL(itemSelectedSignal(int)),
                   this,
                   SLOT(categorySelected(int)));

  QObject::connect(m_database,
                   SIGNAL(itemRemoveSignal(int)),
                   this,
                   SLOT(removeCategory(int)));

  QObject::connect(m_view,
                   SIGNAL(saveSignal()),
                   this,
                   SLOT(saveCategory()));

  QObject::connect(m_view,
                   SIGNAL(searchImageSignal()),
                   this,
                   SLOT(searchImage()));
}

void CategoryWidget::setDatabase(QSqlDatabase db)
{
  QSqlTableModel* model = new QSqlTableModel(m_database, db);
  QVector<SqlTableColumn> columns;
  columns.push_back(SqlTableColumn(true, false, "_ID", "Id", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(false, true, "_NAME", "Nome", QHeaderView::ResizeMode::Stretch));
  columns.push_back(SqlTableColumn(true, false, "_IMAGEID", "Imagem", QHeaderView::ResizeMode::ResizeToContents));
  m_database->set(model, "_CATEGORIES", columns);
}

void CategoryWidget::categorySelected(int id)
{
  QString error;
  Category category;
  category.m_id = id;
  if (CategorySQL::select(m_database->get(),
                          category,
                          error))
  {
    Image image;
    image.m_id = category.m_imageId;
    if (image.isValidId())
      ImageSQL::select(m_database->get(), image, error);

    m_view->setCategory(category, image.m_name, image.m_image);
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao abrir categoria com ID '%2'.").arg(error,
                                                                              QString::number(id)),
                          QMessageBox::Ok);
  }
}

void CategoryWidget::removeCategory(int id)
{
  QString error;
  if (!CategorySQL::remove(m_database->get(),
                          id,
                          error))
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao remover categoria com ID '%2'.").arg(error,
                                                                                QString::number(id)),
                          QMessageBox::Ok);
  }
  else
  {
    Category category = m_view->getCategory();
    if (category.m_id == id)
      m_view->create();
  }
}

void CategoryWidget::saveCategory()
{
  QString error;
  Category category = m_view->getCategory();
  bool bSuccess = category.isValidId()
                  ? CategorySQL::update(m_database->get(), category, error)
                  : CategorySQL::insert(m_database->get(), category, error);

  if (bSuccess)
  {
    m_view->create();
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao salvar categoria.").arg(error),
                          QMessageBox::Ok);
  }
}

void CategoryWidget::searchImage()
{
  QVector<SqlTableColumn> columns;
  columns.push_back(SqlTableColumn(true, false, "_ID", "Id", QHeaderView::ResizeMode::ResizeToContents));
  columns.push_back(SqlTableColumn(false, true, "_NAME", "Nome", QHeaderView::ResizeMode::Stretch));
  columns.push_back(SqlTableColumn(true, false, "_IMAGE", "Imagem", QHeaderView::ResizeMode::ResizeToContents));
  QSqlTableModel* model = new QSqlTableModel(0, m_database->get());
  JDatabaseSelector dlg(tr("Escolher Imagem"),
                        QIcon(":/icons/res/icon.png"),
                        INVALID_IMAGE_ID);
  dlg.set(model, "_IMAGES", columns);
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

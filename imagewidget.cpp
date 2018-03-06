#include "imagewidget.h"
#include "imageview.h"
#include "jdatabase.h"
#include "databaseutils.h"
#include "imagetablemodel.h"
#include <QLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QSqlTableModel>

ImageWidget::ImageWidget(QWidget* parent)
  : QFrame(parent)
  , m_view(nullptr)
  , m_database(nullptr)
{
  m_view = new ImageView();
  m_database = new JDatabase();

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(m_view);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setContentsMargins(0, 0, 0, 0);
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addWidget(splitter);
  setLayout(vlayout0);

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(int)),
                   this,
                   SLOT(imageSelected(int)));

  QObject::connect(m_database,
                   SIGNAL(itemRemoveSignal(int)),
                   this,
                   SLOT(removeImage(int)));

  QObject::connect(m_view,
                   SIGNAL(saveSignal()),
                   this,
                   SLOT(saveImage()));
}

void ImageWidget::setDatabase(QSqlDatabase db)
{
  ImageTableModel* model = new ImageTableModel(m_database, db);
  m_database->set(model, SQL_IMAGE_TABLE_NAME, Image::getColumns());
}

void ImageWidget::imageSelected(int id)
{
  Image image;
  image.m_id = id;
  QString error;
  if (ImageSQL::select(m_database->get(), image, error))
  {
    m_view->setImage(image);
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao abrir a imagem com ID '%2'.").arg(error, QString::number(image.m_id)),
                          QMessageBox::Ok);
  }
}

void ImageWidget::removeImage(int id)
{
  QString error;
  if (QMessageBox::question(this,
                            tr("Remover imagem"),
                            tr("Tem certeza que deseja remover a imagem selecionada?"),
                            QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
  {
    if (ImageSQL::remove(m_database->get(), id, error))
    {
      Image image = m_view->getImage();
      if (id == image.m_id)
        m_view->create();
      m_database->refresh();
    }
    else
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("Erro '%1' ao remover a imagem com ID '%2'.").arg(error, QString::number(id)),
                            QMessageBox::Ok);
    }
  }
}

void ImageWidget::saveImage()
{
  QString error;
  Image image = m_view->getImage();

  if (image.isValidId()
      ? ImageSQL::update(m_database->get(), image, error)
      : ImageSQL::insert(m_database->get(), image, error))
  {
    m_database->refresh();
    m_view->create();
  }
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao salvar imagem.").arg(error),
                          QMessageBox::Ok);
  }
}

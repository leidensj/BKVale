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
}

void ImageWidget::setDatabase(QSqlDatabase db)
{
  ImageTableModel* model = new ImageTableModel(m_database, db);
  m_database->set(model, IMAGE_SQL_TABLE_NAME, Image::getColumns());
}

void ImageWidget::itemSelected(const JItem& jItem)
{
  const Image& image = dynamic_cast<const Image&>(jItem);
  if (image.isValidId())
    m_view->setImage(image);
}

void ImageWidget::itemRemoved(qlonglong id)
{
  if (m_view->getImage().m_id == id)
    m_view->create();
}

void ImageWidget::save()
{
  Image image = m_view->getImage();
  if (m_database->save(image))
    m_view->create();
}

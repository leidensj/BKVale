#include "imageview.h"
#include "jlineedit.h"
#include "jimageview.h"
#include "jdatabase.h"
#include <QPushButton>
#include <QLayout>
#include <QTabWidget>
#include <QSplitter>

ImageView::ImageView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_edImageName(nullptr)
  , m_imageView(nullptr)
{
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

  m_btnSave = new QPushButton;
  m_btnSave->setFlat(true);
  m_btnSave->setText("");
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

  m_edImageName = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edImageName->setPlaceholderText(tr("Nome"));

  m_imageView = new JImageView(true);

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->setAlignment(Qt::AlignLeft);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSave);

  QVBoxLayout* tablayout = new QVBoxLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addWidget(m_edImageName);
  tablayout->addWidget(m_imageView);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  QTabWidget* tabWidget = new QTabWidget;
  tabWidget->addTab(tabframe,
                    QIcon(":/icons/res/icon.png"),
                    tr("Imagem"));

  QVBoxLayout* viewlayout = new QVBoxLayout;
  viewlayout->setContentsMargins(9, 0, 0, 0);
  viewlayout->addLayout(buttonlayout);
  viewlayout->addWidget(tabWidget);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewlayout);

  m_database = new JDatabase;

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(viewFrame);

  QVBoxLayout* mainLayout = new QVBoxLayout();
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
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

void ImageView::setDatabase(QSqlDatabase db)
{
  m_database->setDatabase(db, IMAGE_SQL_TABLE_NAME);
}

void ImageView::setImage(const Image& image)
{
  QString strIcon = image.isValidId()
                    ? ":/icons/res/saveas.png"
                    : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(strIcon));
  m_currentId = image.m_id;
  m_edImageName->clear();
  m_imageView->clearImage();
  m_edImageName->setText(image.m_name);
  m_imageView->setImage(image.m_image);
}

 Image ImageView::getImage() const
 {
   Image image;
   image.m_id = m_currentId;
   image.m_name = m_edImageName->text();
   image.m_image = m_imageView->getImage();
   return image;
 }

void ImageView::create()
{
  m_currentId = INVALID_ID;
  setImage(Image());
}

void ImageView::itemSelected(const JItem& jItem)
{
  const Image& image = dynamic_cast<const Image&>(jItem);
  if (image.isValidId())
    setImage(image);
}

void ImageView::itemRemoved(qlonglong id)
{
  if (m_currentId == id)
    create();
}

void ImageView::save()
{
  if (m_database->save(getImage()))
    create();
}

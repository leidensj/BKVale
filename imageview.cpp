#include "imageview.h"
#include "jlineedit.h"
#include "jimageview.h"
#include <QLayout>

ImageView::ImageView(QWidget* parent)
  : JItemView(IMAGE_SQL_TABLE_NAME, parent)
  , m_edImageName(nullptr)
  , m_imageView(nullptr)
{
  m_edImageName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                JLineEdit::st_defaultFlags1);
  m_edImageName->setPlaceholderText(tr("Nome"));

  m_imageView = new JImageView(true);

  QVBoxLayout* tablayout = new QVBoxLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addWidget(m_edImageName);
  tablayout->addWidget(m_imageView);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  m_tab->addTab(tabframe,
                QIcon(":/icons/res/icon.png"),
                tr("Imagem"));

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(selectItem(const JItem&)));
  QObject::connect(m_database,
                   SIGNAL(itemsRemovedSignal(const QVector<Id>&)),
                   this,
                   SLOT(itemsRemoved(const QVector<Id>&)));
}

void ImageView::setItem(const JItem& o)
{
  auto _o = dynamic_cast<const Image&>(o);
  m_currentId = _o.m_id;
  m_imageView->clearImage();
  m_edImageName->setText(_o.m_name);
  m_imageView->setImage(_o.m_image);
}

const JItem& ImageView::getItem() const
{
  static Image o;
  o.m_id = m_currentId;
  o.m_name = m_edImageName->text();
  o.m_image = m_imageView->getImage();
  return o;
}

void ImageView::create()
{
  selectItem(Image());
  m_edImageName->setFocus();
}

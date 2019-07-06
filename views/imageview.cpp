#include "imageview.h"
#include "widgets/jlineedit.h"
#include "widgets/jimageview.h"
#include <QLayout>

ImageView::ImageView(QWidget* parent)
  : JItemView(IMAGE_SQL_TABLE_NAME, parent)
  , m_edImageName(nullptr)
  , m_imageView(nullptr)
{
  m_edImageName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
  m_edImageName->setPlaceholderText(tr("Nome"));
  m_imageView = new JImageView(true);

  QVBoxLayout* tablayout = new QVBoxLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addWidget(m_edImageName);
  tablayout->addWidget(m_imageView);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  m_tab->addTab(tabframe, QIcon(":/icons/res/icon.png"), tr("Imagem"));
  setFocusWidgetOnCreate(m_edImageName);
}

void ImageView::setItem(const JItemSQL& o)
{
  const Image& _o = dynamic_cast<const Image&>(o);
  m_imageView->clearImage();
  m_edImageName->setText(_o.m_name);
  m_imageView->setImage(_o.m_image);
}

void ImageView::getItem(JItemSQL& o) const
{
  Image& _o = dynamic_cast<Image&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_name = m_edImageName->text();
  _o.m_image = m_imageView->getImage();
}

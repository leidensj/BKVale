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
}

void ImageView::setItem(const JItemSQL& o)
{
  m_ref = dynamic_cast<const Image&>(o);
  m_imageView->clearImage();
  m_edImageName->setText(m_ref.m_name);
  m_imageView->setImage(m_ref.m_image);
}

const JItemSQL& ImageView::getItem() const
{
  m_ref.clear(false);
  m_ref.m_name = m_edImageName->text();
  m_ref.m_image = m_imageView->getImage();
  return m_ref;
}

void ImageView::create()
{
  selectItem(Image());
  m_edImageName->setFocus();
}

Id ImageView::getId() const
{
  return m_ref.m_id;
}

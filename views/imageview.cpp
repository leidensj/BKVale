#include "imageview.h"
#include "widgets/jlineedit.h"
#include "controls/imageviewer.h"
#include <QLayout>

ImageView::ImageView(QWidget* parent)
  : JItemView(IMAGE_SQL_TABLE_NAME, parent)
  , m_edImageName(nullptr)
  , m_viewer(nullptr)
{
  m_edImageName = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edImageName->setPlaceholderText(tr("Nome"));
  m_viewer = new ImageViewer;
  m_viewer->setContentsMargins(0, 0, 0, 0);

  QVBoxLayout* tablayout = new QVBoxLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addWidget(m_edImageName);
  tablayout->addWidget(m_viewer);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  m_tab->addTab(tabframe, QIcon(":/icons/res/icon.png"), tr("Imagem"));
  setFocusWidgetOnClear(m_edImageName);
}

void ImageView::setItem(const JItemSQL& o)
{
  const Image& _o = dynamic_cast<const Image&>(o);
  m_edImageName->setText(_o.m_name);
  m_viewer->setImage(QImage::fromData(o.image()));
}

void ImageView::getItem(JItemSQL& o) const
{
  Image& _o = dynamic_cast<Image&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_name = m_edImageName->text();
  _o.m_image = m_viewer->getCompressedImageAsByteArray();
}

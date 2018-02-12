#include "imagewidget.h"
#include "imageview.h"
#include <QLayout>

ImageWidget::ImageWidget(QWidget* parent)
  : QFrame(parent)
  , m_view(nullptr)
{
  m_view = new ImageView();
  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setContentsMargins(0, 0, 0, 0);
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addWidget(m_view);
  setLayout(vlayout0);
}

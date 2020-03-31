#include "jclicklabel.h"

JClickLabel::JClickLabel(QWidget* parent)
  : QLabel(parent)
{

}

void JClickLabel::mousePressEvent(QMouseEvent* /*event*/)
{
  emit clicked();
}

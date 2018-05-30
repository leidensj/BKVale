#include "jspinbox.h"

JSpinBox::JSpinBox(bool enterAsTab,
                   QWidget* parent)
  : QSpinBox(parent)
  , m_enterAsTab(enterAsTab)
{
}

void JSpinBox::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    emit enterSignal();
    if (m_enterAsTab)
      focusNextChild();
  }
  else if (event->key() == Qt::Key_Down)
    focusNextChild();
  else if (event->key() == Qt::Key_Up)
    focusPreviousChild();
  else
    QSpinBox::keyPressEvent(event);
}

void JSpinBox::setValueBlockingSignals(int value)
{
  blockSignals(true);
  setValue(value);
  blockSignals(false);
}

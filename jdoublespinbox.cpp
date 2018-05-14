#include "jdoublespinbox.h"

JDoubleSpinBox::JDoubleSpinBox(bool enterAsTab,
                               QWidget* parent)
  : QDoubleSpinBox(parent)
  , m_enterAsTab(enterAsTab)
{
}

void JDoubleSpinBox::keyPressEvent(QKeyEvent *event)
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
    QDoubleSpinBox::keyPressEvent(event);
}

void JDoubleSpinBox::setValueBlockingSignals(double value)
{
  blockSignals(true);
  setValue(value);
  blockSignals(false);
}

#include "jtimeedit.h"
#include <QKeyEvent>

JTimeEdit::JTimeEdit(int flags, QWidget* parent)
 : QTimeEdit(parent)
 , m_flags(flags)
{
  setDisplayFormat("HH:mm:ss");
}

void JTimeEdit::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    if (m_flags & (int)Flags::EnterAsTab)
      focusNextChild();
    emit enterSignal();
  }
  else if (event->key() == Qt::Key_Down)
  {
    if (m_flags & (int)Flags::ArrowsAsTab)
      focusNextChild();
    emit keyDownSignal();
  }
  else if (event->key() == Qt::Key_Up)
  {
    if (m_flags && (int)Flags::ArrowsAsTab)
      focusPreviousChild();
    emit keyUpSignal();
  }
  else
    QTimeEdit::keyPressEvent(event);
}

void JTimeEdit::focusInEvent(QFocusEvent *event)
{
  QTimeEdit::focusInEvent(event);
  emit focusSignal();
}

void JTimeEdit::mousePressEvent(QMouseEvent *event)
{
  QTimeEdit::mousePressEvent(event);
  emit mousePressedSignal();
}

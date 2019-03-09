#include "jplaintextedit.h"

JPlainTextEdit::JPlainTextEdit(bool enterAsTab,
                               QWidget* parent)
  : QPlainTextEdit(parent)
  , m_enterAsTab(enterAsTab)
{
}

void JPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    if (event->modifiers() & Qt::ShiftModifier)
      QPlainTextEdit::keyPressEvent(event);
    else
    {
      emit enterSignal();
      if (m_enterAsTab)
        focusNextChild();
    }
  }
  else
    QPlainTextEdit::keyPressEvent(event);
}

void JPlainTextEdit::setTextBlockingSignals(const QString& str)
{
  blockSignals(true);
  setPlainText(str);
  blockSignals(false);
}

#include "jlineedit.h"

JLineEdit::JLineEdit(bool toUpper,
                     bool enterAsTab,
                     QWidget* parent)
  : QLineEdit(parent)
  , m_enterAsTab(enterAsTab)
{
  if (toUpper)
  {
    connect(this,
            SIGNAL(textChanged(const QString&)),
            this,
            SLOT(toUpper()));
  }
}

void JLineEdit::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    emit enterSignal();
    if (m_enterAsTab)
    {
      focusNextChild();
    }
  }
  else
    QLineEdit::keyPressEvent(event);
}

void JLineEdit::toUpper()
{
  blockSignals(true);
  setText(text().toUpper());
  blockSignals(false);
}

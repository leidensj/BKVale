#include "jdateedit.h"


JDateEdit::JDateEdit(QWidget* parent)
  : QDateEdit(parent)
{
  setDisplayFormat("dd/MM/yyyy");
}

void JDateEdit::keyPressEvent(QKeyEvent *event)
{
  switch (event->key())
  {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
      focusNextChild();
      emit enterSignal();
    } break;
    case Qt::Key_Down:
    {
      focusNextChild();
      emit keyDownSignal();
    } break;
    case Qt::Key_Up:
    {
      focusPreviousChild();
      emit keyUpSignal();
    } break;
    case Qt::Key_Delete:
    {
      emit deleteSignal();
      QDateEdit::keyPressEvent(event);
    } break;
    default:
      QDateEdit::keyPressEvent(event);
  }
}

#include "jlineedit.h"

namespace
{
QString getRegEx(JLineEdit::Input input)
{
  switch (input)
  {
    case JLineEdit::Input::Alphanumeric:
      return "^[a-zA-Z0-9]*$";
    case JLineEdit::Input::AlphanumericAndSpaces:
      return "^[a-zA-Z0-9 ]*$";
    case JLineEdit::Input::Numeric:
      return "^[0-9]*$";
    case JLineEdit::Input::BasicMath:
      return "^[0-9/\\-\\*\\+\\.][^,]*$";
    case JLineEdit::Input::All:
    default:
      return "";
  }
}
}

JRegExpValidator::JRegExpValidator(bool toUpper, const QRegExp & rx,QObject* parent)
  : QRegExpValidator(rx, parent)
  , m_toUpper(toUpper)
{

}

QValidator::State JRegExpValidator::validate(QString& input, int& pos) const
{
  if (m_toUpper)
    input = input.toUpper();
  return QRegExpValidator::validate(input, pos);
}

JLineEdit::JLineEdit(Input input,
                     int flags,
                     QWidget* parent)
  : QLineEdit(parent)
  , m_flags(flags)
{
  if (input != Input::All)
    setValidator(new JRegExpValidator(flags & (int)Flags::ToUpper,
                                      QRegExp(getRegEx(input)), this));
}

void JLineEdit::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    emit enterSignal();
    if (m_flags & (int)Flags::EnterAsTab)
      focusNextChild();
  }
  else if (event->key() == Qt::Key_Down)
  {
    emit keyDownSignal();
    if (m_flags & (int)Flags::ArrowsAsTab)
      focusNextChild();
  }
  else if (event->key() == Qt::Key_Up)
  {
    emit keyUpSignal();
    if (m_flags && (int)Flags::ArrowsAsTab)
      focusPreviousChild();
  }
  else
    QLineEdit::keyPressEvent(event);
}

void JLineEdit::setTextBlockingSignals(const QString& str)
{
  blockSignals(true);
  setText(str);
  blockSignals(false);
}

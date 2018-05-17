#include "jlineedit.h"

namespace
{
QString getRegEx(JValidatorType validator)
{
  switch (validator)
  {
    case JValidatorType::Alphanumeric:
      return "^[a-zA-Z0-9]*$";
    case JValidatorType::AlphanumericAndSpaces:
      return "^[a-zA-Z0-9 ]*$";
    case JValidatorType::Numeric:
      return "^[0-9]*$";
    case JValidatorType::All:
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

JLineEdit::JLineEdit(JValidatorType validator,
                     bool toUpper,
                     bool keysAsTab,
                     QWidget* parent)
  : QLineEdit(parent)
  , m_keysAsTab(keysAsTab)
{
  if (validator != JValidatorType::All)
    setValidator(new JRegExpValidator(toUpper, QRegExp(getRegEx(validator)), this));
}

void JLineEdit::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    emit enterSignal();
    if (m_keysAsTab)
      focusNextChild();
  }
  else if (event->key() == Qt::Key_Down)
  {
    emit keyDownSignal();
    if (m_keysAsTab)
      focusNextChild();
  }
  else if (event->key() == Qt::Key_Up)
  {
    emit keyUpSignal();
    if (m_keysAsTab)
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

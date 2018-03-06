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
                     bool enterAsTab,
                     QWidget* parent)
  : QLineEdit(parent)
  , m_enterAsTab(enterAsTab)
{
  if (validator != JValidatorType::All)
    setValidator(new JRegExpValidator(toUpper, QRegExp(getRegEx(validator)), this));
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

void JLineEdit::setTextBlockingSignals(const QString& str)
{
  blockSignals(true);
  setText(str);
  blockSignals(false);
}

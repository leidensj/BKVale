#include "jregexpvalidator.h"

JRegExpValidator::JRegExpValidator(bool toUpper, const QRegularExpression & rx,QObject* parent)
  : QRegularExpressionValidator(rx, parent)
  , m_toUpper(toUpper)
{

}

QValidator::State JRegExpValidator::validate(QString& input, int& pos) const
{
  if (m_toUpper)
    input = input.toUpper();
  return QRegularExpressionValidator::validate(input, pos);
}

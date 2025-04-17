#ifndef JVALIDATOR_H
#define JVALIDATOR_H

#include <QRegularExpressionValidator>

class JRegExpValidator: public QRegularExpressionValidator
{
public:
  JRegExpValidator(bool toUpper, const QRegularExpression & rx, QObject* parent = 0);
  State validate(QString& input, int& pos) const override;

private:
  const bool m_toUpper;
};

#endif // JVALIDATOR_H

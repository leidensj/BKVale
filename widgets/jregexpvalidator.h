#ifndef JVALIDATOR_H
#define JVALIDATOR_H

#include <QRegExpValidator>

class JRegExpValidator: public QRegExpValidator
{
public:
  JRegExpValidator(bool toUpper, const QRegExp & rx, QObject* parent = 0);
  State validate(QString& input, int& pos) const override;

private:
  const bool m_toUpper;
};

#endif // JVALIDATOR_H

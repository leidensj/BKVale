#ifndef JLINEEDIT_H
#define JLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QValidator>
#include <QRegExpValidator>

enum class JValidatorType : int
{
  All,
  Alphanumeric,
  AlphanumericAndSpaces,
  Numeric,
  Email
};

class JRegExpValidator: public QRegExpValidator
{
public:
  JRegExpValidator(bool toUpper, const QRegExp & rx, QObject* parent = 0);
  State validate(QString& input, int& pos) const override;

private:
  const bool m_toUpper;
};

class JLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  explicit JLineEdit(JValidatorType validator,
                     bool toUpper,
                     bool enterAsTab,
                     QWidget* parent = 0);

  void setTextBlockingSignals(const QString& str);

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  const bool m_enterAsTab;

signals:
  void enterSignal();
};

#endif // JLINEEDIT_H

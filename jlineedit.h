#ifndef JLINEEDIT_H
#define JLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QValidator>
#include <QRegExpValidator>
#include "jitem.h"

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
  enum class Flags
  {
    ToUpper = 1 << 0,
    EnterAsTab = 1 << 1,
    ArrowsAsTab = 1 << 2,
  };

  enum class Input
  {
    All = 1 << 0,
    AlphanumericAndSpaces = 1 << 1,
    Alphanumeric = 1 << 2,
    Numeric = 1 << 3,
    BasicMath = 1 << 4
  };

  static const int st_defaultFlags1 = (int)Flags::ToUpper |
                                      (int)Flags::EnterAsTab |
                                      (int)Flags::ArrowsAsTab;
  static const int st_defaultFlags2 = (int)Flags::EnterAsTab |
                                      (int)Flags::ArrowsAsTab;

  explicit JLineEdit(Input input,
                     int flags = st_defaultFlags2,
                     QWidget* parent = nullptr);

  void setTextBlockingSignals(const QString& str);

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  const int m_flags;

signals:
  void enterSignal();
  void keyDownSignal();
  void keyUpSignal();
  void deleteSignal();
};

class JExpLineEdit : public JLineEdit
{
  Q_OBJECT

public:
  JExpLineEdit(JItem::DataType type = JItem::DataType::Fmt,
               int flags = st_defaultFlags1,
               double defaultValue = 0.0,
               QWidget* parent = nullptr);

  double getValue() const;
  void setText(const QString& text);
  void setText(double val);
  QString text() const;

private slots:
  void evaluate();

private:
  const JItem::DataType m_dataType;
  const double m_defaultValue;
  double m_currentValue;
};

#endif // JLINEEDIT_H

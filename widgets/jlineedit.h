#ifndef JLINEEDIT_H
#define JLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QValidator>
#include "items/jitem.h"

class JLineEdit : public QLineEdit
{
  Q_OBJECT

public:
  explicit JLineEdit(Text::Input input, bool bToUpper, QWidget* parent = nullptr);
  void setTextBlockingSignals(const QString& str);
  void setArrowsAndEnterAsTab(bool b);

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  const bool m_bToUpper;
  bool m_bArrowsAndEnterAsTab;

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
  JExpLineEdit(Data::Type type, bool bToUpper, double defaultValue = 0.0, QWidget* parent = nullptr);
  double getValue() const;
  void setText(const QString& text);
  void setText(double val);
  QString text() const;

private slots:
  void evaluate();

signals:
  void valueChanged(double);

private:
  const Data::Type m_dataType;
  const double m_defaultValue;
  double m_currentValue;
};

#endif // JLINEEDIT_H

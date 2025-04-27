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
  JExpLineEdit(Data::Type type, QWidget* parent = nullptr);
  double value() const;
  QString strValue() const;
  void setValue(const QString& value);
  void setValue(double value);
  void setMinimum(double value);
  void setMaximum(double value);
  void setInvertColors(bool b);

private slots:
  void evaluate(const QString& value);

signals:
  void valueChanged(double);

private:
  const Data::Type m_dataType;
  QString m_currentText;
  double m_currentValue;
  double m_minimumValue;
  double m_maximumValue;
  bool m_bInvertColors;
};

#endif // JLINEEDIT_H

#ifndef CALCULATORWIDGET_H
#define CALCULATORWIDGET_H

#include <QFrame>
#include <QPushButton>
#include "calculator.h"

class QLineEdit;

class CalculatorPushButton : public QPushButton
{
  Q_OBJECT

public:
  explicit CalculatorPushButton(Calculator::Button button, QWidget* parent = 0);

private slots:
  void emitCalculatorButtonClickedSignal();

signals:
  calculatorButtonClickedSignal(Calculator::Button button);

private:
  const Calculator::Button m_button;
};

class CalculatorWidget : public QFrame
{
  Q_OBJECT
public:
  explicit CalculatorWidget(QWidget* parent = 0);

private slots:
  void calculatorButtonClicked(Calculator::Button button);

private:
  QPushButton* m_btn0;
  QPushButton* m_btn1;
  QPushButton* m_btn2;
  QPushButton* m_btn3;
  QPushButton* m_btn4;
  QPushButton* m_btn5;
  QPushButton* m_btn6;
  QPushButton* m_btn7;
  QPushButton* m_btn8;
  QPushButton* m_btn9;
  QPushButton* m_btnDec;
  QPushButton* m_btnEq;
  QPushButton* m_btnPlus;
  QPushButton* m_btnMin;
  QPushButton* m_btnMul;
  QPushButton* m_btnDiv;
  QString m_display;
  double m_value;
  Calculator::Button m_op;
};

#endif // CALCULATORWIDGET_H

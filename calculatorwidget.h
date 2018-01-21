#ifndef CALCULATORWIDGET_H
#define CALCULATORWIDGET_H

#include <QFrame>
#include <QPushButton>
#include "calculator.h"

class QLineEdit;
class QPlainTextEdit;
class QRadioButton;


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
  QString text() const;

private slots:
  void calculatorButtonClicked(Calculator::Button button);
  void emitPrintSignal(double value, Calculator::Button button);
  void clear();
  void reset();

signals:
  printSignal(const QString& text);
  printFullCutSignal();

private:
  QPushButton* m_btnPrint;
  QRadioButton* m_rdoAlignLeft;
  QRadioButton* m_rdoAlignCenter;
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
  QPushButton* m_btnClr;
  QPushButton* m_btnCls;
  QLineEdit* m_edDisplay;
  QPlainTextEdit* m_view;
  QString m_display;
  double m_total;
  double m_lastValue;
  Calculator::Button m_lastButton;
  double calculate(double op1, double op2, Calculator::Button button);
};

#endif // CALCULATORWIDGET_H

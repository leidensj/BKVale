#ifndef CALCULATORWIDGET_H
#define CALCULATORWIDGET_H

#include <QFrame>
#include <QPushButton>

class QLineEdit;
class QPlainTextEdit;

class CalculatorWidget : public QFrame
{
  Q_OBJECT

public:
  explicit CalculatorWidget(QWidget* parent = nullptr);

private slots:
  void buttonClicked();
  void clear();
  void reset();
  void display(double value, int button);

private:
  QPushButton* m_btnPrint;
  QPushButton* m_btnAlignCenter;
  QPushButton* m_btnFontSize;
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
  int m_lastButton;
  double calculate(double op1, double op2, int button);
};

#endif // CALCULATORWIDGET_H

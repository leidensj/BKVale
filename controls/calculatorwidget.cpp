#include "calculatorwidget.h"
#include <QLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QScrollBar>
#include <QRadioButton>
#include <QSplitter>
#include <QMessageBox>
#include <QAction>
#include "escpos.h"
#include "printer.h"

#define KEY_CODE "KEY_CODE"

namespace Calculator
{
inline bool isOP(int button)
{
  switch(button)
  {
    case Qt::Key_Plus:
    case Qt::Key_Minus:
    case Qt::Key_Asterisk:
    case Qt::Key_Slash:
      return true;
    default:
      return false;
  }
}

inline bool isDigit(int button)
{
  switch(button)
  {
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
      return true;
    default:
      return false;
  }
}

inline bool isEqual(int button)
{
  return button == Qt::Key_Space;
}

inline bool isDecimal(int button)
{
  return button == Qt::Key_Period;
}

inline QString toStr(int button)
{
  switch(button)
  {
    case Qt::Key_0: return "0";
    case Qt::Key_1: return "1";
    case Qt::Key_2: return "2";
    case Qt::Key_3: return "3";
    case Qt::Key_4: return "4";
    case Qt::Key_5: return "5";
    case Qt::Key_6: return "6";
    case Qt::Key_7: return "7";
    case Qt::Key_8: return "8";
    case Qt::Key_9: return "9";
    case Qt::Key_Plus: return "+";
    case Qt::Key_Minus: return "-";
    case Qt::Key_Slash: return "/";
    case Qt::Key_Asterisk: return "*";
    case Qt::Key_Space: return "=";
    case Qt::Key_Period: return ".";
    default: return " ";
  }
}

inline void removeDecimal(QString& strValue)
{
  if (strValue.contains(Calculator::toStr(Qt::Key_Period)))
    strValue.remove(QRegularExpression("^[0]*"));
  strValue.replace(Calculator::toStr(Qt::Key_Period), "");
}
}

CalculatorWidget::CalculatorWidget(QWidget* parent)
  : QFrame(parent)
  , m_btn0(nullptr)
  , m_btn1(nullptr)
  , m_btn2(nullptr)
  , m_btn3(nullptr)
  , m_btn4(nullptr)
  , m_btn5(nullptr)
  , m_btn6(nullptr)
  , m_btn7(nullptr)
  , m_btn8(nullptr)
  , m_btn9(nullptr)
  , m_btnDec(nullptr)
  , m_btnEq(nullptr)
  , m_btnPlus(nullptr)
  , m_btnMin(nullptr)
  , m_btnMul(nullptr)
  , m_btnDiv(nullptr)
  , m_btnClr(nullptr)
  , m_btnCls(nullptr)
  , m_edDisplay(nullptr)
  , m_view(nullptr)
  , m_total(0.0)
  , m_lastValue(0.0)
  , m_lastButton(Qt::Key_Standby)
{
  m_btnCls = new QPushButton;
  m_btnCls->setFlat(true);
  m_btnCls->setText("");
  m_btnCls->setIconSize(QSize(64, 64));
  m_btnCls->setIcon(QIcon(":/icons/res/calccls.png"));
  m_btnCls->setShortcut(QKeySequence(Qt::Key_Escape));
  m_btnCls->setProperty(KEY_CODE, Qt::Key_Escape);

  m_btnClr = new QPushButton;
  m_btnClr->setFlat(true);
  m_btnClr->setText("");
  m_btnClr->setIconSize(QSize(64, 64));
  m_btnClr->setIcon(QIcon(":/icons/res/calcclr.png"));
  m_btnClr->setShortcut(QKeySequence(Qt::Key_Backspace));
  m_btnClr->setProperty(KEY_CODE, Qt::Key_Backspace);

  QHBoxLayout* hline0 = new QHBoxLayout();
  hline0->addWidget(m_btnCls);
  hline0->addWidget(m_btnClr);
  hline0->setAlignment(Qt::AlignLeft);
  hline0->setContentsMargins(0, 0, 0, 0);

  m_btn7 = new QPushButton;
  m_btn7->setFlat(true);
  m_btn7->setText("");
  m_btn7->setIconSize(QSize(64, 64));
  m_btn7->setIcon(QIcon(":/icons/res/calc7.png"));
  m_btn7->setShortcut(QKeySequence(Qt::Key_7));
  m_btn7->setProperty(KEY_CODE, Qt::Key_7);

  m_btn8 = new QPushButton;
  m_btn8->setFlat(true);
  m_btn8->setText("");
  m_btn8->setIconSize(QSize(64, 64));
  m_btn8->setIcon(QIcon(":/icons/res/calc8.png"));
  m_btn8->setShortcut(QKeySequence(Qt::Key_8));
  m_btn8->setProperty(KEY_CODE, Qt::Key_8);

  m_btn9 = new QPushButton;
  m_btn9->setFlat(true);
  m_btn9->setText("");
  m_btn9->setIconSize(QSize(64, 64));
  m_btn9->setIcon(QIcon(":/icons/res/calc9.png"));
  m_btn9->setShortcut(QKeySequence(Qt::Key_9));
  m_btn9->setProperty(KEY_CODE, Qt::Key_9);

  m_btnMul = new QPushButton;
  m_btnMul->setFlat(true);
  m_btnMul->setText("");
  m_btnMul->setIconSize(QSize(64, 64));
  m_btnMul->setIcon(QIcon(":/icons/res/calcmul.png"));
  m_btnMul->setShortcut(QKeySequence(Qt::Key_Asterisk));
  m_btnMul->setProperty(KEY_CODE, Qt::Key_Asterisk);

  QHBoxLayout* hline1 = new QHBoxLayout;
  hline1->addWidget(m_btn7);
  hline1->addWidget(m_btn8);
  hline1->addWidget(m_btn9);
  hline1->addWidget(m_btnMul);
  hline1->setAlignment(Qt::AlignLeft);
  hline1->setContentsMargins(0, 0, 0, 0);

  m_btn4 = new QPushButton;
  m_btn4->setFlat(true);
  m_btn4->setText("");
  m_btn4->setIconSize(QSize(64, 64));
  m_btn4->setIcon(QIcon(":/icons/res/calc4.png"));
  m_btn4->setShortcut(QKeySequence(Qt::Key_4));
  m_btn4->setProperty(KEY_CODE, Qt::Key_4);

  m_btn5 = new QPushButton;
  m_btn5->setFlat(true);
  m_btn5->setText("");
  m_btn5->setIconSize(QSize(64, 64));
  m_btn5->setIcon(QIcon(":/icons/res/calc5.png"));
  m_btn5->setShortcut(QKeySequence(Qt::Key_5));
  m_btn5->setProperty(KEY_CODE, Qt::Key_5);

  m_btn6 = new QPushButton;
  m_btn6->setFlat(true);
  m_btn6->setText("");
  m_btn6->setIconSize(QSize(64, 64));
  m_btn6->setIcon(QIcon(":/icons/res/calc6.png"));
  m_btn6->setShortcut(QKeySequence(Qt::Key_6));
  m_btn6->setProperty(KEY_CODE, Qt::Key_6);

  m_btnDiv = new QPushButton;
  m_btnDiv->setFlat(true);
  m_btnDiv->setText("");
  m_btnDiv->setIconSize(QSize(64, 64));
  m_btnDiv->setIcon(QIcon(":/icons/res/calcdiv.png"));
  m_btnDiv->setShortcut(QKeySequence(Qt::Key_Slash));
  m_btnDiv->setProperty(KEY_CODE, Qt::Key_Slash);

  QHBoxLayout* hline2 = new QHBoxLayout;
  hline2->addWidget(m_btn4);
  hline2->addWidget(m_btn5);
  hline2->addWidget(m_btn6);
  hline2->addWidget(m_btnDiv);
  hline2->setAlignment(Qt::AlignLeft);
  hline2->setContentsMargins(0, 0, 0, 0);

  m_btn1 = new QPushButton;
  m_btn1->setFlat(true);
  m_btn1->setText("");
  m_btn1->setIconSize(QSize(64, 64));
  m_btn1->setIcon(QIcon(":/icons/res/calc1.png"));
  m_btn1->setShortcut(QKeySequence(Qt::Key_1));
  m_btn1->setProperty(KEY_CODE, Qt::Key_1);

  m_btn2 = new QPushButton;
  m_btn2->setFlat(true);
  m_btn2->setText("");
  m_btn2->setIconSize(QSize(64, 64));
  m_btn2->setIcon(QIcon(":/icons/res/calc2.png"));
  m_btn2->setShortcut(QKeySequence(Qt::Key_2));
  m_btn2->setProperty(KEY_CODE, Qt::Key_2);

  m_btn3 = new QPushButton;
  m_btn3->setFlat(true);
  m_btn3->setText("");
  m_btn3->setIconSize(QSize(64, 64));
  m_btn3->setIcon(QIcon(":/icons/res/calc3.png"));
  m_btn3->setShortcut(QKeySequence(Qt::Key_3));
  m_btn3->setProperty(KEY_CODE, Qt::Key_3);

  m_btnMin = new QPushButton;
  m_btnMin->setFlat(true);
  m_btnMin->setText("");
  m_btnMin->setIconSize(QSize(64, 64));
  m_btnMin->setIcon(QIcon(":/icons/res/calcmin.png"));
  m_btnMin->setShortcut(QKeySequence(Qt::Key_Minus));
  m_btnMin->setProperty(KEY_CODE, Qt::Key_Minus);

  QHBoxLayout* hline3 = new QHBoxLayout;
  hline3->addWidget(m_btn1);
  hline3->addWidget(m_btn2);
  hline3->addWidget(m_btn3);
  hline3->addWidget(m_btnMin);
  hline3->setAlignment(Qt::AlignLeft);
  hline3->setContentsMargins(0, 0, 0, 0);

  m_btn0 = new QPushButton;
  m_btn0->setFlat(true);
  m_btn0->setText("");
  m_btn0->setIconSize(QSize(64, 64));
  m_btn0->setIcon(QIcon(":/icons/res/calc0.png"));
  m_btn0->setShortcut(QKeySequence(Qt::Key_0));
  m_btn0->setProperty(KEY_CODE, Qt::Key_0);

  m_btnDec = new QPushButton;
  m_btnDec->setFlat(true);
  m_btnDec->setText("");
  m_btnDec->setIconSize(QSize(64, 64));
  m_btnDec->setIcon(QIcon(":/icons/res/calcdec.png"));
  m_btnDec->setShortcut(QKeySequence(Qt::Key_Period));
  m_btnDec->setProperty(KEY_CODE, Qt::Key_Period);

  m_btnEq = new QPushButton;
  m_btnEq->setFlat(true);
  m_btnEq->setText("");
  m_btnEq->setIconSize(QSize(64, 64));
  m_btnEq->setIcon(QIcon(":/icons/res/calcequal.png"));
  m_btnEq->setShortcut(QKeySequence(Qt::Key_Space));
  m_btnEq->setProperty(KEY_CODE, Qt::Key_Space);

  m_btnPlus = new QPushButton;
  m_btnPlus->setFlat(true);
  m_btnPlus->setText("");
  m_btnPlus->setIconSize(QSize(64, 64));
  m_btnPlus->setIcon(QIcon(":/icons/res/calcplus.png"));
  m_btnPlus->setShortcut(QKeySequence(Qt::Key_Plus));
  m_btnPlus->setProperty(KEY_CODE, Qt::Key_Plus);

  auto action = new QAction(this);
  action->setShortcuts({QKeySequence(Qt::Key_Return), QKeySequence(Qt::Key_Enter) });
  this->addAction(action);
  connect(action, SIGNAL(triggered(bool)), m_btnPlus, SLOT(animateClick()));

  QHBoxLayout* hline4 = new QHBoxLayout;
  hline4->addWidget(m_btn0);
  hline4->addWidget(m_btnDec);
  hline4->addWidget(m_btnEq);
  hline4->addWidget(m_btnPlus);
  hline4->setAlignment(Qt::AlignLeft);
  hline4->setContentsMargins(0, 0, 0, 0);

  m_edDisplay = new QLineEdit;
  m_edDisplay->setAlignment(Qt::AlignLeft);
  m_edDisplay->setReadOnly(true);
  QFont f = m_edDisplay->font();
  f.setPointSize(24);
  m_edDisplay->setFont(f);
  m_edDisplay->setAlignment(Qt::AlignRight);
  m_edDisplay->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

  m_btnPrint = new QPushButton;
  m_btnPrint->setText("");
  m_btnPrint->setIconSize(QSize(24, 24));
  m_btnPrint->setIcon(QIcon(":/icons/res/calcprint.png"));
  m_btnPrint->setCheckable(true);
  m_btnPrint->setFlat(true);
  m_btnPrint->setChecked(true);

  m_rdoAlignLeft = new QRadioButton;
  m_rdoAlignLeft->setText("");
  m_rdoAlignLeft->setIconSize(QSize(24, 24));
  m_rdoAlignLeft->setIcon(QIcon(":/icons/res/calcalignleft.png"));
  m_rdoAlignLeft->setChecked(true);

  m_rdoAlignCenter = new QRadioButton;
  m_rdoAlignCenter->setText("");
  m_rdoAlignCenter->setIconSize(QSize(24, 24));
  m_rdoAlignCenter->setIcon(QIcon(":/icons/res/calcaligncenter.png"));

  QHBoxLayout* hlineCmd = new QHBoxLayout;
  hlineCmd->addWidget(m_btnPrint);
  hlineCmd->addWidget(m_rdoAlignLeft);
  hlineCmd->addWidget(m_rdoAlignCenter);
  hlineCmd->setAlignment(Qt::AlignLeft);
  hlineCmd->setContentsMargins(0, 0, 0, 0);

  QVBoxLayout* vlayoutl = new QVBoxLayout;
  vlayoutl->addLayout(hlineCmd);
  vlayoutl->addWidget(m_edDisplay);
  vlayoutl->addLayout(hline0);
  vlayoutl->addLayout(hline1);
  vlayoutl->addLayout(hline2);
  vlayoutl->addLayout(hline3);
  vlayoutl->addLayout(hline4);
  vlayoutl->setAlignment(Qt::AlignTop);

  m_view = new QPlainTextEdit;
  m_view->setReadOnly(true);

  QHBoxLayout* hlayout = new QHBoxLayout;
  hlayout->addLayout(vlayoutl);
  hlayout->addWidget(m_view);

  setLayout(hlayout);
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

  connect(m_btn0, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btn1, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btn2, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btn3, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btn4, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btn5, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btn6, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btn7, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btn8, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btn9, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btnEq, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btnPlus, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btnMin, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btnDiv, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btnMul, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btnDec, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
  connect(m_btnClr, SIGNAL(clicked(bool)), this, SLOT(clear()));
  connect(m_btnCls, SIGNAL(clicked(bool)), this, SLOT(reset()));

  m_edDisplay->setText("0");
  m_view->setPlainText("0");
}

double CalculatorWidget::calculate(double op1, double op2, int button)
{
  if (!Calculator::isOP(button))
    return 0.0;

  switch (button)
  {
    case Qt::Key_Plus:
      return op1 + op2;
    case Qt::Key_Minus:
      return op1 - op2;
    case Qt::Key_Slash:
      return op2 ? op1 / op2 : 0.0;
    case Qt::Key_Asterisk:
      return op1 * op2;
    default:
      return 0.0;
  }
}

void CalculatorWidget::print(double value, int button)
{
  QString text = buildPrintContent(value, button);
  m_view->appendPlainText(text);
  m_view->verticalScrollBar()->setValue(m_view->verticalScrollBar()->maximum());
  if (m_btnPrint->isChecked())
  {
    QString text2 = m_rdoAlignLeft->isChecked() ? ESC_ALIGN_LEFT : ESC_ALIGN_CENTER;
    text2 += ESC_EXPAND_ON + text + ESC_LF ESC_EXPAND_OFF;
    Printer printer;
    QString error;
    printer.print(text2, error);
  }
}

QString CalculatorWidget::buildPrintContent(double value, int button)
{
  return Calculator::toStr(button) + " " + QString::number(value, 'f').remove(QRegularExpression("\\.?0*$"));
}

void CalculatorWidget::buttonClicked()
{
  int button = sender()->property(KEY_CODE).toInt();
  if (Calculator::isOP(button))
  {
    double currentValue = m_edDisplay->text().toDouble();
    double value = !Calculator::isOP(m_lastButton) &&
                   !Calculator::isEqual(m_lastButton)
                   ? currentValue : m_lastValue;
    m_total = calculate(m_total, value, button);
    print(value, button);
    m_edDisplay->setText(QString::number(m_total, 'f').remove(QRegularExpression("\\.?0*$")));
    m_lastValue = value;
  }
  else if (Calculator::isEqual(button))
  {
    print(m_total, button);
    m_edDisplay->setText(QString::number(m_total, 'f').
                         remove(QRegularExpression("\\.?0*$")));
  }
  else if (Calculator::isDigit(button))
  {
    if (!Calculator::isDigit(m_lastButton) &&
        !Calculator::isDecimal(m_lastButton))
      m_edDisplay->setText("");
    QString strValue = m_edDisplay->text();
    strValue.append(Calculator::toStr(button));
    m_edDisplay->setText(strValue);
  }
  else if (Calculator::isDecimal(button))
  {
    if (Calculator::isEqual(m_lastButton) ||
        Calculator::isOP(m_lastButton))
      m_edDisplay->setText("");
    QString strValue = m_edDisplay->text();
    Calculator::removeDecimal(strValue);
    strValue.append(Calculator::toStr(button));
    m_edDisplay->setText(strValue);
  }

  m_lastButton = button;
}

void CalculatorWidget::clear()
{
  m_edDisplay->setText("");
}

void CalculatorWidget::reset()
{
  m_edDisplay->setText("0");
  m_view->setPlainText("0");
  m_lastValue = 0.0;
  m_total = 0.0;
  if (m_btnPrint->isChecked())
  {
    QString error;
    Printer printer;
    printer.print(Printer::st_strFullCut(), error);
  }
}

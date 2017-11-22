#include "calculatorwidget.h"
#include <QLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QScrollBar>
#include <QRadioButton>
#include <QSplitter>
#include "printutils.h"
#include "escpos.h"

CalculatorPushButton::CalculatorPushButton(Calculator::Button button, QWidget* parent)
  : QPushButton(parent)
  , m_button(button)
{
  QObject::connect(this,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitCalculatorButtonClickedSignal()));
}

void CalculatorPushButton::emitCalculatorButtonClickedSignal()
{
  emit calculatorButtonClickedSignal(m_button);
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
  , m_currentValue(0.0)
  , m_lastButton(Calculator::Button::Nop)
{
  m_btnCls = new QPushButton();
  m_btnCls->setFlat(true);
  m_btnCls->setText("");
  m_btnCls->setIconSize(QSize(64, 64));
  m_btnCls->setIcon(QIcon(":/icons/res/calccls.png"));
  m_btnCls->setShortcut(QKeySequence(Qt::Key_Escape));

  m_edDisplay = new QLineEdit();
  m_edDisplay->setAlignment(Qt::AlignLeft);
  m_edDisplay->setReadOnly(true);
  m_edDisplay->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  QFont f = m_edDisplay->font();
  f.setPointSize(24);
  m_edDisplay->setFont(f);

  m_btnClr = new QPushButton();
  m_btnClr->setFlat(true);
  m_btnClr->setText("");
  m_btnClr->setIconSize(QSize(64, 64));
  m_btnClr->setIcon(QIcon(":/icons/res/calcclr.png"));
  m_btnClr->setShortcut(QKeySequence(Qt::Key_Backspace));

  QHBoxLayout* hline0 = new QHBoxLayout();
  hline0->addWidget(m_btnCls);
  hline0->addWidget(m_edDisplay);
  hline0->addWidget(m_btnClr);
  hline0->setAlignment(Qt::AlignCenter);
  hline0->setContentsMargins(0, 0, 0, 0);

  m_btn7 = new CalculatorPushButton(Calculator::Button::Num7);
  m_btn7->setFlat(true);
  m_btn7->setText("");
  m_btn7->setIconSize(QSize(64, 64));
  m_btn7->setIcon(QIcon(":/icons/res/calc7.png"));
  m_btn7->setShortcut(QKeySequence(Qt::Key_7));

  m_btn8 = new CalculatorPushButton(Calculator::Button::Num8);
  m_btn8->setFlat(true);
  m_btn8->setText("");
  m_btn8->setIconSize(QSize(64, 64));
  m_btn8->setIcon(QIcon(":/icons/res/calc8.png"));
  m_btn8->setShortcut(QKeySequence(Qt::Key_8));

  m_btn9 = new CalculatorPushButton(Calculator::Button::Num9);
  m_btn9->setFlat(true);
  m_btn9->setText("");
  m_btn9->setIconSize(QSize(64, 64));
  m_btn9->setIcon(QIcon(":/icons/res/calc9.png"));
  m_btn9->setShortcut(QKeySequence(Qt::Key_9));

  m_btnMul = new CalculatorPushButton(Calculator::Button::Mul);
  m_btnMul->setFlat(true);
  m_btnMul->setText("");
  m_btnMul->setIconSize(QSize(64, 64));
  m_btnMul->setIcon(QIcon(":/icons/res/calcmul.png"));
  m_btnMul->setShortcut(QKeySequence(Qt::Key_Asterisk));

  QHBoxLayout* hline1 = new QHBoxLayout();
  hline1->addWidget(m_btn7);
  hline1->addWidget(m_btn8);
  hline1->addWidget(m_btn9);
  hline1->addWidget(m_btnMul);
  hline1->setAlignment(Qt::AlignCenter);
  hline1->setContentsMargins(0, 0, 0, 0);

  m_btn4 = new CalculatorPushButton(Calculator::Button::Num4);
  m_btn4->setFlat(true);
  m_btn4->setText("");
  m_btn4->setIconSize(QSize(64, 64));
  m_btn4->setIcon(QIcon(":/icons/res/calc4.png"));
  m_btn4->setShortcut(QKeySequence(Qt::Key_4));

  m_btn5 = new CalculatorPushButton(Calculator::Button::Num5);
  m_btn5->setFlat(true);
  m_btn5->setText("");
  m_btn5->setIconSize(QSize(64, 64));
  m_btn5->setIcon(QIcon(":/icons/res/calc5.png"));
  m_btn5->setShortcut(QKeySequence(Qt::Key_5));

  m_btn6 = new CalculatorPushButton(Calculator::Button::Num6);
  m_btn6->setFlat(true);
  m_btn6->setText("");
  m_btn6->setIconSize(QSize(64, 64));
  m_btn6->setIcon(QIcon(":/icons/res/calc6.png"));
  m_btn6->setShortcut(QKeySequence(Qt::Key_6));

  m_btnDiv = new CalculatorPushButton(Calculator::Button::Div);
  m_btnDiv->setFlat(true);
  m_btnDiv->setText("");
  m_btnDiv->setIconSize(QSize(64, 64));
  m_btnDiv->setIcon(QIcon(":/icons/res/calcdiv.png"));
  m_btnDiv->setShortcut(QKeySequence(Qt::Key_Slash));

  QHBoxLayout* hline2 = new QHBoxLayout();
  hline2->addWidget(m_btn4);
  hline2->addWidget(m_btn5);
  hline2->addWidget(m_btn6);
  hline2->addWidget(m_btnDiv);
  hline2->setAlignment(Qt::AlignCenter);
  hline2->setContentsMargins(0, 0, 0, 0);

  m_btn1 = new CalculatorPushButton(Calculator::Button::Num1);
  m_btn1->setFlat(true);
  m_btn1->setText("");
  m_btn1->setIconSize(QSize(64, 64));
  m_btn1->setIcon(QIcon(":/icons/res/calc1.png"));
  m_btn1->setShortcut(QKeySequence(Qt::Key_1));

  m_btn2 = new CalculatorPushButton(Calculator::Button::Num2);
  m_btn2->setFlat(true);
  m_btn2->setText("");
  m_btn2->setIconSize(QSize(64, 64));
  m_btn2->setIcon(QIcon(":/icons/res/calc2.png"));
  m_btn2->setShortcut(QKeySequence(Qt::Key_2));

  m_btn3 = new CalculatorPushButton(Calculator::Button::Num3);
  m_btn3->setFlat(true);
  m_btn3->setText("");
  m_btn3->setIconSize(QSize(64, 64));
  m_btn3->setIcon(QIcon(":/icons/res/calc3.png"));
  m_btn3->setShortcut(QKeySequence(Qt::Key_3));

  m_btnMin = new CalculatorPushButton(Calculator::Button::Min);
  m_btnMin->setFlat(true);
  m_btnMin->setText("");
  m_btnMin->setIconSize(QSize(64, 64));
  m_btnMin->setIcon(QIcon(":/icons/res/calcmin.png"));
  m_btnMin->setShortcut(QKeySequence(Qt::Key_Minus));

  QHBoxLayout* hline3 = new QHBoxLayout();
  hline3->addWidget(m_btn1);
  hline3->addWidget(m_btn2);
  hline3->addWidget(m_btn3);
  hline3->addWidget(m_btnMin);
  hline3->setAlignment(Qt::AlignCenter);
  hline3->setContentsMargins(0, 0, 0, 0);

  m_btn0 = new CalculatorPushButton(Calculator::Button::Num0);
  m_btn0->setFlat(true);
  m_btn0->setText("");
  m_btn0->setIconSize(QSize(64, 64));
  m_btn0->setIcon(QIcon(":/icons/res/calc0.png"));
  m_btn0->setShortcut(QKeySequence(Qt::Key_0));

  m_btnDec = new CalculatorPushButton(Calculator::Button::Dec);
  m_btnDec->setFlat(true);
  m_btnDec->setText("");
  m_btnDec->setIconSize(QSize(64, 64));
  m_btnDec->setIcon(QIcon(":/icons/res/calcdec.png"));
  m_btnDec->setShortcut(QKeySequence(Qt::Key_Period));

  m_btnEq = new CalculatorPushButton(Calculator::Button::Eq);
  m_btnEq->setFlat(true);
  m_btnEq->setText("");
  m_btnEq->setIconSize(QSize(64, 64));
  m_btnEq->setIcon(QIcon(":/icons/res/calcequal.png"));
  m_btnEq->setShortcut(QKeySequence(Qt::Key_Enter));

  m_btnPlus = new CalculatorPushButton(Calculator::Button::Plus);
  m_btnPlus->setFlat(true);
  m_btnPlus->setText("");
  m_btnPlus->setIconSize(QSize(64, 64));
  m_btnPlus->setIcon(QIcon(":/icons/res/calcplus.png"));
  m_btnPlus->setShortcut(QKeySequence(Qt::Key_Plus));

  QHBoxLayout* hline4 = new QHBoxLayout();
  hline4->addWidget(m_btn0);
  hline4->addWidget(m_btnDec);
  hline4->addWidget(m_btnEq);
  hline4->addWidget(m_btnPlus);
  hline4->setAlignment(Qt::AlignCenter);
  hline4->setContentsMargins(0, 0, 0, 0);

  QVBoxLayout* vlayoutl = new QVBoxLayout();
  vlayoutl->addLayout(hline0);
  vlayoutl->addLayout(hline1);
  vlayoutl->addLayout(hline2);
  vlayoutl->addLayout(hline3);
  vlayoutl->addLayout(hline4);
  vlayoutl->setAlignment(Qt::AlignTop);

  m_btnPrint = new QPushButton();
  m_btnPrint->setText("");
  m_btnPrint->setIconSize(QSize(24, 24));
  m_btnPrint->setIcon(QIcon(":/icons/res/calcprint.png"));
  m_btnPrint->setCheckable(true);
  m_btnPrint->setFlat(true);
  m_btnPrint->setChecked(true);

  m_rdoAlignLeft = new QRadioButton();
  m_rdoAlignLeft->setText("");
  m_rdoAlignLeft->setIconSize(QSize(24, 24));
  m_rdoAlignLeft->setIcon(QIcon(":/icons/res/calcalignleft.png"));
  m_rdoAlignLeft->setChecked(true);

  m_rdoAlignCenter = new QRadioButton();
  m_rdoAlignCenter->setText("");
  m_rdoAlignCenter->setIconSize(QSize(24, 24));
  m_rdoAlignCenter->setIcon(QIcon(":/icons/res/calcaligncenter.png"));

  QHBoxLayout* hline5 = new QHBoxLayout();
  hline5->addWidget(m_btnPrint);
  hline5->addWidget(m_rdoAlignLeft);
  hline5->addWidget(m_rdoAlignCenter);
  hline5->setAlignment(Qt::AlignLeft);
  hline5->setContentsMargins(0, 0, 0, 0);

  m_view = new QPlainTextEdit();
  m_view->setReadOnly(true);

  QVBoxLayout* vlayoutr = new QVBoxLayout();
  vlayoutr->addLayout(hline5);
  vlayoutr->addWidget(m_view);

  QFrame* framl = new QFrame();
  framl->setLayout(vlayoutl);

  QFrame* framr = new QFrame();
  framr->setLayout(vlayoutr);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(framl);
  splitter->addWidget(framr);

  QHBoxLayout* hlayout = new QHBoxLayout();
  hlayout->addWidget(splitter);
  hlayout->setContentsMargins(0, 0, 0, 0);

  setLayout(hlayout);

  QObject::connect(m_btn0,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btn1,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btn2,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btn3,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btn4,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btn5,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btn6,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btn7,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btn8,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btn9,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btnEq,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btnPlus,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btnMin,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btnDiv,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btnMul,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btnDec,
                   SIGNAL(calculatorButtonClickedSignal(Calculator::Button)),
                   this,
                   SLOT(calculatorButtonClicked(Calculator::Button)));
  QObject::connect(m_btnClr,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clear()));
  QObject::connect(m_btnCls,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(reset()));

  clear();
}

double CalculatorWidget::calculate(double op1, double op2, Calculator::Button button)
{
  if (!Calculator::isOP(button))
    return 0.0;

  switch (button)
  {
    case Calculator::Button::Plus:
      return op1 + op2;
    case Calculator::Button::Min:
      return op1 - op2;
    case Calculator::Button::Div:
      return op2 ? op1 / op2 : 0.0;
    case Calculator::Button::Mul:
      return op1 * op2;
    default:
      return 0.0;
  }
}

void CalculatorWidget::emitPrintSignal(double value, Calculator::Button button)
{
  QString text = QString::number(value, 'f').remove(QRegExp("\\.?0*$")) +
                 " " + Calculator::toChar(button);
  m_view->appendPlainText(text);
  m_view->verticalScrollBar()->setValue(m_view->verticalScrollBar()->maximum());
  if (m_btnPrint->isChecked())
  {
    QString text2 = m_rdoAlignLeft->isChecked() ? ESC_ALIGN_LEFT : ESC_ALIGN_CENTER;
    text2 += ESC_EXPAND_ON + text + ESC_LF ESC_EXPAND_OFF;
    emit printSignal(text2);
  }
}

void CalculatorWidget::calculatorButtonClicked(Calculator::Button button)
{
  if (Calculator::isOP(button))
  {
    double op2 = Calculator::isOP(m_lastButton)
                 ? m_lastValue
                 : m_currentValue;

    emitPrintSignal(op2, button);
    m_total = calculate(m_total, op2, button);
    m_edDisplay->setText(QString::number(m_total, 'f').
                         remove(QRegExp("\\.?0*$")));
    m_currentValue = 0.0;
    m_lastValue = op2;
  }
  else if (Calculator::isEqual(button))
  {
    emitPrintSignal(m_total, button);
  }
  else if (Calculator::isDecimal(button))
  {
    if (Calculator::isOP(m_lastButton) ||
        Calculator::isEqual(m_lastButton))
      m_edDisplay->setText("");
    else
    {
      QString strValue = m_edDisplay->text();
      strValue.replace(Calculator::toChar(button), "");
      strValue.append(Calculator::toChar(button));
      m_edDisplay->setText(strValue);
      m_currentValue = strValue.toDouble();
    }
  }
  else if (Calculator::isDigit(button))
  {
    if (Calculator::isOP(m_lastButton) ||
        Calculator::isEqual(m_lastButton))
      m_edDisplay->setText("");
    QString strValue = m_edDisplay->text();
    strValue.append(Calculator::toChar(button));
    m_edDisplay->setText(strValue);
    m_currentValue = strValue.toDouble();
  }
  m_lastButton = button;
}

void CalculatorWidget::clear()
{
  m_edDisplay->setText("");
  m_currentValue = 0.0;
  m_lastButton = Calculator::Button::Nop;
}

void CalculatorWidget::reset()
{
  m_edDisplay->setText("");
  m_view->setPlainText("");
  m_currentValue = 0.0;
  m_lastValue = 0.0;
  m_total = 0.0;
  m_lastButton = Calculator::Button::Nop;
  if (m_btnPrint->isChecked())
    emit printFullCutSignal();
}

QString CalculatorWidget::text() const
{
  return m_view->toPlainText();
}

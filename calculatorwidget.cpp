#include "calculatorwidget.h"

#include <QLayout>
#include <QPushButton>

CalculatorWidget::CalculatorWidget(QWidget* parent)
  : QFrame(parent)
{
  {
    m_btn7 = new QPushButton();
    m_btn7->setFlat(true);
    m_btn7->setText("");
    m_btn7->setIconSize(QSize(64, 64));
    m_btn7->setIcon(QIcon(":/icons/res/calc7.png"));
    m_btn7->setShortcut(QKeySequence(Qt::Key_7));

    m_btn8 = new QPushButton();
    m_btn8->setFlat(true);
    m_btn8->setText("");
    m_btn8->setIconSize(QSize(64, 64));
    m_btn8->setIcon(QIcon(":/icons/res/calc8.png"));
    m_btn8->setShortcut(QKeySequence(Qt::Key_8));

    m_btn9 = new QPushButton();
    m_btn9->setFlat(true);
    m_btn9->setText("");
    m_btn9->setIconSize(QSize(64, 64));
    m_btn9->setIcon(QIcon(":/icons/res/calc9.png"));
    m_btn9->setShortcut(QKeySequence(Qt::Key_9));

    m_btnMul = new QPushButton();
    m_btnMul->setFlat(true);
    m_btnMul->setText("");
    m_btnMul->setIconSize(QSize(64, 64));
    m_btnMul->setIcon(QIcon(":/icons/res/calcmul.png"));
    m_btnMul->setShortcut(QKeySequence(Qt::Key_Asterisk));
  }
  QHBoxLayout* hline1 = new QHBoxLayout();
  hline1->addWidget(m_btn7);
  hline1->addWidget(m_btn8);
  hline1->addWidget(m_btn9);
  hline1->addWidget(m_btnMul);
  hline1->setAlignment(Qt::AlignLeft);
  hline1->setContentsMargins(0, 0, 0, 0);

  {
    m_btn4 = new QPushButton();
    m_btn4->setFlat(true);
    m_btn4->setText("");
    m_btn4->setIconSize(QSize(64, 64));
    m_btn4->setIcon(QIcon(":/icons/res/calc4.png"));
    m_btn4->setShortcut(QKeySequence(Qt::Key_4));

    m_btn5 = new QPushButton();
    m_btn5->setFlat(true);
    m_btn5->setText("");
    m_btn5->setIconSize(QSize(64, 64));
    m_btn5->setIcon(QIcon(":/icons/res/calc5.png"));
    m_btn5->setShortcut(QKeySequence(Qt::Key_5));

    m_btn6 = new QPushButton();
    m_btn6->setFlat(true);
    m_btn6->setText("");
    m_btn6->setIconSize(QSize(64, 64));
    m_btn6->setIcon(QIcon(":/icons/res/calc6.png"));
    m_btn6->setShortcut(QKeySequence(Qt::Key_6));

    m_btnDiv = new QPushButton();
    m_btnDiv->setFlat(true);
    m_btnDiv->setText("");
    m_btnDiv->setIconSize(QSize(64, 64));
    m_btnDiv->setIcon(QIcon(":/icons/res/calcdiv.png"));
    m_btnDiv->setShortcut(QKeySequence(Qt::Key_Slash));
  }
  QHBoxLayout* hline2 = new QHBoxLayout();
  hline2->addWidget(m_btn4);
  hline2->addWidget(m_btn5);
  hline2->addWidget(m_btn6);
  hline2->addWidget(m_btnDiv);
  hline2->setAlignment(Qt::AlignLeft);
  hline2->setContentsMargins(0, 0, 0, 0);

  {
    m_btn1 = new QPushButton();
    m_btn1->setFlat(true);
    m_btn1->setText("");
    m_btn1->setIconSize(QSize(64, 64));
    m_btn1->setIcon(QIcon(":/icons/res/calc1.png"));
    m_btn1->setShortcut(QKeySequence(Qt::Key_1));

    m_btn2 = new QPushButton();
    m_btn2->setFlat(true);
    m_btn2->setText("");
    m_btn2->setIconSize(QSize(64, 64));
    m_btn2->setIcon(QIcon(":/icons/res/calc2.png"));
    m_btn2->setShortcut(QKeySequence(Qt::Key_2));

    m_btn3 = new QPushButton();
    m_btn3->setFlat(true);
    m_btn3->setText("");
    m_btn3->setIconSize(QSize(64, 64));
    m_btn3->setIcon(QIcon(":/icons/res/calc3.png"));
    m_btn3->setShortcut(QKeySequence(Qt::Key_3));

    m_btnMin = new QPushButton();
    m_btnMin->setFlat(true);
    m_btnMin->setText("");
    m_btnMin->setIconSize(QSize(64, 64));
    m_btnMin->setIcon(QIcon(":/icons/res/calcmin.png"));
    m_btnMin->setShortcut(QKeySequence(Qt::Key_Minus));

  }
  QHBoxLayout* hline3 = new QHBoxLayout();
  hline3->addWidget(m_btn1);
  hline3->addWidget(m_btn2);
  hline3->addWidget(m_btn3);
  hline3->addWidget(m_btnMin);
  hline3->setAlignment(Qt::AlignLeft);
  hline3->setContentsMargins(0, 0, 0, 0);

  {
    m_btn0 = new QPushButton();
    m_btn0->setFlat(true);
    m_btn0->setText("");
    m_btn0->setIconSize(QSize(64, 64));
    m_btn0->setIcon(QIcon(":/icons/res/calc0.png"));
    m_btn0->setShortcut(QKeySequence(Qt::Key_0));

    m_btnDec = new QPushButton();
    m_btnDec->setFlat(true);
    m_btnDec->setText("");
    m_btnDec->setIconSize(QSize(64, 64));
    m_btnDec->setIcon(QIcon(":/icons/res/calcdec.png"));
    m_btnDec->setShortcut(QKeySequence(Qt::Key_Comma, Qt::Key_Period));

    m_btnEq = new QPushButton();
    m_btnEq->setFlat(true);
    m_btnEq->setText("");
    m_btnEq->setIconSize(QSize(64, 64));
    m_btnEq->setIcon(QIcon(":/icons/res/calcequal.png"));
    m_btnEq->setShortcut(QKeySequence(Qt::Key_Return, Qt::Key_Enter));

    m_btnPlus = new QPushButton();
    m_btnPlus->setFlat(true);
    m_btnPlus->setText("");
    m_btnPlus->setIconSize(QSize(64, 64));
    m_btnPlus->setIcon(QIcon(":/icons/res/calcplus.png"));
    m_btnPlus->setShortcut(QKeySequence(Qt::Key_Plus));
  }
  QHBoxLayout* hline4 = new QHBoxLayout();
  hline4->addWidget(m_btn0);
  hline4->addWidget(m_btnDec);
  hline4->addWidget(m_btnEq);
  hline4->addWidget(m_btnPlus);
  hline4->setAlignment(Qt::AlignLeft);
  hline4->setContentsMargins(0, 0, 0, 0);

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->addLayout(hline1);
  vlayout->addLayout(hline2);
  vlayout->addLayout(hline3);
  vlayout->addLayout(hline4);
  setLayout(vlayout);
}

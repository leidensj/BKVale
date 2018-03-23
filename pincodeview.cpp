#include "pincodeview.h"
#include "databaseutils.h"
#include <QFrame>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QKeyEvent>

PinCodeView::PinCodeView(QWidget* parent)
  : QDialog(parent)
{
  m_btn7 = new QPushButton;
  m_btn7->setFlat(true);
  m_btn7->setText("");
  m_btn7->setIconSize(QSize(64, 64));
  m_btn7->setIcon(QIcon(":/icons/res/calc7.png"));
  m_btn7->setShortcut(QKeySequence(Qt::Key_7));

  m_btn8 = new QPushButton;
  m_btn8->setFlat(true);
  m_btn8->setText("");
  m_btn8->setIconSize(QSize(64, 64));
  m_btn8->setIcon(QIcon(":/icons/res/calc8.png"));
  m_btn8->setShortcut(QKeySequence(Qt::Key_8));

  m_btn9 = new QPushButton;
  m_btn9->setFlat(true);
  m_btn9->setText("");
  m_btn9->setIconSize(QSize(64, 64));
  m_btn9->setIcon(QIcon(":/icons/res/calc9.png"));
  m_btn9->setShortcut(QKeySequence(Qt::Key_9));

  QHBoxLayout* hline1 = new QHBoxLayout;
  hline1->addWidget(m_btn7);
  hline1->addWidget(m_btn8);
  hline1->addWidget(m_btn9);
  hline1->setContentsMargins(0, 0, 0, 0);

  m_btn4 = new QPushButton;
  m_btn4->setFlat(true);
  m_btn4->setText("");
  m_btn4->setIconSize(QSize(64, 64));
  m_btn4->setIcon(QIcon(":/icons/res/calc4.png"));
  m_btn4->setShortcut(QKeySequence(Qt::Key_4));

  m_btn5 = new QPushButton;
  m_btn5->setFlat(true);
  m_btn5->setText("");
  m_btn5->setIconSize(QSize(64, 64));
  m_btn5->setIcon(QIcon(":/icons/res/calc5.png"));
  m_btn5->setShortcut(QKeySequence(Qt::Key_5));

  m_btn6 = new QPushButton;
  m_btn6->setFlat(true);
  m_btn6->setText("");
  m_btn6->setIconSize(QSize(64, 64));
  m_btn6->setIcon(QIcon(":/icons/res/calc6.png"));
  m_btn6->setShortcut(QKeySequence(Qt::Key_6));

  QHBoxLayout* hline2 = new QHBoxLayout;
  hline2->addWidget(m_btn4);
  hline2->addWidget(m_btn5);
  hline2->addWidget(m_btn6);
  hline2->setContentsMargins(0, 0, 0, 0);

  m_btn1 = new QPushButton;
  m_btn1->setFlat(true);
  m_btn1->setText("");
  m_btn1->setIconSize(QSize(64, 64));
  m_btn1->setIcon(QIcon(":/icons/res/calc1.png"));
  m_btn1->setShortcut(QKeySequence(Qt::Key_1));

  m_btn2 = new QPushButton;
  m_btn2->setFlat(true);
  m_btn2->setText("");
  m_btn2->setIconSize(QSize(64, 64));
  m_btn2->setIcon(QIcon(":/icons/res/calc2.png"));
  m_btn2->setShortcut(QKeySequence(Qt::Key_2));

  m_btn3 = new QPushButton;
  m_btn3->setFlat(true);
  m_btn3->setText("");
  m_btn3->setIconSize(QSize(64, 64));
  m_btn3->setIcon(QIcon(":/icons/res/calc3.png"));
  m_btn3->setShortcut(QKeySequence(Qt::Key_3));

  QHBoxLayout* hline3 = new QHBoxLayout;
  hline3->addWidget(m_btn1);
  hline3->addWidget(m_btn2);
  hline3->addWidget(m_btn3);
  hline3->setContentsMargins(0, 0, 0, 0);

  m_btnClr = new QPushButton;
  m_btnClr->setFlat(true);
  m_btnClr->setText("");
  m_btnClr->setIconSize(QSize(64, 64));
  m_btnClr->setIcon(QIcon(":/icons/res/calcclr.png"));
  m_btnClr->setShortcut(QKeySequence(Qt::Key_Backspace));

  m_btn0 = new QPushButton;
  m_btn0->setFlat(true);
  m_btn0->setText("");
  m_btn0->setIconSize(QSize(64, 64));
  m_btn0->setIcon(QIcon(":/icons/res/calc0.png"));
  m_btn0->setShortcut(QKeySequence(Qt::Key_0));

  m_btnEnter = new QPushButton;
  m_btnEnter->setFlat(true);
  m_btnEnter->setText("");
  m_btnEnter->setIconSize(QSize(64, 64));
  m_btnEnter->setIcon(QIcon(":/icons/res/enter.png"));

  QHBoxLayout* hline4 = new QHBoxLayout();
  hline4->addWidget(m_btnClr);
  hline4->addWidget(m_btn0);
  hline4->addWidget(m_btnEnter);
  hline4->setContentsMargins(0, 0, 0, 0);

  m_edPinCode = new QLineEdit;
  m_edPinCode->setAlignment(Qt::AlignCenter);
  m_edPinCode->setReadOnly(true);
  m_edPinCode->setEchoMode(QLineEdit::EchoMode::Password);
  QFont f = m_edPinCode->font();
  f.setPointSize(24);
  m_edPinCode->setFont(f);

  QVBoxLayout* vlayoutl = new QVBoxLayout();
  vlayoutl->addWidget(m_edPinCode);
  vlayoutl->addLayout(hline1);
  vlayoutl->addLayout(hline2);
  vlayoutl->addLayout(hline3);
  vlayoutl->addLayout(hline4);
  vlayoutl->addStretch();

  setLayout(vlayoutl);
  m_edPinCode->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  setFixedSize(vlayoutl->sizeHint());

  QObject::connect(m_btnEnter, SIGNAL(clicked(bool)), this, SLOT(search()));
  QObject::connect(m_btn0, SIGNAL(clicked(bool)), this, SLOT(pressed0()));
  QObject::connect(m_btn1, SIGNAL(clicked(bool)), this, SLOT(pressed1()));
  QObject::connect(m_btn2, SIGNAL(clicked(bool)), this, SLOT(pressed2()));
  QObject::connect(m_btn3, SIGNAL(clicked(bool)), this, SLOT(pressed3()));
  QObject::connect(m_btn4, SIGNAL(clicked(bool)), this, SLOT(pressed4()));
  QObject::connect(m_btn5, SIGNAL(clicked(bool)), this, SLOT(pressed5()));
  QObject::connect(m_btn6, SIGNAL(clicked(bool)), this, SLOT(pressed6()));
  QObject::connect(m_btn7, SIGNAL(clicked(bool)), this, SLOT(pressed7()));
  QObject::connect(m_btn8, SIGNAL(clicked(bool)), this, SLOT(pressed8()));
  QObject::connect(m_btn9, SIGNAL(clicked(bool)), this, SLOT(pressed9()));
  QObject::connect(m_btnClr, SIGNAL(clicked(bool)), this, SLOT(erase()));
}

void PinCodeView::setDatabase(QSqlDatabase db)
{
  m_db = db;
}

void PinCodeView::append(QChar c)
{
  m_edPinCode->setText(m_edPinCode->text().append(c));
}

void PinCodeView::search()
{
  QString error;
  if (!PersonSQL::select(m_db, m_currentPerson, error))
  {
    QMessageBox::warning(this,
                         tr("Aviso"),
                         error,
                         QMessageBox::Ok);
  }
  else
  {
    close();
  }
}

void PinCodeView::erase()
{
  QString str = m_edPinCode->text();
  str.chop(1);
  m_edPinCode->setText(str);
}

Person PinCodeView::getCurrentPerson() const
{
  return m_currentPerson;
}

void PinCodeView::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    m_btnEnter->animateClick();
  else
    QDialog::keyPressEvent(event);
}

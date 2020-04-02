#include "pincodedialog.h"
#include "databaseutils.h"
#include <QFrame>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QKeyEvent>

#define BUTTON_CODE "button_code"

PinCodeDialog::PinCodeDialog(QWidget* parent)
  : QDialog(parent)
  , m_bError(false)
  , m_edPinCode(nullptr)
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
  , m_btnEnter(nullptr)
  , m_btnClr(nullptr)
{
  setWindowIcon(QIcon(":/icons/res/employee.png"));
  setWindowTitle(tr("Selecionar Funcionário"));
  m_btn7 = new QPushButton;
  m_btn7->setFlat(true);
  m_btn7->setText("");
  m_btn7->setIconSize(QSize(64, 64));
  m_btn7->setIcon(QIcon(":/icons/res/calc7.png"));
  m_btn7->setShortcut(QKeySequence(Qt::Key_7));
  m_btn7->setProperty(BUTTON_CODE, Qt::Key_7);

  m_btn8 = new QPushButton;
  m_btn8->setFlat(true);
  m_btn8->setText("");
  m_btn8->setIconSize(QSize(64, 64));
  m_btn8->setIcon(QIcon(":/icons/res/calc8.png"));
  m_btn8->setShortcut(QKeySequence(Qt::Key_8));
  m_btn8->setProperty(BUTTON_CODE, Qt::Key_8);

  m_btn9 = new QPushButton;
  m_btn9->setFlat(true);
  m_btn9->setText("");
  m_btn9->setIconSize(QSize(64, 64));
  m_btn9->setIcon(QIcon(":/icons/res/calc9.png"));
  m_btn9->setShortcut(QKeySequence(Qt::Key_9));
  m_btn9->setProperty(BUTTON_CODE, Qt::Key_9);

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
  m_btn4->setProperty(BUTTON_CODE, Qt::Key_4);

  m_btn5 = new QPushButton;
  m_btn5->setFlat(true);
  m_btn5->setText("");
  m_btn5->setIconSize(QSize(64, 64));
  m_btn5->setIcon(QIcon(":/icons/res/calc5.png"));
  m_btn5->setShortcut(QKeySequence(Qt::Key_5));
  m_btn5->setProperty(BUTTON_CODE, Qt::Key_5);

  m_btn6 = new QPushButton;
  m_btn6->setFlat(true);
  m_btn6->setText("");
  m_btn6->setIconSize(QSize(64, 64));
  m_btn6->setIcon(QIcon(":/icons/res/calc6.png"));
  m_btn6->setShortcut(QKeySequence(Qt::Key_6));
  m_btn6->setProperty(BUTTON_CODE, Qt::Key_6);

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
  m_btn1->setProperty(BUTTON_CODE, Qt::Key_1);

  m_btn2 = new QPushButton;
  m_btn2->setFlat(true);
  m_btn2->setText("");
  m_btn2->setIconSize(QSize(64, 64));
  m_btn2->setIcon(QIcon(":/icons/res/calc2.png"));
  m_btn2->setShortcut(QKeySequence(Qt::Key_2));
  m_btn2->setProperty(BUTTON_CODE, Qt::Key_2);

  m_btn3 = new QPushButton;
  m_btn3->setFlat(true);
  m_btn3->setText("");
  m_btn3->setIconSize(QSize(64, 64));
  m_btn3->setIcon(QIcon(":/icons/res/calc3.png"));
  m_btn3->setShortcut(QKeySequence(Qt::Key_3));
  m_btn3->setProperty(BUTTON_CODE, Qt::Key_3);

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
  m_btnClr->setProperty(BUTTON_CODE, Qt::Key_Backspace);

  m_btn0 = new QPushButton;
  m_btn0->setFlat(true);
  m_btn0->setText("");
  m_btn0->setIconSize(QSize(64, 64));
  m_btn0->setIcon(QIcon(":/icons/res/calc0.png"));
  m_btn0->setShortcut(QKeySequence(Qt::Key_0));
  m_btn0->setProperty(BUTTON_CODE, Qt::Key_0);

  m_btnEnter = new QPushButton;
  m_btnEnter->setFlat(true);
  m_btnEnter->setText("");
  m_btnEnter->setIconSize(QSize(64, 64));
  m_btnEnter->setIcon(QIcon(":/icons/res/ok.png"));
  m_btnEnter->setProperty(BUTTON_CODE, Qt::Key_Enter);

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
  f.setBold(true);
  m_edPinCode->setFont(f);

  QVBoxLayout* vlayoutl = new QVBoxLayout();
  vlayoutl->addWidget(m_edPinCode);
  vlayoutl->addLayout(hline1);
  vlayoutl->addLayout(hline2);
  vlayoutl->addLayout(hline3);
  vlayoutl->addLayout(hline4);
  vlayoutl->addStretch();

  setLayout(vlayoutl);
  setFixedSize(vlayoutl->minimumSize());

  m_edPinCode->installEventFilter(this);
  m_btnClr->installEventFilter(this);
  m_btn0->installEventFilter(this);
  m_btn1->installEventFilter(this);
  m_btn2->installEventFilter(this);
  m_btn3->installEventFilter(this);
  m_btn4->installEventFilter(this);
  m_btn5->installEventFilter(this);
  m_btn6->installEventFilter(this);
  m_btn7->installEventFilter(this);
  m_btn8->installEventFilter(this);
  m_btn9->installEventFilter(this);

  QObject::connect(m_btnEnter, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btn0, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btn1, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btn2, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btn3, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btn4, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btn5, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btn6, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btn7, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btn8, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btn9, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
  QObject::connect(m_btnClr, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));

}

void PinCodeDialog::buttonPressed()
{
  auto p = sender();
  if (p == nullptr)
    return;
  switch (p->property(BUTTON_CODE).toInt())
  {
    case Qt::Key_0:
      append('0');
      break;
    case Qt::Key_1:
      append('1');
      break;
    case Qt::Key_2:
      append('2');
      break;
    case Qt::Key_3:
      append('3');
      break;
    case Qt::Key_4:
      append('4');
      break;
    case Qt::Key_5:
      append('5');
      break;
    case Qt::Key_6:
      append('6');
      break;
    case Qt::Key_7:
      append('7');
      break;
    case Qt::Key_8:
      append('8');
      break;
    case Qt::Key_9:
      append('9');
      break;
    case Qt::Key_Enter:
      search();
      break;
    case Qt::Key_Backspace:
      erase();
      break;
  }
}

void PinCodeDialog::append(QChar c)
{
  if (m_bError)
    erase();
  m_edPinCode->setText(m_edPinCode->text().append(c));
}

void PinCodeDialog::search()
{
  QString error;
  m_employee.m_pincode = m_edPinCode->text();
  if (!m_employee.SQL_select_by_pincode(error))
  {
    QPalette palette = m_edPinCode->palette();
    palette.setColor(QPalette::Base, QColor(255, 200, 200));
    m_edPinCode->setPalette(palette);
    m_edPinCode->setEchoMode(QLineEdit::EchoMode::Normal);
    m_edPinCode->setText(error);
    m_bError = true;
  }
  else
  {
    accept();
  }
}

void PinCodeDialog::erase()
{
  m_edPinCode->clear();
  if (m_bError)
  {
    QPalette palette = m_edPinCode->palette();
    palette.setColor(QPalette::Base, Qt::white);
    m_edPinCode->setPalette(palette);
    m_edPinCode->setEchoMode(QLineEdit::EchoMode::Password);
    m_bError = false;
  }
}

Employee PinCodeDialog::getEmployee() const
{
  return m_employee;
}

bool PinCodeDialog::eventFilter(QObject *target, QEvent *event)
{
  if (event->type() == QEvent::KeyPress)
  {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_Return ||
        keyEvent->key() == Qt::Key_Enter)
    {
      m_btnEnter->animateClick();
      return true;
    }
  }
  return QDialog::eventFilter(target, event);
}

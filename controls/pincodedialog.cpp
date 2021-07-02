#include "pincodedialog.h"
#include "databaseutils.h"
#include <QFrame>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QKeyEvent>

#define BUTTON_CODE "button_code"

namespace
{
  QPushButton* createButton(int idx, QWidget* parent)
  {
    QPushButton* btn = new QPushButton;
    btn->setFlat(true);
    btn->setIconSize(QSize(64, 64));
    btn->setIcon(QIcon(QString(":/icons/res/calc%1.png").arg(QString::number(idx))));
    btn->setShortcut(QKeySequence(Qt::Key_0 + idx));
    btn->setProperty(BUTTON_CODE, Qt::Key_0 + idx);
    btn->installEventFilter(parent);
    QObject::connect(btn, SIGNAL(clicked(bool)), parent, SLOT(buttonPressed()));
    return btn;
  }
}

PinCodeDialog::PinCodeDialog(QWidget* parent)
  : QDialog(parent)
  , m_bError(false)
  , m_edPinCode(nullptr)
  , m_btnEnter(nullptr)
  , m_btnClr(nullptr)
{
  setWindowIcon(QIcon(":/icons/res/employee.png"));
  setWindowTitle(tr("Selecionar Funcionário"));

  for (int i = 0; i != 10; ++i)
    m_btn[i] = createButton(i, this);

  m_btnClr = new QPushButton;
  m_btnClr->setFlat(true);
  m_btnClr->setIconSize(QSize(64, 64));
  m_btnClr->setIcon(QIcon(":/icons/res/calcclr.png"));
  m_btnClr->setShortcut(QKeySequence(Qt::Key_Backspace));
  m_btnClr->setProperty(BUTTON_CODE, Qt::Key_Backspace);
  m_btnClr->installEventFilter(this);
  QObject::connect(m_btnClr, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));

  m_btnEnter = new QPushButton;
  m_btnEnter->setFlat(true);
  m_btnEnter->setIconSize(QSize(64, 64));
  m_btnEnter->setIcon(QIcon(":/icons/res/ok.png"));
  m_btnEnter->setProperty(BUTTON_CODE, Qt::Key_Enter);
  QObject::connect(m_btnEnter, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));

  QHBoxLayout* hline1 = new QHBoxLayout;
  hline1->addWidget(m_btn[7]);
  hline1->addWidget(m_btn[8]);
  hline1->addWidget(m_btn[9]);
  hline1->setContentsMargins(0, 0, 0, 0);

  QHBoxLayout* hline2 = new QHBoxLayout;
  hline2->addWidget(m_btn[4]);
  hline2->addWidget(m_btn[5]);
  hline2->addWidget(m_btn[6]);
  hline2->setContentsMargins(0, 0, 0, 0);

  QHBoxLayout* hline3 = new QHBoxLayout;
  hline3->addWidget(m_btn[1]);
  hline3->addWidget(m_btn[2]);
  hline3->addWidget(m_btn[3]);
  hline3->setContentsMargins(0, 0, 0, 0);

  QHBoxLayout* hline4 = new QHBoxLayout();
  hline4->addWidget(m_btnClr);
  hline4->addWidget(m_btn[0]);
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

  m_edPinCode->installEventFilter(this);
  setLayout(vlayoutl);
  setFixedSize(vlayoutl->minimumSize());
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

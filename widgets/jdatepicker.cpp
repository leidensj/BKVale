#include "jdatepicker.h"

#include <QTimer>
#include "jdateedit.h"
#include <QLayout>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QPushButton>

JDatePicker::JDatePicker(QWidget* parent)
  : QWidget(parent)
{
  m_dt = new JDateEdit;
  m_dt->setCalendarPopup(true);
  m_dt->setDate(QDate::currentDate());

  m_btn = new QPushButton;
  m_btn->setFlat(true);
  m_btn->setText("");
  m_btn->setIconSize(QSize(24, 24));
  m_btn->setIcon(QIcon(":/icons/res/calendarok.png"));

  QHBoxLayout* lt = new QHBoxLayout;
  lt->setContentsMargins(0, 0, 0, 0);
  lt->addWidget(m_dt);
  lt->addWidget(m_btn);

  connect(m_btn, SIGNAL(clicked(bool)), this, SLOT(setToday()));
  connect(m_dt, SIGNAL(dateChanged(const QDate&)), this, SLOT(emitDateChangedSignal(const QDate&)));
  connect(m_dt, SIGNAL(dateChanged(const QDate&)), this, SLOT(checkDate()));

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(checkDate()));
  timer->start(60000);

  setLayout(lt);
  checkDate();
}

void JDatePicker::checkDate()
{
  QTextCharFormat fmt;
  m_dt->calendarWidget()->setDateTextFormat(QDate::currentDate().addDays(-1), fmt);
  fmt.setForeground(Qt::darkGreen);
  fmt.setFontWeight(QFont::ExtraBold);
  fmt.setFontUnderline(true);
  fmt.setFontOverline(true);
  m_dt->calendarWidget()->setDateTextFormat(QDate::currentDate(), fmt);

  m_btn->setIcon(QIcon(m_dt->date() == QDate::currentDate()
                       ? ":/icons/res/calendarok.png"
                       : ":/icons/res/calendarwarning.png"));
  m_btn->setToolTip(m_dt->date() == QDate::currentDate()
                    ? tr("A data informada é a data de hoje.")
                    : tr("Aviso! A data informada não é a de hoje. Usar a data de hoje?"));
}

void JDatePicker::setToday()
{
  m_dt->setDate(QDate::currentDate());
}

QDate JDatePicker::getDate() const
{
  return m_dt->date();
}

void JDatePicker::setDate(const QDate& dt)
{
  m_dt->setDate(dt);
}

void JDatePicker::emitDateChangedSignal(const QDate& dt)
{
  emit dateChangedSignal(dt);
}

#include "jdatepicker.h"

#include <QTimer>
#include "jdateedit.h"
#include <QLayout>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QToolButton>
#include <QAction>

JDatePicker::JDatePicker(QWidget* parent)
  : QWidget(parent)
{
  m_dt = new JDateEdit;
  m_dt->setCalendarPopup(true);
  m_dt->setDate(QDate::currentDate());

  m_btn = new QToolButton;
  m_btn->setIconSize(QSize(24, 24));
  m_btn->setAutoRaise(true);
  m_btn->setPopupMode(QToolButton::MenuButtonPopup);

  auto actToday = new QAction(QIcon(":/icons/res/calendarok.png"), tr("Hoje"), m_btn);
  auto actMinusDay = new QAction(QIcon(":/icons/res/calminday.png"), tr("- Dia"), m_btn);
  auto actPlusDay = new QAction(QIcon(":/icons/res/calplusday.png"), tr("+ Dia"), m_btn);
  auto actMinusMonth = new QAction(QIcon(":/icons/res/calminmonth.png"), tr("- Mês"), m_btn);
  auto actPlusMonth = new QAction(QIcon(":/icons/res/calplusmonth.png"), tr("+ Mês"), m_btn);
  auto actMinusYear = new QAction(QIcon(":/icons/res/calminyear.png"), tr("- Ano"), m_btn);
  auto actPlusYear = new QAction(QIcon(":/icons/res/calplusyear.png"), tr("+ Ano"), m_btn);
  m_btn->addActions({actToday, actMinusDay, actPlusDay, actMinusMonth, actPlusMonth, actMinusYear, actPlusYear});
  m_btn->setDefaultAction(actToday);

  QHBoxLayout* lt = new QHBoxLayout;
  lt->setContentsMargins(0, 0, 0, 0);
  lt->addWidget(m_dt);
  lt->addWidget(m_btn);

  auto dt = m_dt;
  connect(actToday, &QAction::triggered, [dt](){ dt->setDate(QDate::currentDate()); });
  connect(actMinusDay, &QAction::triggered, [dt](){ dt->setDate(dt->date().addDays(-1)); });
  connect(actPlusDay, &QAction::triggered, [dt](){ dt->setDate(dt->date().addDays(1)); });
  connect(actMinusMonth, &QAction::triggered, [dt](){ dt->setDate(dt->date().addMonths(-1)); });
  connect(actPlusMonth, &QAction::triggered, [dt](){ dt->setDate(dt->date().addMonths(1)); });
  connect(actMinusYear, &QAction::triggered, [dt](){ dt->setDate(dt->date().addYears(-1)); });
  connect(actPlusYear, &QAction::triggered, [dt](){ dt->setDate(dt->date().addYears(1)); });
  connect(m_dt, &JDateEdit::dateChanged, [this](const QDate& dt) { emit dateChangedSignal(dt); });
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

QDate JDatePicker::getDate() const
{
  return m_dt->date();
}

void JDatePicker::setDate(const QDate& dt)
{
  m_dt->setDate(dt);
}

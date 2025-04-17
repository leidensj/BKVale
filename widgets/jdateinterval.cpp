#include "jdateinterval.h"
#include "jdateedit.h"
#include <QToolButton>
#include <QAction>
#include <QLayout>
#include <QFormLayout>

JDateInterval::JDateInterval(QWidget *parent)
  : QGroupBox(parent)
  , m_btn(nullptr)
  , m_dtInit(nullptr)
  , m_dtFinal(nullptr)
{
  m_btn = new QToolButton;
  m_btn->setIconSize(QSize(24, 24));
  m_btn->setAutoRaise(true);
  m_btn->setPopupMode(QToolButton::MenuButtonPopup);

  auto actDay = new QAction(QIcon(":/icons/res/calendarok.png"), tr("Hoje"), m_btn);
  auto actWeek = new QAction(QIcon(":/icons/res/calendarweek.png"), tr("Esta semana"), m_btn);
  auto actMonth = new QAction(QIcon(":/icons/res/calendarmonth.png"), tr("Este mês"), m_btn);
  auto actYear = new QAction(QIcon(":/icons/res/calendaryear.png"), tr("Este ano"), m_btn);
  auto actYesterday = new QAction(QIcon(":/icons/res/calminday.png"), tr("Ontem"), m_btn);
  auto actLastWeek = new QAction(QIcon(":/icons/res/calminweek.png"), tr("Semana passada"), m_btn);
  auto actLastMonth = new QAction(QIcon(":/icons/res/calminmonth.png"), tr("Mês passado"), m_btn);
  auto actLastYear = new QAction(QIcon(":/icons/res/calminyear.png"), tr("Ano passado"), m_btn);

  m_btn->addActions({actDay, actWeek, actMonth, actYear, actYesterday, actLastWeek, actLastMonth, actLastYear });
  m_btn->setDefaultAction(actDay);

  connect(actDay, SIGNAL(triggered(bool)), this, SLOT(day()));
  connect(actWeek, SIGNAL(triggered(bool)), this, SLOT(week()));
  connect(actMonth, SIGNAL(triggered(bool)), this, SLOT(month()));
  connect(actYear, SIGNAL(triggered(bool)), this, SLOT(year()));
  connect(actYesterday, SIGNAL(triggered(bool)), this, SLOT(yesterday()));
  connect(actLastWeek, SIGNAL(triggered(bool)), this, SLOT(lastWeek()));
  connect(actLastMonth, SIGNAL(triggered(bool)), this, SLOT(lastMonth()));
  connect(actLastYear, SIGNAL(triggered(bool)), this, SLOT(lastYear()));

  setCheckable(true);
  setTitle(tr("Data"));
  m_dtInit = new JDateEdit;
  m_dtInit->setCalendarPopup(true);
  m_dtFinal = new JDateEdit;
  m_dtFinal->setCalendarPopup(true);

  QFormLayout* ltfr = new QFormLayout;
  ltfr->setContentsMargins(0, 0, 0, 0);
  ltfr->addRow(tr("Data inicial:"), m_dtInit);
  ltfr->addRow(tr("Data final:"), m_dtFinal);

  QHBoxLayout* ltMain= new QHBoxLayout;
  ltMain->addLayout(ltfr);
  ltMain->addWidget(m_btn);

  connect(m_dtInit, SIGNAL(dateChanged(const QDate)), this, SLOT(emitChangedSignal()));
  connect(m_dtFinal, SIGNAL(dateChanged(const QDate)), this, SLOT(emitChangedSignal()));
  connect(this, SIGNAL(clicked(bool)), this, SLOT(emitChangedSignal()));

  day();
  setLayout(ltMain);
}

void JDateInterval::year()
{
  setChecked(true);
  QDate dt(QDate::currentDate());
  m_dtInit->setDate(QDate(dt.year(), 1, 1));
  m_dtFinal->setDate(dt);
}

void JDateInterval::month()
{
  setChecked(true);
  QDate dt(QDate::currentDate());
  m_dtInit->setDate(QDate(dt.year(), dt.month(), 1));
  m_dtFinal->setDate(dt);
}

void JDateInterval::week()
{
  setChecked(true);
  QDate dt(QDate::currentDate());
  m_dtInit->setDate(dt.addDays(dt.dayOfWeek() == 7 ? 0 : -dt.dayOfWeek()));
  m_dtFinal->setDate(dt);
}

void JDateInterval::day()
{
  setChecked(true);
  QDate dt(QDate::currentDate());
  m_dtInit->setDate(dt);
  m_dtFinal->setDate(dt);
}

void JDateInterval::lastYear()
{
  setChecked(true);
  QDate dt(QDate::currentDate());
  m_dtInit->setDate(QDate(dt.addYears(-1).year(), 1, 1));
  m_dtFinal->setDate(QDate(dt.addYears(-1).year(), 12, 31));
}

void JDateInterval::lastMonth()
{
  setChecked(true);
  QDate dt(QDate::currentDate());
  m_dtInit->setDate(QDate(dt.year(), dt.addMonths(-1).month(), 1));
  m_dtFinal->setDate(QDate(dt.year(), dt.addMonths(-1).month(), dt.addMonths(-1).daysInMonth()));
}

void JDateInterval::lastWeek()
{
  setChecked(true);
  QDate dt(QDate::currentDate());
  m_dtInit->setDate(dt.addDays((dt.dayOfWeek() == 7 ? 0 : -dt.dayOfWeek()) - 7));
  m_dtFinal->setDate(dt.addDays((dt.dayOfWeek() == 7 ? 0 : -dt.dayOfWeek()) -1));
}

void JDateInterval::yesterday()
{
  setChecked(true);
  QDate dt(QDate::currentDate());
  m_dtInit->setDate(dt.addDays(-1));
  m_dtFinal->setDate(dt.addDays(-1));
}

QDate JDateInterval::getInitialDate() const
{
  return m_dtInit->date();
}

QDate JDateInterval::getFinalDate() const
{
  return m_dtFinal->date();
}

void JDateInterval::emitChangedSignal()
{
  emit changedSignal(isChecked(), m_dtInit->date(), m_dtFinal->date());
}

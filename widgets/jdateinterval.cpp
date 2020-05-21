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
  m_btn->addActions({actDay, actWeek, actMonth, actYear });
  m_btn->setDefaultAction(actDay);

  connect(actDay, SIGNAL(triggered(bool)), this, SLOT(day()));
  connect(actWeek, SIGNAL(triggered(bool)), this, SLOT(week()));
  connect(actMonth, SIGNAL(triggered(bool)), this, SLOT(month()));
  connect(actYear, SIGNAL(triggered(bool)), this, SLOT(year()));

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

  connect(m_dtInit, &JDateEdit::dateChanged, [this](const QDate& dt){ emit initialDateChangedSignal(dt); });
  connect(m_dtFinal, &JDateEdit::dateChanged, [this](const QDate& dt){ emit finalDateChangedSignal(dt); });

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

QDate JDateInterval::getInitialDate() const
{
  return m_dtInit->date();
}

QDate JDateInterval::getFinalDate() const
{
  return m_dtFinal->date();
}


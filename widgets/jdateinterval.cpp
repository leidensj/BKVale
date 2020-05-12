#include "jdateinterval.h"
#include "jdateedit.h"
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>

JDateInterval::JDateInterval(QWidget *parent)
  : QGroupBox(parent)
  , m_btnYear(nullptr)
  , m_btnMonth(nullptr)
  , m_btnWeek(nullptr)
  , m_btnDay(nullptr)
  , m_dtInit(nullptr)
  , m_dtFinal(nullptr)
{
  m_btnYear = new QPushButton;
  m_btnYear->setFlat(true);
  m_btnYear->setIconSize(QSize(16, 16));
  m_btnYear->setIcon(QIcon(":/icons/res/calendaryear.png"));
  m_btnYear->setToolTip(tr("Este ano"));
  m_btnMonth = new QPushButton;
  m_btnMonth->setFlat(true);
  m_btnMonth->setIconSize(QSize(16, 16));
  m_btnMonth->setIcon(QIcon(":/icons/res/calendarmonth.png"));
  m_btnMonth->setToolTip(tr("Este mês"));
  m_btnWeek = new QPushButton;
  m_btnWeek->setFlat(true);
  m_btnWeek->setIconSize(QSize(16, 16));
  m_btnWeek->setIcon(QIcon(":/icons/res/calendarweek.png"));
  m_btnWeek->setToolTip(tr("Esta semana"));
  m_btnDay = new QPushButton;
  m_btnDay->setFlat(true);
  m_btnDay->setIconSize(QSize(16, 16));
  m_btnDay->setIcon(QIcon(":/icons/res/calendarok.png"));
  m_btnDay->setToolTip(tr("Hoje"));

  setCheckable(true);
  setTitle(tr("Data"));
  m_dtInit = new JDateEdit;
  m_dtInit->setCalendarPopup(true);
  m_dtFinal = new JDateEdit;
  m_dtFinal->setCalendarPopup(true);

  QHBoxLayout* ltButton = new QHBoxLayout;
  ltButton->setContentsMargins(0, 0, 0, 0);
  ltButton->setAlignment(Qt::AlignLeft);
  ltButton->addWidget(m_btnYear);
  ltButton->addWidget(m_btnMonth);
  ltButton->addWidget(m_btnWeek);
  ltButton->addWidget(m_btnDay);

  QFormLayout* ltfr = new QFormLayout;
  ltfr->setContentsMargins(0, 0, 0, 0);
  ltfr->addRow(tr("Data inicial:"), m_dtInit);
  ltfr->addRow(tr("Data final:"), m_dtFinal);

  QVBoxLayout* ltMain= new QVBoxLayout;
  ltMain->addLayout(ltButton);
  ltMain->addLayout(ltfr);

  connect(m_btnYear, SIGNAL(clicked(bool)), this, SLOT(year()));
  connect(m_btnMonth, SIGNAL(clicked(bool)), this, SLOT(month()));
  connect(m_btnWeek, SIGNAL(clicked(bool)), this, SLOT(week()));
  connect(m_btnDay, SIGNAL(clicked(bool)), this, SLOT(day()));
  connect(m_dtInit, SIGNAL(dateChanged(const QDate&)), this, SLOT(emitInitialDateChangedSignal(const QDate&)));
  connect(m_dtFinal, SIGNAL(dateChanged(const QDate&)), this, SLOT(emitFinalDateChangedSignal(const QDate&)));

  day();
  setLayout(ltMain);
}

void JDateInterval::emitInitialDateChangedSignal(const QDate& dt)
{
  emit initialDateChangedSignal(dt);
}

void JDateInterval::emitFinalDateChangedSignal(const QDate& dt)
{
  emit finalDateChangedSignal(dt);
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


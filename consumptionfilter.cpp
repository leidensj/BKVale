#include "consumptionfilter.h"
#include <QDateEdit>
#include <QLabel>
#include <QLayout>
#include <QCheckBox>

ConsumptionFilter::ConsumptionFilter(QWidget* parent)
  : QFrame(parent)
  , m_datei(nullptr)
  , m_datef(nullptr)
  , m_enable(nullptr)
{
  m_enable = new QCheckBox();
  m_enable->setText("");

  QFrame* vFrame = new QFrame;
  vFrame->setFrameShape(QFrame::VLine);

  QLabel* dateli = new QLabel();
  dateli->setText("Data inicial:");
  QFont f1 = dateli->font();
  f1.setPointSize(8);
  dateli->setFont(f1);

  QLabel* datelf = new QLabel();
  datelf->setText("Data final:");
  QFont f2 = datelf->font();
  f2.setPointSize(8);
  datelf->setFont(f2);

  m_datei = new QDateEdit();
  m_datei->setCalendarPopup(true);
  m_datei->setDate(QDate::currentDate());
  m_datei->setDisplayFormat("dd/MM/yyyy");

  m_datef = new QDateEdit();
  m_datef->setCalendarPopup(true);
  m_datef->setDate(QDate::currentDate());
  m_datef->setDisplayFormat("dd/MM/yyyy");

  QHBoxLayout* hlayout = new QHBoxLayout();
  hlayout->addWidget(m_enable);
  hlayout->addWidget(vFrame);
  hlayout->addWidget(dateli);
  hlayout->addWidget(m_datei);
  hlayout->addWidget(datelf);
  hlayout->addWidget(m_datef);
  hlayout->setContentsMargins(0, 0, 0, 0);
  hlayout->setAlignment(Qt::AlignLeft);

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->addLayout(hlayout);

  setFrameShape(QFrame::Shape::StyledPanel);
  setFrameShadow(QFrame::Shadow::Plain);
  setLayout(vlayout);

  QObject::connect(m_enable,
                   SIGNAL(stateChanged(int)),
                   this,
                   SLOT(enableControls()));

  QObject::connect(m_enable,
                   SIGNAL(stateChanged(int)),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(m_datei,
                   SIGNAL(dateChanged(const QDate&)),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(m_datef,
                   SIGNAL(dateChanged(const QDate&)),
                   this,
                   SLOT(emitChangedSignal()));

  enableControls();
}

void ConsumptionFilter::enableControls()
{
  const bool bEnable = m_enable->isChecked();
  m_datei->setEnabled(bEnable);
  m_datef->setEnabled(bEnable);
}

void ConsumptionFilter::emitChangedSignal()
{
  emit changedSignal(filter());
}

Consumption::Filter ConsumptionFilter::filter() const
{
  Consumption::Filter f;
  f.m_bDate = m_enable->isChecked();
  f.m_datei = m_datei->date().toJulianDay();
  f.m_datef = m_datef->date().toJulianDay();
  return f;
}

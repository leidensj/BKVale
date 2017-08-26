#include "consumptionfilter.h"
#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <QCheckBox>

ConsumptionFilter::ConsumptionFilter(QWidget* parent)
  : QFrame(parent)
{
  {
    m_enable = new QCheckBox();
    m_enable->setText("");
  }

  QFrame* vFrame = new QFrame;
  vFrame->setFrameShape(QFrame::VLine);

  QLabel* dateli = new QLabel();
  {
    dateli->setText("Data inicial:");
    QFont f = dateli->font();
    f.setPointSize(8);
    dateli->setFont(f);
  }

  QLabel* datelf = new QLabel();
  {
    datelf->setText("Data final:");
    QFont f = datelf->font();
    f.setPointSize(8);
    datelf->setFont(f);
  }

  {
    m_datei = new QDateEdit();
    m_datei->setCalendarPopup(true);
    m_datei->setDate(QDate::currentDate());
  }

  {
    m_datef = new QDateEdit();
    m_datef->setCalendarPopup(true);
    m_datef->setDate(QDate::currentDate());
  }

  {
    m_total = new QLineEdit();
    m_total->setAlignment(Qt::AlignRight);
    m_total->setPlaceholderText("Consumo Total");
    m_total->setReadOnly(true);
    QPalette palette = m_total->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    m_total->setPalette(palette);
  }

  {
    m_graph = new QPushButton();
    m_graph->setFlat(true);
    m_graph->setText("");
    m_graph->setIconSize(QSize(24, 24));
    m_graph->setIcon(QIcon(":/icons/res/chart.png"));
    m_graph->setDefault(true);
  }

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->addWidget(m_enable);
  hlayout1->addWidget(vFrame);
  hlayout1->addWidget(dateli);
  hlayout1->addWidget(m_datei);
  hlayout1->addWidget(datelf);
  hlayout1->addWidget(m_datef);
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);

  QHBoxLayout* hlayout2 = new QHBoxLayout();
  hlayout2->addWidget(m_total);
  hlayout2->addWidget(m_graph);
  hlayout2->setContentsMargins(0, 0, 0, 0);

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->addLayout(hlayout1);
  vlayout->addLayout(hlayout2);

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
  m_total->setEnabled(bEnable);
  m_graph->setEnabled(bEnable);
}

void ConsumptionFilter::emitChangedSignal()
{
  emit changedSignal(m_enable->isChecked(),
                     m_datei->date().toJulianDay(),
                     m_datef->date().toJulianDay());
}

void ConsumptionFilter::updateTotal(double total)
{
  if (m_enable->isChecked())
    m_total->setText("R$ " + QString::number(total, 'f', 2));
  else
    m_total->setText("");
}

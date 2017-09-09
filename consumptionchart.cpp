#include "consumptionchart.h"
#include <QLayout>

ConsumptionChart::ConsumptionChart(const QVector<qint64>& vDate,
                                   const QVector<double>& vTotal,
                                   QWidget* parent)
  : QFrame(parent)
{
  m_plot = new QCustomPlot();

  // set dark background gradient:
  QLinearGradient gradient(0, 0, 0, 400);
  gradient.setColorAt(0, QColor(90, 90, 90));
  gradient.setColorAt(0.38, QColor(105, 105, 105));
  gradient.setColorAt(1, QColor(70, 70, 70));
  m_plot->setBackground(QBrush(gradient));

  // create empty bar chart objects:
  QCPBars *bars = new QCPBars(m_plot->xAxis, m_plot->yAxis);
  bars->setAntialiased(false); // gives more crisp, pixel aligned bar borders
  bars->setStackingGap(1);
  // set names and colors:
  bars->setName("Consumo diário");
  bars->setPen(QPen(QColor(0, 168, 140).lighter(130)));
  bars->setBrush(QColor(0, 168, 140));
  // stack bars on top of each other:
  //nuclear->moveAbove(fossil);
  //regen->moveAbove(nuclear);

  // prepare x axis with country labels:
  QVector<double> ticks;
  QVector<QString> labels;
  for (int i = 0; i != vDate.size(); ++i)
  {
    ticks << (i + 1);
    labels << QDate::fromJulianDay(vDate.at(i)).toString("dd/MM/yyyy");
  }
  QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
  textTicker->addTicks(ticks, labels);
  m_plot->xAxis->setTicker(textTicker);
  m_plot->xAxis->setTickLabelRotation(60);
  m_plot->xAxis->setSubTicks(false);
  m_plot->xAxis->setTickLength(0, 4);
  m_plot->xAxis->setRange(0, ticks.size() + 1);
  m_plot->xAxis->setBasePen(QPen(Qt::white));
  m_plot->xAxis->setTickPen(QPen(Qt::white));
  m_plot->xAxis->grid()->setVisible(true);
  m_plot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
  m_plot->xAxis->setTickLabelColor(Qt::white);
  m_plot->xAxis->setLabelColor(Qt::white);

  // prepare y axis:
  m_plot->yAxis->setRange(0, 100);
  m_plot->yAxis->setPadding(5); // a bit more space to the left border
  m_plot->yAxis->setLabel("Consumo (em R$)");
  m_plot->yAxis->setBasePen(QPen(Qt::white));
  m_plot->yAxis->setTickPen(QPen(Qt::white));
  m_plot->yAxis->setSubTickPen(QPen(Qt::white));
  m_plot->yAxis->grid()->setSubGridVisible(true);
  m_plot->yAxis->setTickLabelColor(Qt::white);
  m_plot->yAxis->setLabelColor(Qt::white);
  m_plot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
  m_plot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

  // Add data:
  bars->setData(ticks, vTotal);

  // setup legend:
  /*customPlot->legend->setVisible(true);
  customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
  customPlot->legend->setBrush(QColor(255, 255, 255, 100));
  customPlot->legend->setBorderPen(Qt::NoPen);
  QFont legendFont = font();
  legendFont.setPointSize(10);
  customPlot->legend->setFont(legendFont);*/
  m_plot->yAxis->rescale(true);
  m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->addWidget(m_plot);
  setLayout(vlayout);
}



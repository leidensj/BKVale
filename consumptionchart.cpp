#include "consumptionchart.h"
#include <QLayout>

ConsumptionChart::ConsumptionChart(const QVector<qint64>& dates,
                                   const QVector<double>& totals,
                                   QWidget* parent)
  : QFrame(parent)
{
  m_plot = new QCustomPlot();
  QVector<double> x(dates.size());
  for (int i=0; i != dates.size(); ++i)
    x[i] = i;
  // create graph and assign data to it:
  m_plot->addGraph();
  m_plot->graph(0)->setData(x, totals);
  // give the axes some labels:
  m_plot->xAxis->setLabel("x");
  m_plot->yAxis->setLabel("Total (em R$)");
  // set axes ranges, so we see all data:
  m_plot->xAxis->setRange(0, 10);
  m_plot->yAxis->setRange(0, 300);
  m_plot->replot();

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->addWidget(m_plot);
  setLayout(vlayout);
}



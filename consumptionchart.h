#ifndef CONSUMPTIONCHART_H
#define CONSUMPTIONCHART_H

#include <QFrame>
#include "qcustomplot.h"

class ConsumptionChart : public QFrame
{
public:
  explicit ConsumptionChart(QWidget* parent = 0);

private:
  QCustomPlot* m_plot;
};

#endif // CONSUMPTIONCHART_H

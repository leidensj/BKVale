#ifndef CONSUMPTIONCHART_H
#define CONSUMPTIONCHART_H

#include <QFrame>
#include <QVector>
#include "qcustomplot.h"

class ConsumptionChart : public QFrame
{
public:
  explicit ConsumptionChart(const QVector<qint64>& dates,
                            const QVector<double>& totals,
                            QWidget* parent = 0);

private:
  QCustomPlot* m_plot;
};

#endif // CONSUMPTIONCHART_H

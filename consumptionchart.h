#ifndef CONSUMPTIONCHART_H
#define CONSUMPTIONCHART_H

#include <QFrame>
#include <QVector>
#include "qcustomplot.h"

class ConsumptionChart : public QFrame
{
public:
  explicit ConsumptionChart(const QVector<qint64>& vDate,
                            const QVector<double>& vTotal,
                            QWidget* parent = 0);

private:
  QCustomPlot* m_plot;
};

#endif // CONSUMPTIONCHART_H

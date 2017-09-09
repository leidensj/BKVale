#ifndef CONSUMPTIONFILTER_H
#define CONSUMPTIONFILTER_H

#include <QFrame>
#include "consumption.h"

class QDateEdit;
class QLabel;
class QCheckBox;

class ConsumptionFilter : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionFilter(QWidget* parent = 0);
  Consumption::Filter filter() const;

private:
  QDateEdit* m_datei;
  QDateEdit* m_datef;
  QCheckBox* m_enable;

private slots:
  void emitChangedSignal();

public slots:
  void enableControls();

signals:
  void changedSignal(const Consumption::Filter& filter);
};

#endif // CONSUMPTIONFILTER_H

#ifndef CONSUMPTIONFILTER_H
#define CONSUMPTIONFILTER_H

#include <QFrame>

class QDateEdit;
class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;

class ConsumptionFilter : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionFilter(QWidget* parent = 0);

private:
  QDateEdit* m_datei;
  QDateEdit* m_datef;
  QLineEdit* m_total;
  QPushButton* m_chart;
  QCheckBox* m_enable;

private slots:
  void emitChangedSignal();
  void emitChartSignal();

public slots:
  void enableControls();
  void updateTotal(double total);

signals:
  void changedSignal(bool bEnable,
                     qint64 datei,
                     qint64 datef);

  void chartSignal(bool bEnable,
                   qint64 datei,
                   qint64 datef);
};

#endif // CONSUMPTIONFILTER_H

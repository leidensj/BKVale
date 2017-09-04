#ifndef CONSUMPTIONDATABASE_H
#define CONSUMPTIONDATABASE_H

#include <QFrame>
#include <QSqlDatabase>
#include "consumption.h"

class QPushButton;
class QTableView;

class ConsumptionDatabase : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionDatabase(QWidget *parent = 0);
  ~ConsumptionDatabase();
  void setDatabase(QSqlDatabase db);

private slots:
  void enableControls();
  void insert(const Consumption& consumption);
  void remove();
  void refresh();
  void emitTotalSignal(bool bEnable,
                       qint64 datei,
                       qint64 datef);
  void emitFilterSignal();

public slots:
  void setFilter(bool bEnable,
                 qint64 datei,
                 qint64 datef);

  void processChartData(bool bEnable,
                        qint64 datei,
                        qint64 datef);

signals:
  void filterSignal();
  void totalSignal(double total);
  void chartSignal(const QVector<qint64>& dates,
                   const QVector<double>& totals);

private:
  QPushButton* m_filter;
  QPushButton* m_refresh;
  QPushButton* m_remove;
  QTableView* m_table;
};

#endif // CONSUMPTIONDATABASE_H

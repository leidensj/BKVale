#ifndef CONSUMPTIONDATABASE_H
#define CONSUMPTIONDATABASE_H

#include <QFrame>
#include <QSqlDatabase>
#include "consumption.h"
#include "item.h"

class QPushButton;
class QTableView;
class QLineEdit;

class ConsumptionDatabase : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionDatabase(QWidget *parent = 0);
  ~ConsumptionDatabase();
  void setDatabase(QSqlDatabase db);
  bool isValid() const;

private slots:
  void enableControls();
  void insert(const Consumption& consumption);
  void remove();
  void updateTotal();
  void emitFilterSignal();
  void emitChartSignal();

public slots:
  void setFilter(const Consumption::Filter& filter);
  void refresh();

signals:
  void filterSignal();
  void chartSignal(const QVector<qint64>& vDate,
                   const QVector<double>& vTotal);

private:
  QPushButton* m_openFilter;
  QPushButton* m_refresh;
  QPushButton* m_remove;
  QTableView* m_table;
  QLineEdit* m_total;
  QPushButton* m_chart;
  Consumption::Filter m_filter;
};

#endif // CONSUMPTIONDATABASE_H

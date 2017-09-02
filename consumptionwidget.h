#ifndef CONSUMPTIONWIDGET_H
#define CONSUMPTIONWIDGET_H

#include <QFrame>
#include <QSqlDatabase>

class ConsumptionView;
class ConsumptionDatabase;
class ConsumptionFilter;

class ConsumptionWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionWidget(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);

private slots:
  void showChart(const QVector<qint64>& dates,
                 const QVector<double>& totals);

private:
  ConsumptionView* m_view;
  ConsumptionDatabase* m_database;
  ConsumptionFilter* m_filter;
};

#endif // CONSUMPTIONWIDGET_H

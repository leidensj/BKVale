#ifndef CONSUMPTIONWIDGET_H
#define CONSUMPTIONWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "consumption.h"
#include "product.h"

class ConsumptionView;
class ConsumptionDatabase;
class ConsumptionFilter;
class QDockWidget;

class ConsumptionWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionWidget(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);
  QString printContent();
  bool isValid() const;

private slots:
  void showChart(const QVector<qint64>& vDate,
                 const QVector<double>& vTotal);
  void showFilter();

private:
  QSqlDatabase m_db;
  ConsumptionView* m_view;
  ConsumptionDatabase* m_database;
  ConsumptionFilter* m_filter;
  QDockWidget* m_dock;
};

#endif // CONSUMPTIONWIDGET_H

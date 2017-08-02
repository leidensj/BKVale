#ifndef CONSUMPTIONWIDGET_H
#define CONSUMPTIONWIDGET_H

#include <QFrame>
#include <QSqlDatabase>

namespace Ui {
class ConsumptionWidget;
}

class ConsumptionWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionWidget(QWidget *parent = 0);
  ~ConsumptionWidget();
  void setDatabase(QSqlDatabase db);

private:
  Ui::ConsumptionWidget *ui;
  QSqlDatabase m_db;

private slots:
  void search();
};

#endif // CONSUMPTIONWIDGET_H

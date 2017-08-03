#ifndef CONSUMPTIONDATABASEWIDGET_H
#define CONSUMPTIONDATABASEWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "consumptionwidget.h"

namespace Ui {
class ConsumptionDatabaseWidget;
}

class ConsumptionDatabaseWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionDatabaseWidget(QWidget *parent = 0);
  ~ConsumptionDatabaseWidget();
  void setDatabase(QSqlDatabase db);

private slots:
  void create();

private:
  Ui::ConsumptionDatabaseWidget *ui;
  QSqlDatabase m_db;
  ConsumptionWidget m_consumptionWidget;
};

#endif // CONSUMPTIONDATABASEWIDGET_H

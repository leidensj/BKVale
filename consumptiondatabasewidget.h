#ifndef CONSUMPTIONDATABASEWIDGET_H
#define CONSUMPTIONDATABASEWIDGET_H

#include <QFrame>
#include <QSqlDatabase>

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
};

#endif // CONSUMPTIONDATABASEWIDGET_H

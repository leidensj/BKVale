#ifndef CONSUMPTIONDATABASEWIDGET_H
#define CONSUMPTIONDATABASEWIDGET_H

#include <QFrame>

namespace Ui {
class ConsumptionDatabaseWidget;
}

class ConsumptionDatabaseWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionDatabaseWidget(QWidget *parent = 0);
  ~ConsumptionDatabaseWidget();

private:
  Ui::ConsumptionDatabaseWidget *ui;
};

#endif // CONSUMPTIONDATABASEWIDGET_H

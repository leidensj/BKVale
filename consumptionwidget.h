#ifndef CONSUMPTIONWIDGET_H
#define CONSUMPTIONWIDGET_H

#include <QFrame>

namespace Ui {
class ConsumptionWidget;
}

class ConsumptionWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionWidget(QWidget *parent = 0);
  ~ConsumptionWidget();

private:
  Ui::ConsumptionWidget *ui;
};

#endif // CONSUMPTIONWIDGET_H

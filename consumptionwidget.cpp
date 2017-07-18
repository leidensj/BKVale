#include "consumptionwidget.h"
#include "ui_consumptionwidget.h"

ConsumptionWidget::ConsumptionWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::ConsumptionWidget)
{
  ui->setupUi(this);
}

ConsumptionWidget::~ConsumptionWidget()
{
  delete ui;
}

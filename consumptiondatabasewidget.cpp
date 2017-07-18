#include "consumptiondatabasewidget.h"
#include "ui_consumptiondatabasewidget.h"

ConsumptionDatabaseWidget::ConsumptionDatabaseWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::ConsumptionDatabaseWidget)
{
  ui->setupUi(this);
}

ConsumptionDatabaseWidget::~ConsumptionDatabaseWidget()
{
  delete ui;
}

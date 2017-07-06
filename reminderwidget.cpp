#include "reminderwidget.h"
#include "ui_reminderwidget.h"

ReminderWidget::ReminderWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::ReminderWidget)
{
  ui->setupUi(this);
}

ReminderWidget::~ReminderWidget()
{
  delete ui;
}

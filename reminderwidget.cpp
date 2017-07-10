#include "reminderwidget.h"
#include "ui_reminderwidget.h"

ReminderWidget::ReminderWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::ReminderWidget)
{
  ui->setupUi(this);
  ui->buttonFontSmall->setChecked(true);
}

ReminderWidget::~ReminderWidget()
{
  delete ui;
}

Reminder ReminderWidget::reminder() const
{
  Reminder r;
  r.m_title = ui->editTitle->text();
  r.m_message = ui->editMessage->toPlainText();
  r.m_bFontSmall = ui->buttonFontSmall->isChecked();
  return r;
}

void ReminderWidget::clear()
{
  ui->buttonFontSmall->setChecked(true);
  ui->editTitle->clear();
  ui->editMessage->clear();
}

bool ReminderWidget::isValid() const
{
  return !ui->editTitle->text().isEmpty() &&
      !ui->editMessage->toPlainText().isEmpty();
}

#include "reminderwidget.h"
#include "ui_reminderwidget.h"

ReminderWidget::ReminderWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::ReminderWidget)
{
  ui->setupUi(this);
  ui->buttonFontBig->setChecked(true);
  ui->buttonUppercase->setCheckState(Qt::CheckState::Checked);
  setCapitalization((int)Qt::CheckState::Checked);

  QObject::connect(ui->editTitle,
                   SIGNAL(textEdited(const QString&)),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(ui->editMessage,
                   SIGNAL(textChanged()),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(ui->buttonUppercase,
                   SIGNAL(stateChanged(int)),
                   this,
                   SLOT(setCapitalization(int)));

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
  switch(ui->buttonUppercase->checkState())
  {
    case Qt::CheckState::Unchecked:
      r.m_bfontType = Reminder::FontType::Normal; break;
    case Qt::CheckState::PartiallyChecked:
      r.m_bfontType = Reminder::FontType::AllLowercase; break;
    case Qt::CheckState::Checked:
    default:
      r.m_bfontType = Reminder::FontType::AllUppercase; break;
  }
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
  return !ui->editTitle->text().isEmpty() ||
      !ui->editMessage->toPlainText().isEmpty();
}

void ReminderWidget::emitChangedSignal()
{
  emit changedSignal();
}

void ReminderWidget::setCapitalization(int state)
{
  QFont::Capitalization cap = QFont::AllUppercase;
  switch ((Qt::CheckState)state)
  {
    case Qt::CheckState::Unchecked:
      cap = QFont::MixedCase; break;
    case Qt::CheckState::PartiallyChecked:
      cap = QFont::AllLowercase; break;
    default:
      cap = QFont::AllUppercase;
  }

  {
    QFont f = ui->editTitle->font();
    f.setCapitalization(cap);
    ui->editTitle->setFont(f);
  }

  {
    QFont f = ui->editMessage->font();
    f.setCapitalization(cap);
    ui->editMessage->setFont(f);
  }
}

#include "reminderview.h"
#include "ui_reminderview.h"

ReminderView::ReminderView(QWidget *parent)
  : QFrame(parent)
  , ui(new Ui::ReminderView)
  , m_currentID(INVALID_REMINDER_ID)
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

ReminderView::~ReminderView()
{
  delete ui;
}

Reminder ReminderView::reminder() const
{
  Reminder r;
  r.m_id = m_currentID;
  r.m_title = ui->editTitle->text();
  r.m_message = ui->editMessage->toPlainText();
  r.m_size = ui->buttonFontSmall->isChecked() ? Reminder::Size::Normal
                                              : Reminder::Size::Large;
  switch(ui->buttonUppercase->checkState())
  {
    case Qt::CheckState::Unchecked:
      r.m_capitalization = Reminder::Capitalization::Normal;
      break;
    case Qt::CheckState::PartiallyChecked:
      r.m_capitalization = Reminder::Capitalization::AllLowercase;
      break;
    case Qt::CheckState::Checked:
    default:
      r.m_capitalization = Reminder::Capitalization::AllUppercase;
      break;
  }
  return r;
}

void ReminderView::clear()
{
  ui->buttonFontSmall->setChecked(true);
  ui->editTitle->clear();
  ui->editMessage->clear();
  m_currentID = INVALID_REMINDER_ID;
}

bool ReminderView::isValid() const
{
  return !ui->editTitle->text().isEmpty() ||
      !ui->editMessage->toPlainText().isEmpty();
}

void ReminderView::emitChangedSignal()
{
  emit changedSignal();
}

void ReminderView::setCapitalization(int state)
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

void ReminderView::setReminder(const Reminder r)
{
  m_currentID = r.m_id;
  ui->editTitle->setText(r.m_title);
  ui->editMessage->setPlainText(r.m_message);
  ui->buttonFontBig->setChecked(r.m_size == Reminder::Size::Large);
  ui->buttonFontSmall->setChecked(r.m_size == Reminder::Size::Normal);
  switch(r.m_capitalization)
  {
    case Reminder::Capitalization::AllLowercase:
      ui->buttonUppercase->setCheckState(Qt::PartiallyChecked);
      break;
    case Reminder::Capitalization::AllUppercase:
      ui->buttonUppercase->setCheckState(Qt::Checked);
      break;
    case Reminder::Capitalization::Normal:
    default:
      ui->buttonUppercase->setCheckState(Qt::Unchecked);
      break;
  }
  //TODO
}

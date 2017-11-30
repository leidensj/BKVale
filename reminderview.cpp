#include "reminderview.h"
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QLayout>
#include <QPushButton>

ReminderView::ReminderView(QWidget *parent)
  : QFrame(parent)
  , m_currentID(INVALID_REMINDER_ID)
  , m_title(nullptr)
  , m_message(nullptr)
  , m_capitalization(nullptr)
  , m_size1(nullptr)
  , m_size2(nullptr)
  , m_create(nullptr)
  , m_search(nullptr)
{
  m_create = new QPushButton();
  m_create->setFlat(true);
  m_create->setText("");
  m_create->setIconSize(QSize(24, 24));
  m_create->setIcon(QIcon(":/icons/res/file.png"));
  m_create->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
  m_search = new QPushButton();
  m_search->setFlat(true);
  m_search->setText("");
  m_search->setIconSize(QSize(24, 24));
  m_search->setIcon(QIcon(":/icons/res/search.png"));
  m_search->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
  m_save = new QCheckBox();
  m_save->setText(tr("Salvar lembrete"));
  m_save->setChecked(true);
  m_title = new QLineEdit();
  m_title->setPlaceholderText(tr("Título"));
  m_message = new QPlainTextEdit();
  m_message->setPlaceholderText(tr("Mensagem"));
  m_capitalization = new QCheckBox();
  m_capitalization->setText("");
  m_capitalization->setIconSize(QSize(24, 24));
  m_capitalization->setIcon(QIcon(":/icons/res/uppercase.png"));
  m_capitalization->setTristate(true);
  m_capitalization->setCheckState(Qt::CheckState::Checked);
  m_size1 = new QRadioButton();
  m_size1->setText("");
  m_size1->setIconSize(QSize(16, 16));
  m_size1->setIcon(QIcon(":/icons/res/text.png"));
  m_size1->setChecked(true);
  m_size2 = new QRadioButton();
  m_size2->setIconSize(QSize(24, 24));
  m_size2->setIcon(QIcon(":/icons/res/text.png"));

  QFrame* vFrame1 = new QFrame;
  vFrame1->setFrameShape(QFrame::VLine);

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_create);
  hlayout1->addWidget(m_search);
  hlayout1->addWidget(vFrame1);
  hlayout1->addWidget(m_save);

  QFrame* vFrame2 = new QFrame;
  vFrame2->setFrameShape(QFrame::VLine);

  QHBoxLayout* hlayout2 = new QHBoxLayout();
  hlayout2->setContentsMargins(0, 0, 0, 0);
  hlayout2->setAlignment(Qt::AlignLeft);
  hlayout2->addWidget(m_capitalization);
  hlayout2->addWidget(vFrame2);
  hlayout2->addWidget(m_size1);
  hlayout2->addWidget(m_size2);

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->setContentsMargins(0, 0, 0, 0);
  vlayout->addLayout(hlayout1);
  vlayout->addWidget(m_title);
  vlayout->addLayout(hlayout2);
  vlayout->addWidget(m_message);

  setLayout(vlayout);

  QObject::connect(m_title,
                   SIGNAL(textEdited(const QString&)),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(m_message,
                   SIGNAL(textChanged()),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(m_capitalization,
                   SIGNAL(stateChanged(int)),
                   this,
                   SLOT(setCapitalization(int)));

  QObject::connect(m_create,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clear()));

  QObject::connect(m_search,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSearchClickedSignal()));

  setCapitalization(m_capitalization->checkState());
}

ReminderView::~ReminderView()
{

}

Reminder ReminderView::reminder() const
{
  Reminder r;
  r.m_id = m_currentID;
  r.m_title = m_title->text();
  r.m_message = m_message->toPlainText();
  r.m_size = m_size1->isChecked() ? Reminder::Size::Normal
                                  : Reminder::Size::Large;
  switch(m_capitalization->checkState())
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
  m_size1->setChecked(true);
  m_title->clear();
  m_message->clear();
  m_currentID = INVALID_REMINDER_ID;
}

bool ReminderView::isValid() const
{
  return !m_title->text().isEmpty() ||
      !m_message->toPlainText().isEmpty();
}

void ReminderView::emitChangedSignal()
{
  emit changedSignal();
}

void ReminderView::emitSearchClickedSignal()
{
  emit searchClickedSignal();
}

void ReminderView::setCapitalization(int state)
{
  QFont::Capitalization cap = QFont::AllUppercase;
  switch ((Qt::CheckState)state)
  {
    case Qt::CheckState::Unchecked:
      cap = QFont::MixedCase;
      break;
    case Qt::CheckState::PartiallyChecked:
      cap = QFont::AllLowercase;
      break;
    case Qt::CheckState::Checked:
    default:
      cap = QFont::AllUppercase;
      break;
  }

  QFont f1 = m_title->font();
  f1.setCapitalization(cap);
  m_title->setFont(f1);

  QFont f2 = m_message->font();
  f2.setCapitalization(cap);
  m_message->setFont(f2);
}

void ReminderView::setReminder(const Reminder r)
{
  m_currentID = r.m_id;
  m_title->setText(r.m_title);
  m_message->setPlainText(r.m_message);
  m_size2->setChecked(r.m_size == Reminder::Size::Large);
  m_size1->setChecked(r.m_size == Reminder::Size::Normal);
  switch(r.m_capitalization)
  {
    case Reminder::Capitalization::AllLowercase:
      m_capitalization->setCheckState(Qt::PartiallyChecked);
      break;
    case Reminder::Capitalization::AllUppercase:
      m_capitalization->setCheckState(Qt::Checked);
      break;
    case Reminder::Capitalization::Normal:
    default:
      m_capitalization->setCheckState(Qt::Unchecked);
      break;
  }
}

bool ReminderView::isSaveChecked() const
{
  return m_save->isChecked();
}

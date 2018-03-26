#include "reminderview.h"
#include "jlineedit.h"
#include "jdatabase.h"
#include "printutils.h"
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QLayout>
#include <QPushButton>
#include <QDockWidget>
#include <QSplitter>
#include <QMessageBox>

ReminderView::ReminderView(QWidget *parent)
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_edTitle(nullptr)
  , m_teMessage(nullptr)
  , m_cbCapitalization(nullptr)
  , m_rdSize1(nullptr)
  , m_rdSize2(nullptr)
  , m_cbFavorite(nullptr)
  , m_btnCreate(nullptr)
  , m_btnSearch(nullptr)
  , m_cbSave(nullptr)
  , m_database(nullptr)
  , m_dock(nullptr)
{
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
  m_btnSearch = new QPushButton;
  m_btnSearch->setFlat(true);
  m_btnSearch->setText("");
  m_btnSearch->setIconSize(QSize(24, 24));
  m_btnSearch->setIcon(QIcon(":/icons/res/search.png"));
  m_btnSearch->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
  m_cbSave = new QCheckBox();
  m_cbSave->setText(tr("Salvar lembrete"));
  m_cbSave->setChecked(true);
  m_edTitle = new JLineEdit(JValidatorType::AlphanumericAndSpaces, false, true);
  m_edTitle->setPlaceholderText(tr("Título"));
  m_teMessage = new QPlainTextEdit();
  m_teMessage->setPlaceholderText(tr("Mensagem"));
  m_cbCapitalization = new QCheckBox();
  m_cbCapitalization->setText("");
  m_cbCapitalization->setIconSize(QSize(24, 24));
  m_cbCapitalization->setIcon(QIcon(":/icons/res/uppercase.png"));
  m_cbCapitalization->setTristate(true);
  m_cbCapitalization->setCheckState(Qt::CheckState::Checked);
  m_rdSize1 = new QRadioButton();
  m_rdSize1->setText("");
  m_rdSize1->setIconSize(QSize(16, 16));
  m_rdSize1->setIcon(QIcon(":/icons/res/text.png"));
  m_rdSize1->setChecked(true);
  m_rdSize2 = new QRadioButton();
  m_rdSize2->setIconSize(QSize(24, 24));
  m_rdSize2->setIcon(QIcon(":/icons/res/text.png"));
  m_cbFavorite = new QCheckBox;
  m_cbFavorite->setText(tr("Favorito"));
  m_cbFavorite->setIcon(QIcon(":/icons/res/favorite.png"));
  m_cbFavorite->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F));

  QFrame* vFrame1 = new QFrame;
  vFrame1->setFrameShape(QFrame::VLine);

  QHBoxLayout* hlayout1 = new QHBoxLayout;
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_btnCreate);
  hlayout1->addWidget(m_btnSearch);
  hlayout1->addWidget(vFrame1);
  hlayout1->addWidget(m_cbSave);

  QFrame* vFrame2 = new QFrame;
  vFrame2->setFrameShape(QFrame::VLine);

  QFrame* vFrame3 = new QFrame;
  vFrame3->setFrameShape(QFrame::VLine);

  QHBoxLayout* hlayout2 = new QHBoxLayout();
  hlayout2->setContentsMargins(0, 0, 0, 0);
  hlayout2->setAlignment(Qt::AlignLeft);
  hlayout2->addWidget(m_cbCapitalization);
  hlayout2->addWidget(vFrame2);
  hlayout2->addWidget(m_rdSize1);
  hlayout2->addWidget(m_rdSize2);
  hlayout2->addWidget(vFrame3);
  hlayout2->addWidget(m_cbFavorite);

  QVBoxLayout* viewLayout = new QVBoxLayout;
  viewLayout->setContentsMargins(9, 0, 0, 0);
  viewLayout->addLayout(hlayout1);
  viewLayout->addWidget(m_edTitle);
  viewLayout->addLayout(hlayout2);
  viewLayout->addWidget(m_teMessage);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewLayout);

  m_database = new JDatabase;
  m_database->layout()->setContentsMargins(0, 0, 9, 0);

  m_dock = new QDockWidget();
  m_dock->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  m_dock->setFeatures(0);
  m_dock->setFeatures(QDockWidget::DockWidgetClosable);
  m_dock->setWindowTitle("Pesquisar");
  m_dock->setWidget(m_database);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_dock);
  splitter->addWidget(viewFrame);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  m_dock->close();

  QObject::connect(m_edTitle,
                   SIGNAL(textEdited(const QString&)),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(m_teMessage,
                   SIGNAL(textChanged()),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(m_cbCapitalization,
                   SIGNAL(stateChanged(int)),
                   this,
                   SLOT(setCapitalization(int)));

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));

  QObject::connect(m_btnSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(search()));

  setCapitalization(m_cbCapitalization->checkState());
}

void ReminderView::setDatabase(QSqlDatabase db)
{
  m_database->setDatabase(db, REMINDER_SQL_TABLE_NAME, Reminder::getColumns());
}

Reminder ReminderView::getReminder() const
{
  Reminder r;
  r.m_id = m_currentId;
  r.m_title = m_edTitle->text();
  r.m_message = m_teMessage->toPlainText();
  r.m_bFavorite = m_cbFavorite->isChecked();
  r.m_size = m_rdSize1->isChecked() ? Reminder::Size::Normal
                                    : Reminder::Size::Large;
  switch(m_cbCapitalization->checkState())
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

void ReminderView::create()
{
  Reminder reminder;
  setReminder(reminder);
}

void ReminderView::itemSelected(const JItem& jItem)
{
  const Reminder& reminder = dynamic_cast<const Reminder&>(jItem);
  if (reminder.isValidId())
    setReminder(reminder);
}

void ReminderView::itemRemoved(qlonglong id)
{
  if (id == m_currentId)
    create();
}

bool ReminderView::save()
{
  if (!m_cbSave->isChecked())
    return true;
  bool bSuccess = m_database->save(getReminder());
  if (bSuccess)
    create();
  return bSuccess;
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

  QFont f1 = m_edTitle->font();
  f1.setCapitalization(cap);
  m_edTitle->setFont(f1);

  QFont f2 = m_teMessage->font();
  f2.setCapitalization(cap);
  m_teMessage->setFont(f2);
}

void ReminderView::setReminder(const Reminder& reminder)
{
  m_currentId = reminder.m_id;
  m_edTitle->setText(reminder.m_title);
  m_teMessage->setPlainText(reminder.m_message);
  m_cbFavorite->setChecked(reminder.m_bFavorite);
  m_rdSize2->setChecked(reminder.m_size == Reminder::Size::Large);
  m_rdSize1->setChecked(reminder.m_size == Reminder::Size::Normal);
  switch(reminder.m_capitalization)
  {
    case Reminder::Capitalization::AllLowercase:
      m_cbCapitalization->setCheckState(Qt::PartiallyChecked);
      break;
    case Reminder::Capitalization::AllUppercase:
      m_cbCapitalization->setCheckState(Qt::Checked);
      break;
    case Reminder::Capitalization::Normal:
    default:
      m_cbCapitalization->setCheckState(Qt::Unchecked);
      break;
  }
}

void ReminderView::search()
{
  if (m_dock->isVisible())
    m_dock->close();
  else
    m_dock->show();
}

bool ReminderView::print(QIODevice* printer, InterfaceType type)
{
  QString str(ReminderPrinter::build(getReminder()));
  QString error;
  bool bSuccess = Printer::printString(printer, type, str, error);
  if (!bSuccess)
  {
    QMessageBox::warning(this,
                         tr("Erro"),
                         tr("Erro '%1' ao imprimir o lembrete.").arg(error),
                         QMessageBox::Ok);
  }
  return bSuccess;
}


void ReminderView::saveAndPrint(QIODevice* printer, InterfaceType type)
{
  if (save())
  {
    if (print(printer, type))
      create();
  }
}

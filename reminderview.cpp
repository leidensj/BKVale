#include "reminderview.h"
#include "jlineedit.h"
#include "jdatabase.h"
#include "jspinbox.h"
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QLayout>
#include <QPushButton>
#include <QDockWidget>
#include <QSplitter>
#include <QMessageBox>
#include <QLabel>
#include <QDialog>
#include <QDialogButtonBox>

ReminderView::ReminderView(QWidget *parent)
  : QFrame(parent)
  , m_edTitle(nullptr)
  , m_teMessage(nullptr)
  , m_edBarcode(nullptr)
  , m_cbCapitalization(nullptr)
  , m_rdSize1(nullptr)
  , m_rdSize2(nullptr)
  , m_cbFavorite(nullptr)
  , m_cbBarcodeHRI(nullptr)
  , m_btnCreate(nullptr)
  , m_btnSearch(nullptr)
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
  m_edTitle = new JLineEdit(JLineEdit::Input::ASCII,
                            JLineEdit::st_defaultFlags2);
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
  m_edBarcode = new JLineEdit(JLineEdit::Input::All, JLineEdit::st_defaultFlags2);
  m_edBarcode->setPlaceholderText(tr("Código de barras"));
  m_edBarcode->setMaxLength(REMINDER_MAX_BARCODE_CODE93_LENGTH);
  m_edBarcode->setClearButtonEnabled(true);
  m_cbBarcodeHRI = new QCheckBox;
  m_cbBarcodeHRI->setText(tr("Incluir HRI"));

  QLabel* lblBarcode = new QLabel();
  lblBarcode->setPixmap(QIcon(":/icons/res/barcode.png").pixmap(QSize(24, 24)));
  lblBarcode->setMinimumSize(24, 24);
  lblBarcode->setMaximumSize(24, 24);
  lblBarcode->setScaledContents(true);

  QFrame* vFrame1 = new QFrame;
  vFrame1->setFrameShape(QFrame::VLine);

  QFrame* vFrame2 = new QFrame;
  vFrame2->setFrameShape(QFrame::VLine);

  QFrame* vFrame3 = new QFrame;
  vFrame3->setFrameShape(QFrame::VLine);

  QHBoxLayout* hlayout1 = new QHBoxLayout;
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_btnCreate);
  hlayout1->addWidget(m_btnSearch);
  hlayout1->addWidget(vFrame1);
  hlayout1->addWidget(m_cbCapitalization);
  hlayout1->addWidget(vFrame2);
  hlayout1->addWidget(m_rdSize1);
  hlayout1->addWidget(m_rdSize2);
  hlayout1->addWidget(vFrame3);
  hlayout1->addWidget(m_cbFavorite);

  QHBoxLayout* hlayout2 = new QHBoxLayout;
  hlayout2->setContentsMargins(0, 0, 0, 0);
  hlayout2->setAlignment(Qt::AlignLeft);
  hlayout2->addWidget(lblBarcode);
  hlayout2->addWidget(m_edBarcode);
  hlayout2->addWidget(m_cbBarcodeHRI);

  QVBoxLayout* viewLayout = new QVBoxLayout;
  viewLayout->setContentsMargins(9, 0, 0, 0);
  viewLayout->addLayout(hlayout1);
  viewLayout->addWidget(m_edTitle);
  viewLayout->addWidget(m_teMessage);
  viewLayout->addLayout(hlayout2);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewLayout);

  m_database = new JDatabase(REMINDER_SQL_TABLE_NAME);

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
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  m_dock->close();

  connect(m_edTitle, SIGNAL(textEdited(const QString&)), this, SLOT(emitChangedSignal()));
  connect(m_teMessage, SIGNAL(textChanged()), this, SLOT(emitChangedSignal()));
  connect(m_edBarcode, SIGNAL(textChanged(const QString&)), this, SLOT(emitChangedSignal()));
  connect(m_cbCapitalization, SIGNAL(stateChanged(int)), this, SLOT(setCapitalization(int)));
  connect(m_btnCreate, SIGNAL(clicked(bool)), this, SLOT(create()));
  connect(m_btnSearch, SIGNAL(clicked(bool)), this, SLOT(search()));
  connect(m_database, SIGNAL(itemSelectedSignal(const JItem&)), this, SLOT(itemSelected(const JItem&)));
  connect(m_database, SIGNAL(itemsRemovedSignal(const QVector<Id>&)), this, SLOT(itemsRemoved(const QVector<Id>&)));

  setCapitalization(m_cbCapitalization->checkState());
  create();
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
  r.m_bBarcodeHRI = m_cbBarcodeHRI->isChecked();
  r.m_barcode = m_edBarcode->text();
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
  if (reminder.m_id.isValid())
    setReminder(reminder);
}

void ReminderView::itemsRemoved(const QVector<Id>& ids)
{
  if (ids.contains(m_currentId))
    create();
}

bool ReminderView::save()
{
  return m_database->save(getReminder());
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
  m_cbBarcodeHRI->setChecked(reminder.m_bBarcodeHRI);
  m_edBarcode->setText(reminder.m_barcode);
}

void ReminderView::search()
{
  if (m_dock->isVisible())
    m_dock->close();
  else
    m_dock->show();
}

ReminderPrintDialog::ReminderPrintDialog(QWidget* parent)
  : QDialog(parent)
  , m_spnCopies(nullptr)
  , m_cbSave(nullptr)
{
  m_spnCopies = new JSpinBox;
  m_spnCopies->setMinimum(1);
  m_spnCopies->setMaximum(100);
  m_spnCopies->setSuffix(tr(" cópias"));
  m_cbSave = new QCheckBox;
  m_cbSave->setIcon(QIcon(":/icons/res/save.png"));
  m_cbSave->setText(tr("Salvar lembrete"));
  m_cbSave->setChecked(true);

  QDialogButtonBox* btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
  connect(btn, SIGNAL(rejected()), this, SLOT(reject()));

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addWidget(m_spnCopies);
  lt->addWidget(m_cbSave);
  lt->addWidget(btn);
  lt->setSizeConstraint(QLayout::SetFixedSize);

  setWindowTitle(tr("Imprimir"));
  setWindowIcon(QIcon(":/icons/res/printer.png"));

  setLayout(lt);
  m_spnCopies->setFocus();
  m_spnCopies->selectAll();
}

int ReminderPrintDialog::getCopies() const
{
  return m_spnCopies->value();
}

bool ReminderPrintDialog::getSave() const
{
  return m_cbSave->isChecked();
}

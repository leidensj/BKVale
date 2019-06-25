#include "reminderview.h"
#include "widgets/jlineedit.h"
#include "widgets/jdatabase.h"
#include "widgets/jspinbox.h"
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
  : JItemView(REMINDER_SQL_TABLE_NAME, parent)
  , m_edTitle(nullptr)
  , m_teMessage(nullptr)
  , m_edBarcode(nullptr)
  , m_cbCapitalization(nullptr)
  , m_rdSize1(nullptr)
  , m_rdSize2(nullptr)
  , m_cbFavorite(nullptr)
  , m_cbBarcodeHRI(nullptr)
{
  m_btnSave->setEnabled(false);
  m_btnSave->hide();
  m_edTitle = new JLineEdit(JLineEdit::Input::ASCII, JLineEdit::st_defaultFlags2);
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

  QHBoxLayout* ltHeader = new QHBoxLayout;
  ltHeader->setContentsMargins(0, 0, 0, 0);
  ltHeader->setAlignment(Qt::AlignLeft);
  ltHeader->addWidget(m_cbCapitalization);
  ltHeader->addWidget(vFrame1);
  ltHeader->addWidget(m_rdSize1);
  ltHeader->addWidget(m_rdSize2);
  ltHeader->addWidget(vFrame2);
  ltHeader->addWidget(m_cbFavorite);

  QHBoxLayout* ltFooter = new QHBoxLayout;
  ltFooter->setContentsMargins(0, 0, 0, 0);
  ltFooter->setAlignment(Qt::AlignLeft);
  ltFooter->addWidget(lblBarcode);
  ltFooter->addWidget(m_edBarcode);
  ltFooter->addWidget(m_cbBarcodeHRI);

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addLayout(ltHeader);
  lt->addWidget(m_edTitle);
  lt->addWidget(m_teMessage);
  lt->addLayout(ltFooter);

  QFrame* fr = new QFrame;
  fr->setLayout(lt);

  layout()->setContentsMargins(9, 9, 9, 9);
  m_tab->addTab(fr, QIcon(":/icons/res/postit.png"), tr("Lembrete"));

  connect(m_edTitle, SIGNAL(textEdited(const QString&)), this, SLOT(emitChangedSignal()));
  connect(m_teMessage, SIGNAL(textChanged()), this, SLOT(emitChangedSignal()));
  connect(m_edBarcode, SIGNAL(textChanged(const QString&)), this, SLOT(emitChangedSignal()));
  connect(m_cbCapitalization, SIGNAL(stateChanged(int)), this, SLOT(setCapitalization(int)));

  setCapitalization(m_cbCapitalization->checkState());
  setFocusWidgetOnCreate(m_edTitle);
  create();
}

void ReminderView::getItem(JItemSQL& o) const
{
  Reminder& _o = dynamic_cast<Reminder&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_title = m_edTitle->text();
  _o.m_message = m_teMessage->toPlainText();
  _o.m_bFavorite = m_cbFavorite->isChecked();
  _o.m_size = m_rdSize1->isChecked() ? Reminder::Size::Normal : Reminder::Size::Large;
  switch(m_cbCapitalization->checkState())
  {
    case Qt::CheckState::Unchecked:
      _o.m_capitalization = Reminder::Capitalization::Normal;
      break;
    case Qt::CheckState::PartiallyChecked:
      _o.m_capitalization = Reminder::Capitalization::AllLowercase;
      break;
    case Qt::CheckState::Checked:
    default:
      _o.m_capitalization = Reminder::Capitalization::AllUppercase;
      break;
  }
  _o.m_bBarcodeHRI = m_cbBarcodeHRI->isChecked();
  _o.m_barcode = m_edBarcode->text();
}

void ReminderView::setItem(const JItemSQL& o)
{
  const Reminder& _o = dynamic_cast<const Reminder&>(o);
  m_id = _o.m_id;
  m_edTitle->setText(_o.m_title);
  m_teMessage->setPlainText(_o.m_message);
  m_cbFavorite->setChecked(_o.m_bFavorite);
  m_rdSize2->setChecked(_o.m_size == Reminder::Size::Large);
  m_rdSize1->setChecked(_o.m_size == Reminder::Size::Normal);
  switch(_o.m_capitalization)
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
  m_cbBarcodeHRI->setChecked(_o.m_bBarcodeHRI);
  m_edBarcode->setText(_o.m_barcode);
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

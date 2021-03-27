#include "reminderview.h"
#include "controls/databaseviewer.h"
#include "widgets/jlineedit.h"
#include "widgets/jspinbox.h"
#include "widgets/jdatepicker.h"
#include "widgets/jtimeedit.h"
#include "items/jitemhelper.h"
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QLayout>
#include <QPushButton>
#include <QDockWidget>
#include <QSplitter>
#include <QMessageBox>
#include <QLabel>
#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QInputDialog>

ReminderView::ReminderView(QWidget *parent)
  : JItemView(REMINDER_SQL_TABLE_NAME, parent)
  , m_edTitle(nullptr)
  , m_teMessage(nullptr)
  , m_edBarcode(nullptr)
  , m_cbCapitalization(nullptr)
  , m_cbSize(nullptr)
  , m_cbFavorite(nullptr)
  , m_cbBarcodeHRI(nullptr)
  , m_subject(nullptr)
  , m_date(nullptr)
  , m_cbDate(nullptr)
  , m_time(nullptr)
  , m_cbTime(nullptr)
  , m_btnPin(nullptr)
{
  m_btnPrint->setEnabled(true);
  m_btnPrint->show();
  m_btnPrint->setCheckable(false);
  m_edTitle = new JLineEdit(Text::Input::ASCII, false);
  m_edTitle->setPlaceholderText(tr("Título"));
  m_teMessage = new QPlainTextEdit();
  m_teMessage->setPlaceholderText(tr("Mensagem"));
  m_cbCapitalization = new QCheckBox();
  m_cbCapitalization->setText("");
  m_cbCapitalization->setIconSize(QSize(24, 24));
  m_cbCapitalization->setIcon(QIcon(":/icons/res/uppercase.png"));
  m_cbCapitalization->setTristate(true);
  m_cbCapitalization->setCheckState(Qt::CheckState::Checked);
  m_cbCapitalization->setToolTip(tr("Maiúsculas e minúsculas"));
  m_cbSize = new QCheckBox;
  m_cbSize->setIconSize(QSize(24, 24));
  m_cbSize->setIcon(QIcon(":/icons/res/text.png"));
  m_cbSize->setToolTip(tr("Fonte grande"));
  m_cbSize->setChecked(true);
  m_cbFavorite = new QCheckBox;
  m_cbFavorite->setIconSize(QSize(24, 24));
  m_cbFavorite->setIcon(QIcon(":/icons/res/favorite.png"));
  m_cbFavorite->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F));
  m_cbFavorite->setToolTip(tr("Marcar como favorito"));
  m_edBarcode = new JLineEdit(Text::Input::All, false);
  m_edBarcode->setPlaceholderText(tr("Código de barras"));
  m_edBarcode->setMaxLength(REMINDER_MAX_BARCODE_CODE93_LENGTH);
  m_edBarcode->setClearButtonEnabled(true);
  m_cbBarcodeHRI = new QCheckBox;
  m_cbBarcodeHRI->setText(tr("Incluir HRI"));
  m_subject = new QComboBox;
  JLineEdit* edType = new JLineEdit(Text::Input::ASCII, false);
  edType->setPlaceholderText(tr("Assunto"));
  m_subject->setEditable(true);
  m_subject->setLineEdit(edType);
  m_subject->setSizePolicy(QSizePolicy::QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_cbDate = new QCheckBox;
  m_date = new JDatePicker;
  m_cbTime = new QCheckBox;
  m_time = new JTimeEdit;
  m_btnPin = m_viewer->addButton(tr("Fixar"), QIcon(":/icons/res/favorite.png"));

  QLabel* lblBarcode = new QLabel();
  lblBarcode->setPixmap(QIcon(":/icons/res/barcode.png").pixmap(QSize(24, 24)));
  lblBarcode->setMinimumSize(24, 24);
  lblBarcode->setMaximumSize(24, 24);
  lblBarcode->setScaledContents(true);

  QFrame* vFrame1 = new QFrame;
  vFrame1->setFrameShape(QFrame::VLine);

  QHBoxLayout* ltHeader = new QHBoxLayout;
  ltHeader->setContentsMargins(0, 0, 0, 0);
  ltHeader->addWidget(m_cbCapitalization);
  ltHeader->addWidget(m_cbSize);
  ltHeader->addWidget(m_cbFavorite);
  ltHeader->addWidget(m_cbDate);
  ltHeader->addWidget(m_date);
  ltHeader->addWidget(m_cbTime);
  ltHeader->addWidget(m_time);
  ltHeader->addWidget(vFrame1);
  ltHeader->addWidget(m_subject);

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

  connect(m_edTitle, SIGNAL(textEdited(const QString&)), this, SLOT(updateControls()));
  connect(m_teMessage, SIGNAL(textChanged()), this, SLOT(updateControls()));
  connect(m_edBarcode, SIGNAL(textChanged(const QString&)), this, SLOT(updateControls()));
  connect(m_cbCapitalization, SIGNAL(stateChanged(int)), this, SLOT(updateControls()));
  connect(m_cbDate, SIGNAL(stateChanged(int)), this, SLOT(updateControls()));
  connect(m_cbTime, SIGNAL(stateChanged(int)), this, SLOT(updateControls()));
  connect(m_btnPin, SIGNAL(clicked(bool)), this, SLOT(setFavorite()));
  connect(m_btnPrint, SIGNAL(clicked(bool)), this, SLOT(print()));

  updateControls();
  setFocusWidgetOnClear(m_edTitle);
  clear();
}

void ReminderView::getItem(JItemSQL& o) const
{
  Reminder& _o = dynamic_cast<Reminder&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_title = m_edTitle->text();
  _o.m_message = m_teMessage->toPlainText();
  _o.m_bFavorite = m_cbFavorite->isChecked();
  _o.m_size = m_cbSize->isChecked() ? Reminder::Size::Large : Reminder::Size::Normal;
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
  _o.m_date = m_date->getDate();
  _o.m_bDate = m_cbDate->isChecked();
  _o.m_time = m_time->time();
  _o.m_bTime = m_cbTime->isChecked();
  _o.m_subject = m_subject->currentText();
}

void ReminderView::setItem(const JItemSQL& o)
{
  m_subject->clear();
  m_subject->addItems(Reminder::SQL_select_subjects());

  const Reminder& _o = dynamic_cast<const Reminder&>(o);
  m_id = _o.m_id;
  m_edTitle->setText(_o.m_title);
  m_teMessage->setPlainText(_o.m_message);
  m_cbFavorite->setChecked(_o.m_bFavorite);
  m_cbSize->setChecked(_o.m_size == Reminder::Size::Large);
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
  m_date->setDate(_o.m_date);
  m_cbDate->setChecked(_o.m_bDate);
  m_time->setTime(_o.m_time);
  m_cbTime->setChecked(_o.m_bTime);
  m_subject->setCurrentText(_o.m_subject);
  updateControls();
}

void ReminderView::updateControls()
{
  m_date->setEnabled(m_cbDate->isChecked());
  m_time->setEnabled(m_cbTime->isChecked());

  QFont::Capitalization cap = QFont::AllUppercase;
  switch (m_cbCapitalization->checkState())
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

  QFont f3 = m_subject->lineEdit()->font();
  f3.setCapitalization(cap);
  m_subject->lineEdit()->setFont(f3);

  Reminder o;
  ReminderView::getItem(o);
  m_btnPrint->setEnabled(o.isValid());
  m_btnSave->setEnabled(o.isValid());
}

void ReminderView::setFavorite()
{
  Ids ids(m_viewer->getSelectedIds());
  if (!ids.isEmpty())
  {
    for (int i = 0; i != ids.size(); ++i)
    {
      QString error;
      Reminder o(ids.at(i));
      o.SQL_toggleFavorite(error);
    }
    m_viewer->refresh();
    m_viewer->selectIds(ids);
  }
}

void ReminderView::print()
{
  Reminder o;
  getItem(o);
  bool ok = false;
  int n = QInputDialog::getInt(this, tr("Imprimir Lembrete"), tr("Número de cópias"), 1, 1, 999, 1, &ok);
  if (ok)
    for (int i = 0; i != n; ++i)
      JItemHelper::print(o, nullptr, this);
}

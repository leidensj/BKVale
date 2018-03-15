#include "noteview.h"
#include "databaseutils.h"
#include "notetablewidget.h"
#include "person.h"
#include "jpicker.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QLayout>
#include <QDateEdit>
#include <QSpinBox>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QTimer>
#include <QCheckBox>

NoteView::NoteView(QWidget *parent)
  : QFrame(parent)
  , m_btnCreate(nullptr)
  , m_btnOpenLast(nullptr)
  , m_btnSearch(nullptr)
  , m_btnSearchItem(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_snNumber(nullptr)
  , m_lblNumberStatus(nullptr)
  , m_dtDate(nullptr)
  , m_btnToday(nullptr)
  , m_edTotal(nullptr)
  , m_supplierPicker(nullptr)
  , m_table(nullptr)
  , m_cbCash(nullptr)
  , m_currentID(INVALID_ID)
  , m_lastID(INVALID_ID)
{
  m_btnCreate = new QPushButton();
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
  m_btnCreate->setToolTip(tr("Novo vale (Ctrl+N)"));

  m_btnSearch = new QPushButton();
  m_btnSearch->setFlat(true);
  m_btnSearch->setText("");
  m_btnSearch->setIconSize(QSize(24, 24));
  m_btnSearch->setIcon(QIcon(":/icons/res/search.png"));
  m_btnSearch->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
  m_btnSearch->setToolTip(tr("Procurar vales (Ctrl+F)"));

  m_btnOpenLast = new QPushButton();
  m_btnOpenLast->setFlat(true);
  m_btnOpenLast->setText("");
  m_btnOpenLast->setIconSize(QSize(24, 24));
  m_btnOpenLast->setIcon(QIcon(":/icons/res/openlast.png"));
  m_btnOpenLast->setToolTip(tr("Abrir último vale"));

  QFrame* vFrame0 = new QFrame;
  vFrame0->setFrameShape(QFrame::VLine);

  m_btnSearchItem = new QPushButton();
  m_btnSearchItem->setFlat(true);
  m_btnSearchItem->setText("");
  m_btnSearchItem->setIconSize(QSize(24, 24));
  m_btnSearchItem->setIcon(QIcon(":/icons/res/binoculars.png"));
  m_btnSearchItem->setToolTip(tr("Procurar item (F3)"));
  m_btnSearchItem->setShortcut(QKeySequence(Qt::Key_F3));

  m_btnAdd = new QPushButton();
  m_btnAdd->setFlat(true);
  m_btnAdd->setText("");
  m_btnAdd->setIconSize(QSize(24, 24));
  m_btnAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAdd->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));
  m_btnAdd->setToolTip(tr("Adicionar item (Alt++)"));

  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Minus));
  m_btnRemove->setToolTip(tr("Remover item (Alt+-)"));

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_btnCreate);
  hlayout1->addWidget(m_btnSearch);
  hlayout1->addWidget(m_btnOpenLast);
  hlayout1->addWidget(vFrame0);
  hlayout1->addWidget(m_btnSearchItem);
  hlayout1->addWidget(m_btnAdd);
  hlayout1->addWidget(m_btnRemove);

  QLabel* lblNumber = new QLabel();
  lblNumber->setText(tr("Número:"));
  {
    QFont font = lblNumber->font();
    font.setPointSize(12);
    lblNumber->setFont(font);
  }

  m_snNumber = new QSpinBox();
  m_snNumber->setReadOnly(true);
  m_snNumber->setButtonSymbols(QSpinBox::ButtonSymbols::NoButtons);
  m_snNumber->setMaximum(999999);
  m_snNumber->setMinimum(-1);
  m_snNumber->setMinimumSize(90, 0);
  m_snNumber->setAlignment(Qt::AlignRight);
  m_snNumber->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
  {
    QFont font = m_snNumber->font();
    font.setBold(true);
    m_snNumber->setFont(font);
    QPalette palette = m_snNumber->palette();
    palette.setColor(QPalette::ColorRole::Text, Qt::red);
    m_snNumber->setPalette(palette);
  }

  m_lblNumberStatus = new QLabel();
  m_lblNumberStatus->setText("");
  m_lblNumberStatus->setPixmap(QPixmap(":/icons/res/filenew.png"));
  m_lblNumberStatus->setScaledContents(true);
  m_lblNumberStatus->setMinimumSize(24, 24);
  m_lblNumberStatus->setMaximumSize(24, 24);

  QLabel* lblDate = new QLabel();
  lblDate->setText(tr("Data:"));
  {
    QFont font = lblDate->font();
    font.setPointSize(12);
    lblDate->setFont(font);
  }

  m_dtDate = new QDateEdit();
  m_dtDate->setCalendarPopup(true);
  m_dtDate->setDisplayFormat("dd/MM/yyyy");
  m_dtDate->setDate(QDate::currentDate());

  m_btnToday = new QPushButton();
  m_btnToday->setFlat(true);
  m_btnToday->setText("");
  m_btnToday->setIconSize(QSize(24, 24));
  m_btnToday->setIcon(QIcon(":/icons/res/calendarok.png"));
  m_btnToday->setToolTip(tr("Usar a data de hoje"));

  m_cbCash = new QCheckBox();
  m_cbCash->setText(tr("À vista"));
  m_cbCash->setIconSize(QSize(24,24));
  m_cbCash->setIcon(QIcon(":/icons/res/cash.png"));
  m_cbCash->setToolTip(tr("Imprime apenas uma via, "
                          "com espaço para assinatura "
                          "do fornecedor"));

  QFrame* line1 = new QFrame;
  line1->setFrameShape(QFrame::VLine);

  QFrame* line2 = new QFrame;
  line2->setFrameShape(QFrame::VLine);

  QHBoxLayout* hlayout2 = new QHBoxLayout();
  hlayout2->setContentsMargins(0, 0, 0, 0);
  hlayout2->setAlignment(Qt::AlignLeft);
  hlayout2->addWidget(lblNumber);
  hlayout2->addWidget(m_snNumber);
  hlayout2->addWidget(m_lblNumberStatus);
  hlayout2->addWidget(line1);
  hlayout2->addWidget(lblDate);
  hlayout2->addWidget(m_dtDate);
  hlayout2->addWidget(m_btnToday);
  hlayout2->addWidget(line2);
  hlayout2->addWidget(m_cbCash);

  m_supplierPicker = new JPicker(tr("Fornecedor"), true, true, false);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->addLayout(hlayout2);
  vlayout1->addWidget(m_supplierPicker);

  QFrame* frame = new QFrame();
  frame->setFrameShape(QFrame::Shape::StyledPanel);
  frame->setFrameShadow(QFrame::Shadow::Plain);
  frame->setLayout(vlayout1);

  m_table = new NoteTableWidget();

  m_edTotal = new QLineEdit();
  m_edTotal->setReadOnly(true);
  m_edTotal->setPlaceholderText(tr("TOTAL"));
  m_edTotal->setAlignment(Qt::AlignRight);
  m_edTotal->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  {
    QFont font = m_edTotal->font();
    font.setBold(true);
    m_edTotal->setFont(font);
    QPalette palette = m_edTotal->palette();
    palette.setColor(QPalette::ColorRole::Text, Qt::red);
    m_edTotal->setPalette(palette);
  }

  QHBoxLayout* hlayout3 = new QHBoxLayout();
  hlayout3->setContentsMargins(0, 0, 0, 0);
  hlayout3->setAlignment(Qt::AlignRight);
  hlayout3->addWidget(m_edTotal);

  QVBoxLayout* vlayout2 = new QVBoxLayout();
  vlayout2->setContentsMargins(0, 0, 0, 0);
  vlayout2->addLayout(hlayout1);
  vlayout2->addWidget(frame);
  vlayout2->addWidget(m_table);
  vlayout2->addLayout(hlayout3);
  setLayout(vlayout2);

  QObject::connect(m_supplierPicker,
                   SIGNAL(searchSignal()),
                   this,
                   SLOT(emitSearchSupplierSignal()));

  QObject::connect(m_btnSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitShowSearchSignal()));

  QObject::connect(m_btnAdd,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSearchNewProductSignal()));

  QObject::connect(m_btnSearchItem,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSearchProductSignal()));

  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeItem()));

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitCreateSignal()));

  QObject::connect(m_table,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));

  QObject::connect(m_btnOpenLast,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitOpenLastSignal()));

  QObject::connect(m_dtDate,
                   SIGNAL(dateChanged(const QDate&)),
                   this,
                   SLOT(updateControls()));

  QObject::connect(m_btnToday,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(setToday()));

  QObject::connect(m_btnToday,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(setToday()));

  QObject::connect(m_cbCash,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));

  QTimer *timer = new QTimer(this);
  QObject::connect(timer,
                   SIGNAL(timeout()),
                   this,
                   SLOT(checkDate()));
  timer->start(60000);

  create(NOTE_SQL_DEFAULT_NUMBER);
  checkDate();
  updateControls();
}

NoteView::~NoteView()
{

}

void NoteView::setProduct(int row, const Product& product)
{
  m_table->setProduct(row, product);
}

void NoteView::addNoteItem(const NoteItem& noteItem)
{
  m_table->addNoteItem(noteItem);
  m_table->setFocus();
  updateControls();
}

void NoteView::removeItem()
{
  m_table->removeCurrentItem();
  if (!m_table->hasItems())
    m_supplierPicker->setFocus();
  updateControls();
}

Note NoteView::getNote() const
{
  Note note;
  note.m_id = m_currentID;
  note.m_date = m_dtDate->date().toJulianDay();
  note.m_supplier.m_id = m_supplierPicker->getId();
  note.m_total = m_edTotal->text().toDouble();
  note.m_bCash = m_cbCash->isChecked();
  note.m_vNoteItem = m_table->getNoteItems();
  return note;
}

void NoteView::setNote(const Note& note)
{
  m_table->removeAllItems();
  m_supplierPicker->clear();
  m_currentID = note.m_id;
  m_dtDate->setDate(QDate::fromString(note.m_date, Qt::ISODate));
  m_supplierPicker->setText(note.m_supplier.m_alias);
  m_snNumber->setValue(note.m_number);
  m_cbCash->setChecked(note.m_bCash);
  m_table->setNoteItems(note.m_vNoteItem);
  updateControls();
}

void NoteView::create(int number)
{
  m_currentID = INVALID_ID;
  m_dtDate->setDate(QDate::currentDate());
  m_snNumber->setValue(number);
  m_edTotal->setText("");
  m_table->removeAllItems();
  m_supplierPicker->clear();
  m_supplierPicker->setFocus();
  m_cbCash->setChecked(false);
  updateControls();
}

void NoteView::emitSearchSupplierSignal()
{
  emit searchSupplierSignal();

}

void NoteView::setSupplier(const Person& supplier)
{
  m_supplierPicker->setId(supplier.m_id);
  m_supplierPicker->setText(supplier.m_alias);
  m_supplierPicker->setImage(supplier.m_image.m_image);
  if (m_table->hasItems() != 0)
  {
    m_table->setCurrentCell(0, 0);
    m_table->setFocus();
  }
  else
  {
    emit searchProductSignal(-1);
  }
}

void NoteView::updateControls()
{
  // TODO
  const bool bCreated = m_snNumber->value() > 0;
  m_btnRemove->setEnabled(bCreated && m_table->currentRow() >= 0);
  m_btnSearchItem->setEnabled(bCreated && m_table->currentRow() >= 0);
  m_btnAdd->setEnabled(bCreated);
  m_supplierPicker->setEnabled(bCreated);
  m_snNumber->setEnabled(bCreated);
  m_dtDate->setEnabled(bCreated);
  m_table->setEnabled(bCreated);
  m_edTotal->setEnabled(bCreated);
  m_cbCash->setEnabled(bCreated);
  m_btnOpenLast->setEnabled(m_lastID != INVALID_ID);
  m_lblNumberStatus->setPixmap(QPixmap(IS_VALID_ID(m_currentID)
                                     ? ":/icons/res/fileedit.png"
                                     : ":/icons/res/filenew.png"));
  m_btnToday->setIcon(QIcon(m_dtDate->date() == QDate::currentDate()
                            ? ":/icons/res/calendarok.png"
                            : ":/icons/res/calendarwarning.png"));
  if (m_table->hasItems())
    m_edTotal->setText(m_table->computeTotal());
  else
  m_edTotal->clear();

  emit changedSignal();
}

void NoteView::emitShowSearchSignal()
{
  emit showSearchSignal();
}

void NoteView::emitCreateSignal()
{
  emit createSignal();
}

void NoteView::emitOpenLastSignal()
{
  if (m_lastID != INVALID_ID)
    emit openLastSignal(m_lastID);
}

void NoteView::setLastID(int lastID)
{
  m_lastID = lastID;
  updateControls();
}

int NoteView::getLastID() const
{
  return m_lastID;
}

void NoteView::checkDate()
{
  QTextCharFormat fmt;
  m_dtDate->calendarWidget()->setDateTextFormat(QDate::currentDate().addDays(-1), fmt);
  fmt.setForeground(Qt::darkGreen);
  fmt.setFontWeight(QFont::ExtraBold);
  fmt.setFontUnderline(true);
  fmt.setFontOverline(true);
  m_dtDate->calendarWidget()->setDateTextFormat(QDate::currentDate(), fmt);

  bool bIsEditMode = IS_VALID_ID(m_currentID);
  bool bIsDirty = !m_supplierPicker->getText().isEmpty() ||
                  m_table->hasItems();
  if (!bIsEditMode && !bIsDirty)
    setToday();
}

void NoteView::setToday()
{
  m_dtDate->setDate(QDate::currentDate());
  updateControls();
}

void NoteView::emitSearchNewProductSignal()
{
  emit searchProductSignal(-1);
}

void NoteView::emitSearchProductSignal()
{
  emit searchProductSignal(m_table->currentRow());
}

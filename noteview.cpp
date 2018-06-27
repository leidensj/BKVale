#include "noteview.h"
#include "databaseutils.h"
#include "notetablewidget.h"
#include "person.h"
#include "jdatabasepicker.h"
#include "jdatabase.h"
#include "jlineedit.h"
#include "tinyexpr.h"
#include "packageeditor.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QLayout>
#include <QDateEdit>
#include <QSpinBox>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QTimer>
#include <QCheckBox>
#include <QSplitter>
#include <QDockWidget>
#include <QMessageBox>
#include <QPlainTextEdit>

#define DISCCOUNT_LAST_VALUE_PROP "lastValue"

NoteView::NoteView(QWidget *parent)
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_lastId(INVALID_ID)
  , m_btnCreate(nullptr)
  , m_btnOpenLast(nullptr)
  , m_btnSearch(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_snNumber(nullptr)
  , m_dtDate(nullptr)
  , m_btnToday(nullptr)
  , m_edTotal(nullptr)
  , m_supplierPicker(nullptr)
  , m_table(nullptr)
  , m_cbCash(nullptr)
  , m_teObservation(nullptr)
  , m_edDisccount(nullptr)
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
  m_snNumber->setMaximum(99999999);
  m_snNumber->setMinimum(0);
  m_snNumber->setSpecialValueText(tr("S/N"));
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
  hlayout2->addWidget(line1);
  hlayout2->addWidget(lblDate);
  hlayout2->addWidget(m_dtDate);
  hlayout2->addWidget(m_btnToday);
  hlayout2->addWidget(line2);
  hlayout2->addWidget(m_cbCash);

  m_supplierPicker = new JDatabasePicker(PERSON_SQL_TABLE_NAME,
                                         tr("Fornecedor"),
                                         QIcon(":/icons/res/supplier.png"),
                                         true,
                                         false,
                                         false);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->addLayout(hlayout2);
  vlayout1->addWidget(m_supplierPicker);

  QFrame* frame = new QFrame();
  frame->setFrameShape(QFrame::Shape::StyledPanel);
  frame->setFrameShadow(QFrame::Shadow::Plain);
  frame->setLayout(vlayout1);
  frame->setFixedHeight(frame->sizeHint().height());

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

  m_edDisccount = new JLineEdit(JLineEdit::Input::BasicMath, 0);
  m_edDisccount->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  m_edDisccount->setAlignment(Qt::AlignRight);
  m_edDisccount->setProperty(DISCCOUNT_LAST_VALUE_PROP, 0.0);

  QHBoxLayout* totalLayout = new QHBoxLayout;
  totalLayout->setContentsMargins(0, 0, 0, 0);
  totalLayout->addWidget(new QLabel(tr("Descontos/Acréscimos:")));
  totalLayout->addWidget(m_edDisccount);
  totalLayout->addStretch();
  totalLayout->addWidget(m_edTotal);

  m_teObservation = new QPlainTextEdit;
  m_teObservation->setPlaceholderText(tr("Observações (opcional):"));
  m_teObservation->setMaximumHeight(frame->sizeHint().height());

  QHBoxLayout* headerlayout = new QHBoxLayout;
  headerlayout->setContentsMargins(0, 0, 0, 0);
  headerlayout->addWidget(frame);
  headerlayout->addWidget(m_teObservation);

  QVBoxLayout* viewLayout = new QVBoxLayout();
  viewLayout->setContentsMargins(9, 0, 0, 0);
  viewLayout->addLayout(hlayout1);
  viewLayout->addLayout(headerlayout);
  viewLayout->addWidget(m_table);
  viewLayout->addLayout(totalLayout);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewLayout);

  m_database = new JDatabase(NOTE_SQL_TABLE_NAME);

  m_dock = new QDockWidget();
  m_dock->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  m_dock->setFeatures(0);
  m_dock->setFeatures(QDockWidget::DockWidgetClosable);
  m_dock->setWindowTitle(tr("Pesquisar"));
  m_dock->setWidget(m_database);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_dock);
  splitter->addWidget(viewFrame);

  QVBoxLayout* mainLayout = new QVBoxLayout();
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));
  QObject::connect(m_database,
                   SIGNAL(itemRemovedSignal(qlonglong)),
                   this,
                   SLOT(itemRemoved(qlonglong)));
  QObject::connect(m_btnSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(showSearch()));
  QObject::connect(m_btnAdd,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(searchProduct()));
  QObject::connect(m_table,
                   SIGNAL(productSignal(const Product&)),
                   this,
                   SLOT(searchProduct()));
  QObject::connect(m_table,
                   SIGNAL(packageSignal(const Package&, const QString&)),
                   this,
                   SLOT(editPackage(const Package&, const QString&)));
  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeItem()));
  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
  QObject::connect(m_table,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_btnOpenLast,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(lastItemSelected()));
  QObject::connect(m_dtDate,
                   SIGNAL(dateChanged(const QDate&)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_btnToday,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(setToday()));
  QObject::connect(m_cbCash,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_supplierPicker,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(supplierChanged()));
  QObject::connect(m_edDisccount,
                   SIGNAL(editingFinished()),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_edDisccount,
                   SIGNAL(enterSignal()),
                   m_table,
                   SLOT(setFocus()));

  QTimer *timer = new QTimer(this);
  QObject::connect(timer,
                   SIGNAL(timeout()),
                   this,
                   SLOT(checkDate()));
  timer->start(60000);

  create();
  checkDate();
  m_dock->close();
  updateControls();

  m_supplierPicker->getDatabase()->setCustomFilter(PERSON_FILTER_SUPPLIER);
}

NoteView::~NoteView()
{

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
  note.m_id = m_currentId;
  note.m_date = m_dtDate->date().toString(Qt::ISODate);
  note.m_supplier.m_id = m_supplierPicker->getId();
  note.m_bCash = m_cbCash->isChecked();
  note.m_observation = m_teObservation->toPlainText();
  note.m_disccount = m_edDisccount->text().toDouble();
  note.m_vNoteItem = m_table->getNoteItems();
  return note;
}

void NoteView::setNote(const Note& note)
{
  m_currentId = note.m_id;
  m_table->removeAllItems();
  m_supplierPicker->clear();
  m_dtDate->setDate(QDate::fromString(note.m_date, Qt::ISODate));
  m_snNumber->setValue(note.m_number);
  m_cbCash->setChecked(note.m_bCash);
  m_table->setNoteItems(note.m_vNoteItem);
  m_supplierPicker->setItem(note.m_supplier);
  m_teObservation->setPlainText(note.m_observation);
  m_edDisccount->setText(note.strDisccount());
  updateControls();
}

void NoteView::create()
{
  Note note;
  setNote(note);
  updateControls();
}

void NoteView::supplierChanged()
{
  if (IS_VALID_ID(m_supplierPicker->getId()))
  {
    if (m_table->hasItems())
    {
      m_table->setCurrentCell(0, 0);
      m_table->setFocus();
    }
    else if (!IS_VALID_ID(m_currentId))
    {
      searchProduct();
    }
  }
  updateControls();
}

void NoteView::updateControls()
{
  m_btnRemove->setEnabled(m_table->currentRow() >= 0);
  m_btnOpenLast->setEnabled(IS_VALID_ID(m_lastId));
  m_btnToday->setIcon(QIcon(m_dtDate->date() == QDate::currentDate()
                            ? ":/icons/res/calendarok.png"
                            : ":/icons/res/calendarwarning.png"));

  // evaluate disccount
  auto exp = m_edDisccount->text().toStdString();
  int error = 0;
  double disccount = te_interp(exp.c_str(), &error);
  if (!error)
    m_edDisccount->setProperty(DISCCOUNT_LAST_VALUE_PROP, disccount);
  else
    m_edDisccount->setProperty(DISCCOUNT_LAST_VALUE_PROP, 0.0);
  disccount = m_edDisccount->property(DISCCOUNT_LAST_VALUE_PROP).toDouble();
  m_edDisccount->setText(JItem::st_strMoney(disccount));
  QPalette palette = m_edDisccount->palette();
  palette.setColor(QPalette::ColorRole::Text,
                   disccount >= 0 ? Qt::red : Qt::darkGreen);
  m_edDisccount->setPalette(palette);

  double total = m_table->computeTotal() + disccount;

  if (m_table->hasItems() || total != 0)
    m_edTotal->setText(JItem::st_strMoney(total));
  else
    m_edTotal->clear();

  palette.setColor(QPalette::ColorRole::Text,
                   total >= 0 ? Qt::red : Qt::darkGreen);
  m_edTotal->setPalette(palette);

  emit changedSignal();
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

  bool bIsEditMode = IS_VALID_ID(m_currentId);
  bool bIsDirty = IS_VALID_ID(m_supplierPicker->getId()) || m_table->hasItems();
  if (!bIsEditMode && !bIsDirty)
    setToday();
}

void NoteView::setToday()
{
  m_dtDate->setDate(QDate::currentDate());
  updateControls();
}

void NoteView::searchProduct()
{
  JDatabaseSelector dlg(PRODUCT_SQL_TABLE_NAME,
                        tr("Selecionar Produto"),
                        QIcon(":/icons/res/item.png"));
  dlg.getDatabase()->setCustomFilter(PRODUCT_FILTER_NOTE);
  dlg.exec();
  Product* pProduct = static_cast<Product*>(dlg.getDatabase()->getCurrentItem());
  if (pProduct != nullptr && pProduct->isValidId())
  {
    if (m_btnAdd == sender() || !m_table->hasItems())
    {
      NoteItem noteItem;
      if (IS_VALID_ID(m_supplierPicker->getId()))
      {
        noteItem = NoteSQL::selectLastItem(m_supplierPicker->getId(),
                                           pProduct->m_id);
        noteItem.m_ammount = 0.0;
      }
      noteItem.m_product = *pProduct;
      addNoteItem(noteItem);
    }
    else
    {
      m_table->setProduct(*pProduct);
    }
  }
}

void NoteView::editPackage(const Package& package, const QString& productUnity)
{
  PackageEditor dlg(package, productUnity);
  if (dlg.exec())
  {
    m_table->setPackage(dlg.getPackage());
  }
}

void NoteView::showSearch()
{
  if (m_dock->isVisible())
    m_dock->close();
  else
    m_dock->show();
}

Note NoteView::save()
{
  Note note = getNote();
  bool bSuccess = m_database->save(note);
  if (bSuccess)
  {
    m_lastId = note.m_id;
    QString error;
    //TODO quickfix
    NoteSQL::select(note, error);
    create();
  }
  else
    note.clear();
  updateControls();
  return note;
}

void NoteView::lastItemSelected()
{
  if (m_lastId != INVALID_ID)
  m_database->selectItem(m_lastId);
}

void NoteView::itemSelected(const JItem& jItem)
{
  const Note& note = dynamic_cast<const Note&>(jItem);
  m_lastId = note.m_id;
  setNote(note);
}

void NoteView::itemRemoved(qlonglong id)
{
  if (id == m_lastId)
    m_lastId = INVALID_ID;
  if (id == m_currentId)
    create();
}

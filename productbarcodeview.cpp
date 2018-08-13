#include "productbarcodeview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "jdatabase.h"
#include <QLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QSplitter>
#include <QFormLayout>

ProductBarcodeView::ProductBarcodeView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_productPicker(nullptr)
  , m_edCode(nullptr)
  , m_database(nullptr)
  , m_tab(nullptr)
{
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

  m_btnSave = new QPushButton;
  m_btnSave->setFlat(true);
  m_btnSave->setText("");
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

  m_productPicker = new JDatabasePicker(PRODUCT_SQL_TABLE_NAME,
                                         tr("Produto"),
                                         QIcon(":/icons/res/item.png"),
                                         true,
                                         false,
                                         false);

  m_edCode = new JLineEdit(JLineEdit::Input::All,
                           JLineEdit::st_defaultFlags1);
  m_edCode->setPlaceholderText("*");

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->setAlignment(Qt::AlignLeft);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSave);

  QFormLayout* codeLayout = new QFormLayout;
  codeLayout->addRow(tr("Código:"), m_edCode);

  QVBoxLayout* tabInfoLayout = new QVBoxLayout;
  tabInfoLayout->setAlignment(Qt::AlignTop);
  tabInfoLayout->addWidget(m_productPicker);
  tabInfoLayout->addLayout(codeLayout);

  QFrame* tabInfoFrame = new QFrame;
  tabInfoFrame->setLayout(tabInfoLayout);

  m_tab = new QTabWidget;

  m_tab->addTab(tabInfoFrame,
                QIcon(":/icons/res/details.png"),
                tr("Informações"));

  QVBoxLayout* viewlayout = new QVBoxLayout;
  viewlayout->setContentsMargins(9, 0, 0, 0);
  viewlayout->setAlignment(Qt::AlignTop);
  viewlayout->addLayout(buttonlayout);
  viewlayout->addWidget(m_tab);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewlayout);

  m_database = new JDatabase(PRODUCT_BARCODE_SQL_TABLE_NAME);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(viewFrame);

  QVBoxLayout* mainLayout = new QVBoxLayout();
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save()));
  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));
  QObject::connect(m_database,
                   SIGNAL(itemRemovedSignal(qlonglong)),
                   this,
                   SLOT(itemRemoved(qlonglong)));
}

ProductBarcodeView::~ProductBarcodeView()
{

}

ProductBarcode ProductBarcodeView::getProductBarcode() const
{
  ProductBarcode barcode;
  barcode.m_id = m_currentId;
  barcode.m_product.m_id = m_productPicker->getId();
  barcode.m_code = m_edCode->text();
  return barcode;
}

void ProductBarcodeView::setProductBarcode(const ProductBarcode &barcode)
{
  QString strIcon = barcode.isValidId()
                    ? ":/icons/res/saveas.png"
                    : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(strIcon));
  m_currentId = barcode.m_id;
  m_productPicker->setItem(barcode.m_product);
  m_edCode->setText(barcode.m_code);
}

void ProductBarcodeView::create()
{
  m_tab->setCurrentIndex(0);
  ProductBarcode barcode;
  setProductBarcode(barcode);
}

void ProductBarcodeView::itemSelected(const JItem& jItem)
{
  const ProductBarcode& barcode = dynamic_cast<const ProductBarcode&>(jItem);
  if (barcode.isValidId())
    setProductBarcode(barcode);
}

void ProductBarcodeView::itemRemoved(qlonglong id)
{
  if (id == m_currentId)
    create();
}

void ProductBarcodeView::save()
{
  if (m_database->save(getProductBarcode()))
    create();
}

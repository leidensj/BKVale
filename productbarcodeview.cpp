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
  : JItemView(PRODUCT_BARCODE_SQL_TABLE_NAME, parent)
  , m_productPicker(nullptr)
  , m_edCode(nullptr)
{
  m_productPicker = new JDatabasePicker(PRODUCT_SQL_TABLE_NAME,
                                         tr("Produto"),
                                         QIcon(":/icons/res/item.png"),
                                         JDatabasePicker::Flags::TextGroup);

  m_edCode = new JLineEdit(JLineEdit::Input::All,
                           JLineEdit::st_defaultFlags1);
  m_edCode->setPlaceholderText("*");

  QFormLayout* codeLayout = new QFormLayout;
  codeLayout->addRow(tr("Código:"), m_edCode);

  QVBoxLayout* tabInfoLayout = new QVBoxLayout;
  tabInfoLayout->setAlignment(Qt::AlignTop);
  tabInfoLayout->addWidget(m_productPicker);
  tabInfoLayout->addLayout(codeLayout);

  QFrame* tabInfoFrame = new QFrame;
  tabInfoFrame->setLayout(tabInfoLayout);

  m_tab->addTab(tabInfoFrame,
                QIcon(":/icons/res/barcode.png"),
                tr("Código"));
}

ProductBarcodeView::~ProductBarcodeView()
{

}

const JItem& ProductBarcodeView::getItem() const
{
  static ProductBarcode o;
  o.m_id = m_currentId;
  o.m_product.m_id = m_productPicker->getId();
  o.m_code = m_edCode->text();
  return o;
}

void ProductBarcodeView::setItem(const JItem &o)
{
  auto _o = dynamic_cast<const ProductBarcode&>(o);
  m_currentId = _o.m_id;
  m_productPicker->setItem(_o.m_product);
  m_edCode->setText(_o.m_code);
}

void ProductBarcodeView::create()
{
  selectItem(ProductBarcode());
  m_tab->setCurrentIndex(0);
  m_edCode->setFocus();
}

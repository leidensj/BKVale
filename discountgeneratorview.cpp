#include "discountgeneratorview.h"
#include "jlineedit.h"
#include "jdoublespinbox.h"
#include "jdatabase.h"
#include "jspinbox.h"
#include "discounttablewidget.h"
#include <QGroupBox>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QMessageBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QDateEdit>
#include <QPlainTextEdit>

DiscountGeneratorView::DiscountGeneratorView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_edCode(nullptr)
  , m_cbExpires(nullptr)
  , m_dtExp(nullptr)
  , m_rdValue(nullptr)
  , m_rdPercentage(nullptr)
  , m_rdProduct(nullptr)
  , m_spnValue(nullptr)
  , m_spnPercentage(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_table(nullptr)
  , m_database(nullptr)
  , m_cbUsed(nullptr)
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

  m_edCode = new JLineEdit(JLineEdit::Input::Alphanumeric, JLineEdit::st_defaultFlags2);
  m_edCode->setPlaceholderText(tr("*"));
  m_edCode->setMaxLength(DISCOUNT_CODE_MAX_LENGTH);
  m_edCode->setReadOnly(true);

  m_cbExpires = new QCheckBox;
  m_cbExpires->setText(tr("Expira em:"));

  m_dtExp = new QDateEdit;
  m_dtExp->setCalendarPopup(true);
  m_dtExp->setDate(QDate::currentDate());

  m_rdValue = new QRadioButton;
  m_rdValue->setText(tr("Valor"));

  m_rdPercentage = new QRadioButton;
  m_rdPercentage->setText(tr("Porcentagem"));

  m_rdProduct = new QRadioButton;
  m_rdProduct->setText(tr("Produto"));

  m_spnValue = new JDoubleSpinBox;
  m_spnValue->setMinimum(0.0);
  m_spnValue->setMaximum(99999999.0);

  m_spnPercentage = new JDoubleSpinBox;
  m_spnPercentage->setMinimum(0.0);
  m_spnPercentage->setMaximum(100.0);
  m_spnPercentage->setSuffix("%");

  m_btnAdd = new QPushButton;
  m_btnAdd->setFlat(true);
  m_btnAdd->setText("");
  m_btnAdd->setIconSize(QSize(24, 24));
  m_btnAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAdd->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));
  m_btnAdd->setToolTip(tr("Adicionar item (Alt++)"));

  m_btnRemove = new QPushButton;
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Minus));
  m_btnRemove->setToolTip(tr("Remover item (Alt+-)"));

  m_table = new DiscountTableWidget;

  m_cbUsed = new QCheckBox;
  m_cbUsed->setText(tr("Desconto já usado."));
  m_cbUsed->setEnabled(false);

  m_teDescription = new QPlainTextEdit;
  m_teDescription->setPlaceholderText(tr("Descrição:"));

  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->setContentsMargins(0, 0, 0, 0);
  buttonLayout->addWidget(m_btnCreate);
  buttonLayout->addWidget(m_btnSave);
  buttonLayout->setAlignment(Qt::AlignLeft);

  QFormLayout* codeLayout = new QFormLayout;
  codeLayout->setContentsMargins(0, 0, 0, 0);
  codeLayout->addRow(tr("Código:"), m_edCode);

  QHBoxLayout* expLayout = new QHBoxLayout;
  expLayout->setContentsMargins(0, 0, 0, 0);
  expLayout->addWidget(m_cbExpires);
  expLayout->addWidget(m_dtExp);

  QHBoxLayout* valueLayout = new QHBoxLayout;
  valueLayout->setContentsMargins(0, 0, 0, 0);
  valueLayout->setAlignment(Qt::AlignLeft);
  valueLayout->addWidget(m_rdValue);
  valueLayout->addStretch();
  valueLayout->addWidget(m_spnValue);

  QHBoxLayout* percentageLayout = new QHBoxLayout;
  percentageLayout->setContentsMargins(0, 0, 0, 0);
  percentageLayout->setAlignment(Qt::AlignLeft);
  percentageLayout->addWidget(m_rdPercentage);
  percentageLayout->addStretch();
  percentageLayout->addWidget(m_spnPercentage);

  QHBoxLayout* productButtonsLayout = new QHBoxLayout;
  productButtonsLayout->setContentsMargins(0, 0, 0, 0);
  productButtonsLayout->setAlignment(Qt::AlignLeft);
  productButtonsLayout->addWidget(m_btnAdd);
  productButtonsLayout->addWidget(m_btnRemove);

  QVBoxLayout* productLayout = new QVBoxLayout;
  productLayout->setContentsMargins(0, 0, 0, 0);
  productLayout->setAlignment(Qt::AlignTop);
  productLayout->addWidget(m_rdProduct);
  productLayout->addLayout(productButtonsLayout);
  productLayout->addWidget(m_table);

  QVBoxLayout* tabLayout = new QVBoxLayout;
  tabLayout->setAlignment(Qt::AlignTop);
  tabLayout->addWidget(m_cbUsed);
  tabLayout->addLayout(codeLayout);
  tabLayout->addLayout(expLayout);
  tabLayout->addWidget(m_teDescription);
  tabLayout->addLayout(valueLayout);
  tabLayout->addLayout(percentageLayout);
  tabLayout->addLayout(productLayout);

  QFrame* tabFrame = new QFrame;
  tabFrame->setLayout(tabLayout);

  QTabWidget* tabWidget = new QTabWidget;
  tabWidget->addTab(tabFrame,
                    QIcon(":/icons/res/description.png"),
                    tr("Informações"));

  QVBoxLayout* viewLayout = new QVBoxLayout;
  viewLayout->setAlignment(Qt::AlignTop);
  viewLayout->addLayout(buttonLayout);
  viewLayout->addWidget(tabWidget);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewLayout);

  m_database = new JDatabase(DISCOUNT_SQL_TABLE_NAME);

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
  QObject::connect(m_rdValue,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_rdPercentage,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_rdProduct,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbExpires,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_btnAdd,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(searchProduct()));
  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeProduct()));
  QObject::connect(m_table,
                   SIGNAL(productSignal(const Product&)),
                   this,
                   SLOT(searchProduct()));
  create();
}

void DiscountGeneratorView::itemSelected(const JItem& jItem)
{
  const Discount& o = dynamic_cast<const Discount&>(jItem);
  if (o.isValidId())
    setDiscount(o);
}

void DiscountGeneratorView::itemRemoved(qlonglong id)
{
  if (m_currentId == id)
    create();
}

void DiscountGeneratorView::save()
{
  Discount o = getDiscount();
  if (m_database->save(o))
    create();
}

void DiscountGeneratorView::create()
{
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_currentId = INVALID_ID;
  setDiscount(Discount());
}

Discount DiscountGeneratorView::getDiscount() const
{
  Discount o;
  o.m_id = m_currentId;
  o.m_code = m_edCode->text();
  o.m_bExpires = m_cbExpires->isChecked();
  o.m_dtExp = m_dtExp->date();
  o.m_type = m_rdPercentage->isChecked()
             ? Discount::Type::Percentage
             : m_rdValue->isChecked()
             ? Discount::Type::Value
             : m_rdProduct->isChecked()
             ? Discount::Type::Product
             : Discount::Type::None;
  o.m_value = m_spnValue->value();
  o.m_percentage = m_spnPercentage->value();
  o.m_items = m_table->getDiscountItems();
  o.m_bUsed = m_cbUsed->isChecked();
  o.m_description = m_teDescription->toPlainText();
  return o;
}

void DiscountGeneratorView::setDiscount(const Discount &o)
{
  QString strIcon = o.isValidId()
                    ? ":/icons/res/saveas.png"
                    : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(strIcon));
  m_currentId = o.m_id;
  m_cbUsed->setChecked(o.m_bUsed);
  m_cbUsed->setVisible(o.m_bUsed);
  m_edCode->setText(o.m_code);
  m_cbExpires->setChecked(o.m_bExpires);
  m_dtExp->setDate(o.m_dtExp);
  m_rdValue->setChecked(o.m_type == Discount::Type::Value);
  m_rdPercentage->setChecked(o.m_type == Discount::Type::Percentage);
  m_rdProduct->setChecked(o.m_type == Discount::Type::Product);
  m_spnValue->setValue(o.m_value);
  m_spnPercentage->setValue(o.m_percentage);
  m_table->setDiscountItems(o.m_items);
  m_teDescription->setPlainText(o.m_description);
  updateControls();
}

void DiscountGeneratorView::updateControls()
{
  m_spnValue->setEnabled(m_rdValue->isChecked());
  m_spnPercentage->setEnabled(m_rdPercentage->isChecked());
  m_btnAdd->setEnabled(m_rdProduct->isChecked());
  m_btnRemove->setEnabled(m_rdProduct->isChecked());
  m_table->setEnabled(m_rdProduct->isChecked());
  m_dtExp->setEnabled(m_cbExpires->isChecked());
  if (sender() == m_rdValue)
  {
    m_spnValue->setFocus();
    m_spnValue->selectAll();
  }
  if (sender() == m_rdPercentage)
  {
    m_spnPercentage->setFocus();
    m_spnPercentage->selectAll();
  }
}

void DiscountGeneratorView::setProduct(const Product& product, bool bNewProduct)
{
  if (bNewProduct)
  {
    DiscountItem o;
    o.m_product = product;
    m_table->addDiscountItem(o);
    m_table->setFocus();
  }
  else
  {
    m_table->setProduct(product);
  }
  updateControls();
}

void DiscountGeneratorView::searchProduct()
{
  JDatabaseSelector dlg(PRODUCT_SQL_TABLE_NAME,
                        tr("Selecionar Produto"),
                        QIcon(":/icons/res/item.png"));
  dlg.getDatabase()->setFixedFilter(PRODUCT_FILTER_SELL);
  dlg.exec();
  Product* p = static_cast<Product*>(dlg.getDatabase()->getCurrentItem());
  if (p != nullptr && p->isValidId())
    setProduct(*p, m_btnAdd == sender());
}

void DiscountGeneratorView::removeProduct()
{
  m_table->removeCurrentItem();
  updateControls();
}

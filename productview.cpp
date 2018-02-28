#include "productview.h"
#include "jlineedit.h"
#include "jpicker.h"
#include <QLayout>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QTabWidget>

ProductView::ProductView(QWidget* parent)
  : QFrame(parent)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_edName(nullptr)
  , m_edUnity(nullptr)
  , m_edPackageUnity(nullptr)
  , m_spnPackageAmmount(nullptr)
  , m_edDetails(nullptr)
  , m_edCode(nullptr)
  , m_cbAvailableAtNotes(nullptr)
  , m_cbAvailableAtShop(nullptr)
  , m_cbAvailableAtConsumption(nullptr)
  , m_cbAvailableToBuy(nullptr)
  , m_cbAvailableToSell(nullptr)
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

  m_edName = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edName->setMaxLength(MAX_PRODUCT_NAME_LENGTH);
  m_edName->setPlaceholderText("*");

  m_edUnity = new JLineEdit(JValidatorType::Alphanumeric, true, true);
  m_edUnity->setMaxLength(MAX_PRODUCT_UNITY_LENGTH);
  m_edUnity->setPlaceholderText("*");

  m_edPackageUnity = new JLineEdit(JValidatorType::Alphanumeric, true, true);
  m_edPackageUnity->setMaxLength(MAX_PRODUCT_PACKAGE_UNITY_LENGTH);

  m_spnPackageAmmount = new QDoubleSpinBox();
  m_spnPackageAmmount->setMaximum(MAX_PRODUCT_PACKAGE_AMMOUNT_LENGTH);
  m_spnPackageAmmount->setMinimum(0.0);
  m_spnPackageAmmount->setSingleStep(0.1);

  m_edDetails = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edDetails->setMaxLength(MAX_PRODUCT_DETAILS_LENGTH);

  m_edCode = new JLineEdit(JValidatorType::Numeric, true, true);
  m_edCode->setMaxLength(MAX_PRODUCT_MIDASCODE_LENGTH);

  m_cbAvailableAtNotes = new QCheckBox;
  m_cbAvailableAtNotes->setText(tr("Vales"));
  m_cbAvailableAtNotes->setIcon(QIcon(":/icons/res/note.png"));

  m_cbAvailableAtShop = new QCheckBox;
  m_cbAvailableAtShop->setText(tr("Compras"));
  m_cbAvailableAtShop->setIcon(QIcon(":/icons/res/shop.png"));

  m_cbAvailableAtConsumption = new QCheckBox;
  m_cbAvailableAtConsumption->setText(tr("Consumo"));
  m_cbAvailableAtConsumption->setIcon(QIcon(":/icons/res/stock.png"));

  m_cbAvailableToBuy = new QCheckBox;
  m_cbAvailableToBuy->setText(tr("Compra"));
  m_cbAvailableToBuy->setIcon(QIcon(":/icons/res/buy.png"));

  m_cbAvailableToSell = new QCheckBox;
  m_cbAvailableToSell->setText(tr("Venda"));
  m_cbAvailableToSell->setIcon(QIcon(":/icons/res/sell.png"));

  m_categoryPicker = new JPicker(INVALID_CATEGORY_ID, tr("Categoria"), false);

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->setAlignment(Qt::AlignLeft);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSave);

  QVBoxLayout* grplayout = new QVBoxLayout;
  grplayout->addWidget(m_cbAvailableAtNotes);
  grplayout->addWidget(m_cbAvailableAtShop);
  grplayout->addWidget(m_cbAvailableAtConsumption);
  grplayout->addWidget(m_cbAvailableToBuy);
  grplayout->addWidget(m_cbAvailableToSell);

  QGroupBox* grpAccess = new QGroupBox;
  grpAccess->setTitle(tr("Disponível em/para:"));
  grpAccess->setLayout(grplayout);

  QFormLayout* formlayout = new QFormLayout;
  formlayout->setContentsMargins(0, 0, 0, 0);
  formlayout->addRow(tr("Nome:"), m_edName);
  formlayout->addRow(tr("Unidade:"), m_edUnity);
  formlayout->addRow(tr("Unidade embalagem:"), m_edPackageUnity);
  formlayout->addRow(tr("Quantidade embalagem:"), m_spnPackageAmmount);
  formlayout->addRow(tr("Detalhes:"), m_edDetails);
  formlayout->addRow(tr("Código:"), m_edCode);

  QVBoxLayout* tablayout = new QVBoxLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addLayout(formlayout);
  tablayout->addWidget(m_categoryPicker);
  tablayout->addWidget(grpAccess);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  QTabWidget* tabWidget = new QTabWidget;
  tabWidget->addTab(tabframe,
                    QIcon(":/icons/res/item.png"),
                    tr("Produto"));

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->setContentsMargins(0, 0, 0, 0);
  mainlayout->setAlignment(Qt::AlignTop);
  mainlayout->addLayout(buttonlayout);
  mainlayout->addWidget(tabWidget);
  setLayout(mainlayout);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSaveSignal()));
  QObject::connect(m_categoryPicker,
                   SIGNAL(searchSignal()),
                   this,
                   SLOT(emitSearchCategorySignal()));
  QObject::connect(m_edName,
                   SIGNAL(textChanged(const QString&)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_edUnity,
                   SIGNAL(textChanged(const QString&)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_edPackageUnity,
                   SIGNAL(textChanged(const QString&)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_spnPackageAmmount,
                   SIGNAL(valueChanged(double)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_edDetails,
                   SIGNAL(textChanged(const QString&)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_edCode,
                   SIGNAL(textChanged(const QString&)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbAvailableAtNotes,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbAvailableAtShop,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbAvailableAtConsumption,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbAvailableToBuy,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbAvailableToSell,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_categoryPicker,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));

  updateControls();
}

ProductView::~ProductView()
{

}

Product ProductView::getProduct() const
{
  Product product;
  product.m_id = m_currentProduct.m_id;
  product.m_name = m_edName->text();
  product.m_unity = m_edUnity->text();
  product.m_packageUnity = m_edPackageUnity->text();
  product.m_packageAmmount = m_spnPackageAmmount->value();
  product.m_details = m_edDetails->text();
  product.m_code = m_edCode->text();
  product.m_bAvailableAtNotes = m_cbAvailableAtNotes->isChecked();
  product.m_bAvailableAtShop = m_cbAvailableAtShop->isChecked();
  product.m_bAvailableAtConsumption = m_cbAvailableAtConsumption->isChecked();
  product.m_bAvailableToBuy = m_cbAvailableToBuy->isChecked();
  product.m_bAvailableToSell = m_cbAvailableToSell->isChecked();
  product.m_categoryId = m_categoryPicker->getId();
  //TODO item.m_imageId = INVALID_IMAGE_ID;
  return product;
}

void ProductView::setCategory(int id, const QString& name)
{
  m_categoryPicker->setId(id);
  m_categoryPicker->setText(name);
}

void ProductView::setProduct(const Product &product,
                             const QString& categoryName)
{
  m_currentProduct = product;
  m_edName->setText(product.m_name);
  m_edUnity->setText(product.m_unity);
  m_edPackageUnity->setText(product.m_packageUnity);
  m_spnPackageAmmount->setValue(product.m_packageAmmount);
  m_edDetails->setText(product.m_details);
  m_edCode->setText(product.m_code);
  m_cbAvailableAtNotes->setChecked(product.m_bAvailableAtNotes);
  m_cbAvailableAtShop->setChecked(product.m_bAvailableAtShop);
  m_cbAvailableAtConsumption->setChecked(product.m_bAvailableAtConsumption);
  m_cbAvailableToBuy->setChecked(product.m_bAvailableToBuy);
  m_cbAvailableToSell->setChecked(product.m_bAvailableToSell);
  setCategory(product.m_categoryId, categoryName);
  updateControls();
}

void ProductView::create()
{
  Product product;
  setProduct(product, "");
  updateControls();
}

void ProductView::emitSearchCategorySignal()
{
  emit searchCategorySignal();
}

void ProductView::emitSaveSignal()
{
  emit saveSignal();
}

void ProductView::updateControls()
{
  Product product = getProduct();
  bool bEnable = product.isValid();
  QString saveIcon(":/icons/res/save.png");
  if (m_currentProduct.isValidId())
  {
    saveIcon = ":/icons/res/saveas.png";
    bEnable = bEnable && m_currentProduct != product;
  }
  m_btnSave->setEnabled(bEnable);
  m_btnSave->setIcon(QIcon(saveIcon));

}

#include "productview.h"
#include "jlineedit.h"
#include <QLayout>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>

ProductView::ProductView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_PRODUCT_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_edName(nullptr)
  , m_edUnity(nullptr)
  , m_edPackageUnity(nullptr)
  , m_sbPackageAmmount(nullptr)
  , m_edDetails(nullptr)
  , m_edCode(nullptr)
  , m_cbAvailableAtNotes(nullptr)
  , m_cbAvailableAtShop(nullptr)
  , m_cbAvailableAtConsumption(nullptr)
  , m_edCategory(nullptr)
  , m_btnSearchCategory(nullptr)
  , m_btnClearCategory(nullptr)
{
  m_btnCreate = new QPushButton();
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

  m_btnSave = new QPushButton();
  m_btnSave->setFlat(true);
  m_btnSave->setText("");
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

  m_edName = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edName->setMaxLength(MAX_PRODUCT_NAME_LENGTH);
  m_edName->setPlaceholderText(tr("Nome *"));

  m_edUnity = new JLineEdit(JValidatorType::Alphanumeric, true, true);
  m_edUnity->setMaxLength(MAX_PRODUCT_UNITY_LENGTH);
  m_edUnity->setPlaceholderText(tr("Unidade *"));

  m_edPackageUnity = new JLineEdit(JValidatorType::Alphanumeric, true, true);
  m_edPackageUnity->setMaxLength(MAX_PRODUCT_PACKAGE_UNITY_LENGTH);
  m_edPackageUnity->setPlaceholderText(tr("Unidade da Embalagem"));

  m_sbPackageAmmount = new QDoubleSpinBox();
  m_sbPackageAmmount->setMaximum(MAX_PRODUCT_PACKAGE_AMMOUNT_LENGTH);
  m_sbPackageAmmount->setMinimum(0.0);
  m_sbPackageAmmount->setSpecialValueText(tr("Quantidade da Embalagem"));

  m_edDetails = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edDetails->setMaxLength(MAX_PRODUCT_DETAILS_LENGTH);
  m_edDetails->setPlaceholderText(tr("Detalhes"));

  m_edCode = new JLineEdit(JValidatorType::Numeric, true, true);
  m_edCode->setMaxLength(MAX_PRODUCT_MIDASCODE_LENGTH);
  m_edCode->setPlaceholderText(tr("Código"));

  m_cbAvailableAtNotes = new QCheckBox();
  m_cbAvailableAtNotes->setText(tr("Vales"));
  m_cbAvailableAtNotes->setIcon(QIcon(":/icons/res/note.png"));

  m_cbAvailableAtShop = new QCheckBox();
  m_cbAvailableAtShop->setText(tr("Compras"));
  m_cbAvailableAtShop->setIcon(QIcon(":/icons/res/shop.png"));

  m_cbAvailableAtConsumption = new QCheckBox();
  m_cbAvailableAtConsumption->setText(tr("Consumo"));
  m_cbAvailableAtConsumption->setIcon(QIcon(":/icons/res/stock.png"));

  m_btnSearchCategory = new QPushButton();
  m_btnSearchCategory->setFlat(true);
  m_btnSearchCategory->setText("");
  m_btnSearchCategory->setIconSize(QSize(16, 16));
  m_btnSearchCategory->setIcon(QIcon(":/icons/res/binoculars.png"));
  m_btnSearchCategory->setToolTip(tr("Buscar categoria"));

  m_edCategory = new JLineEdit(JValidatorType::All, true, true);
  m_edCategory->setReadOnly(true);
  m_edCategory->setPlaceholderText(tr("Categoria"));

  m_btnClearCategory = new QPushButton();
  m_btnClearCategory->setFlat(true);
  m_btnClearCategory->setText("");
  m_btnClearCategory->setIconSize(QSize(16, 16));
  m_btnClearCategory->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnClearCategory->setToolTip(tr("Limpar categoria"));

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->setAlignment(Qt::AlignLeft);
  hlayout0->addWidget(m_btnCreate);
  hlayout0->addWidget(m_btnSave);

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->addWidget(m_btnSearchCategory);
  hlayout1->addWidget(m_edCategory);
  hlayout1->addWidget(m_btnClearCategory);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->addWidget(m_cbAvailableAtNotes);
  vlayout0->addWidget(m_cbAvailableAtShop);
  vlayout0->addWidget(m_cbAvailableAtConsumption);
  QGroupBox* grpAccess = new QGroupBox();
  grpAccess->setTitle(tr("Disponível em:"));
  grpAccess->setLayout(vlayout0);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->setContentsMargins(0, 0, 0, 0);
  vlayout1->setAlignment(Qt::AlignTop);
  vlayout1->addLayout(hlayout0);
  vlayout1->addWidget(m_edName);
  vlayout1->addWidget(m_edUnity);
  vlayout1->addWidget(m_edPackageUnity);
  vlayout1->addWidget(m_sbPackageAmmount);
  vlayout1->addWidget(m_edDetails);
  vlayout1->addWidget(m_edCode);
  vlayout1->addLayout(hlayout1);
  vlayout1->addWidget(grpAccess);

  setLayout(vlayout1);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));

  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSaveSignal()));

  QObject::connect(m_btnSearchCategory,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSearchCategorySignal()));

  QObject::connect(m_btnClearCategory,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clearCategory()));

  QObject::connect(m_edName,
                   SIGNAL(textChanged(const QString&)),
                   this,
                   SLOT(updateControls()));

  QObject::connect(m_edUnity,
                   SIGNAL(textChanged(const QString&)),
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
  product.m_id = m_currentId;
  product.m_name = m_edName->text();
  product.m_unity = m_edUnity->text();
  product.m_packageUnity = m_edPackageUnity->text();
  product.m_packageAmmount = m_sbPackageAmmount->value();
  product.m_details = m_edDetails->text();
  product.m_code = m_edCode->text();
  product.m_bAvailableAtNotes = m_cbAvailableAtNotes->isChecked();
  product.m_bAvailableAtShop = m_cbAvailableAtShop->isChecked();
  product.m_bAvailableAtConsumption = m_cbAvailableAtConsumption->isChecked();
  product.m_categoryId = m_currentCategoryId;
  //TODO item.m_imageId = INVALID_IMAGE_ID;
  return product;
}

void ProductView::setCategory(int categoryId,
                              const QString& categoryName)
{
  m_currentCategoryId = categoryId;
  m_edCategory->setText(categoryName);
}

void ProductView::setProduct(const Product &product,
                             const QString& categoryName)
{
  m_currentId = product.m_id;
  m_edName->setText(product.m_name);
  m_edUnity->setText(product.m_unity);
  m_edPackageUnity->setText(product.m_packageUnity);
  m_sbPackageAmmount->setValue(product.m_packageAmmount);
  m_edDetails->setText(product.m_details);
  m_edCode->setText(product.m_code);
  m_cbAvailableAtNotes->setChecked(product.m_bAvailableAtNotes);
  m_cbAvailableAtShop->setChecked(product.m_bAvailableAtShop);
  m_cbAvailableAtConsumption->setChecked(product.m_bAvailableAtConsumption);
  setCategory(product.m_categoryId, categoryName);
  updateControls();
}

void ProductView::create()
{
  Product product;
  Category category;
  setProduct(product, category.m_name);
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

void ProductView::clearCategory()
{
  Category category;
  setCategory(category.m_id, category.m_name);
}

void ProductView::updateControls()
{
  bool bEnable = !m_edName->text().isEmpty() &&
                 !m_edUnity->text().isEmpty();
  m_btnSave->setEnabled(bEnable);
  QString saveIcon = Product::st_isValidId(m_currentId)
                     ? ":/icons/res/saveas.png"
                     : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(saveIcon));

}

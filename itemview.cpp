#include "itemview.h"
#include "jlineedit.h"
#include <QLayout>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>

ItemView::ItemView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_ITEM_ID)
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
  m_edName->setMaxLength(MAX_ITEM_NAME_LENGTH);
  m_edName->setPlaceholderText(tr("Nome *"));

  m_edUnity = new JLineEdit(JValidatorType::Alphanumeric, true, true);
  m_edUnity->setMaxLength(MAX_ITEM_UNITY_LENGTH);
  m_edUnity->setPlaceholderText(tr("Unidade *"));

  m_edPackageUnity = new JLineEdit(JValidatorType::Alphanumeric, true, true);
  m_edPackageUnity->setMaxLength(MAX_ITEM_PACKAGE_UNITY_LENGTH);
  m_edPackageUnity->setPlaceholderText(tr("Unidade da Embalagem"));

  m_sbPackageAmmount = new QDoubleSpinBox();
  m_sbPackageAmmount->setMaximum(MAX_ITEM_PACKAGE_AMMOUNT_LENGTH);
  m_sbPackageAmmount->setMinimum(0.0);
  m_sbPackageAmmount->setSpecialValueText(tr("Quantidade da Embalagem"));

  m_edDetails = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edDetails->setMaxLength(MAX_ITEM_DETAILS_LENGTH);
  m_edDetails->setPlaceholderText(tr("Detalhes"));

  m_edCode = new JLineEdit(JValidatorType::Numeric, true, true);
  m_edCode->setMaxLength(MAX_ITEM_MIDASCODE_LENGTH);
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

ItemView::~ItemView()
{

}

Item ItemView::getItem() const
{
  Item item;
  item.m_id = m_currentId;
  item.m_name = m_edName->text();
  item.m_unity = m_edUnity->text();
  item.m_packageUnity = m_edPackageUnity->text();
  item.m_packageAmmount = m_sbPackageAmmount->value();
  item.m_details = m_edDetails->text();
  item.m_code = m_edCode->text();
  item.m_bAvailableAtNotes = m_cbAvailableAtNotes->isChecked();
  item.m_bAvailableAtShop = m_cbAvailableAtShop->isChecked();
  item.m_bAvailableAtConsumption = m_cbAvailableAtConsumption->isChecked();
  item.m_categoryId = m_currentCategoryId;
  //TODO item.m_imageId = INVALID_IMAGE_ID;
  return item;
}

void ItemView::setCategory(int categoryId,
                           const QString& categoryName)
{
  m_currentCategoryId = categoryId;
  m_edCategory->setText(categoryName);
}

void ItemView::setItem(const Item &item,
                       const QString& categoryName)
{
  m_currentId = item.m_id;
  m_edName->setText(item.m_name);
  m_edUnity->setText(item.m_unity);
  m_edPackageUnity->setText(item.m_packageUnity);
  m_sbPackageAmmount->setValue(item.m_packageAmmount);
  m_edDetails->setText(item.m_details);
  m_edCode->setText(item.m_code);
  m_cbAvailableAtNotes->setChecked(item.m_bAvailableAtNotes);
  m_cbAvailableAtShop->setChecked(item.m_bAvailableAtShop);
  m_cbAvailableAtConsumption->setChecked(item.m_bAvailableAtConsumption);
  setCategory(item.m_categoryId, categoryName);
  updateControls();
}

void ItemView::create()
{
  Item item;
  Category category;
  setItem(item, category.m_name);
  updateControls();
}

void ItemView::emitSearchCategorySignal()
{
  emit searchCategorySignal();
}

void ItemView::emitSaveSignal()
{
  emit saveSignal();
}

void ItemView::clearCategory()
{
  Category category;
  setCategory(category.m_id, category.m_name);
}

void ItemView::updateControls()
{
  bool bEnable = !m_edName->text().isEmpty() &&
                 !m_edUnity->text().isEmpty();
  m_btnSave->setEnabled(bEnable);
  QString saveIcon = Item::st_isValidId(m_currentId)
                     ? ":/icons/res/saveas.png"
                     : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(saveIcon));

}

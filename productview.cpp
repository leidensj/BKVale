#include "productview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "jdatabase.h"
#include "jdoublespinbox.h"
#include <QLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QFormLayout>
#include <QTabWidget>
#include <QSplitter>

ProductView::ProductView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_edName(nullptr)
  , m_edUnity(nullptr)
  , m_edPackageUnity(nullptr)
  , m_spnPackageAmmount(nullptr)
  , m_edDetails(nullptr)
  , m_cbAvailableAtNotes(nullptr)
  , m_cbAvailableAtShop(nullptr)
  , m_cbAvailableAtConsumption(nullptr)
  , m_cbAvailableToBuy(nullptr)
  , m_cbAvailableToSell(nullptr)
  , m_categoryPicker(nullptr)
  , m_imagePicker(nullptr)
  , m_database(nullptr)
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
  m_edName->setMaxLength(PRODUCT_MAX_NAME_LENGTH);
  m_edName->setPlaceholderText("*");

  m_edUnity = new JLineEdit(JValidatorType::Alphanumeric, true, true);
  m_edUnity->setMaxLength(PRODUCT_MAX_UNITY_LENGTH);
  m_edUnity->setPlaceholderText("*");

  m_edPackageUnity = new JLineEdit(JValidatorType::Alphanumeric, true, true);
  m_edPackageUnity->setMaxLength(PRODUCT_MAX_PACKAGE_UNITY_LENGTH);

  m_spnPackageAmmount = new JDoubleSpinBox(true);
  m_spnPackageAmmount->setMaximum(PRODUCT_MAX_PACKAGE_AMMOUNT_LENGTH);
  m_spnPackageAmmount->setMinimum(0.0);
  m_spnPackageAmmount->setValue(1.0);
  m_spnPackageAmmount->setSingleStep(0.1);

  m_edDetails = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edDetails->setMaxLength(PRODUCT_MAX_DETAILS_LENGTH);

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

  m_categoryPicker = new JDatabasePicker(tr("Categoria"), QIcon(":/icons/res/category.png"), true, true);
  m_imagePicker = new JDatabasePicker(tr("Imagem"), QIcon(":/icons/res/icon.png"), true, true);

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->setAlignment(Qt::AlignLeft);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSave);

  QFormLayout* formlayout = new QFormLayout;
  formlayout->setContentsMargins(0, 0, 0, 0);
  formlayout->addRow(tr("Nome:"), m_edName);
  formlayout->addRow(tr("Unidade:"), m_edUnity);
  formlayout->addRow(tr("Unidade embalagem:"), m_edPackageUnity);
  formlayout->addRow(tr("Quantidade embalagem:"), m_spnPackageAmmount);
  formlayout->addRow(tr("Detalhes:"), m_edDetails);

  QVBoxLayout* tabInfoLayout = new QVBoxLayout;
  tabInfoLayout->setAlignment(Qt::AlignTop);
  tabInfoLayout->addLayout(formlayout);
  tabInfoLayout->addWidget(m_categoryPicker);
  tabInfoLayout->addWidget(m_imagePicker);

  QVBoxLayout* tabAvailablelayout = new QVBoxLayout;
  tabAvailablelayout->addWidget(m_cbAvailableAtNotes);
  tabAvailablelayout->addWidget(m_cbAvailableAtShop);
  tabAvailablelayout->addWidget(m_cbAvailableAtConsumption);
  tabAvailablelayout->addWidget(m_cbAvailableToBuy);
  tabAvailablelayout->addWidget(m_cbAvailableToSell);
  tabAvailablelayout->setAlignment(Qt::AlignTop);

  QFrame* tabInfoFrame = new QFrame;
  tabInfoFrame->setLayout(tabInfoLayout);

  QFrame* tabAvailableFrame = new QFrame;
  tabAvailableFrame->setLayout(tabAvailablelayout);

  QTabWidget* tabWidget = new QTabWidget;

  tabWidget->addTab(tabInfoFrame,
                    QIcon(":/icons/res/details.png"),
                    tr("Informações"));

  tabWidget->addTab(tabAvailableFrame,
                    QIcon(":/icons/res/check.png"),
                    tr("Disponibilidade"));

  QVBoxLayout* viewlayout = new QVBoxLayout;
  viewlayout->setContentsMargins(9, 0, 0, 0);
  viewlayout->setAlignment(Qt::AlignTop);
  viewlayout->addLayout(buttonlayout);
  viewlayout->addWidget(tabWidget);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewlayout);

  m_database = new JDatabase;

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

ProductView::~ProductView()
{

}

void ProductView::setDatabase(QSqlDatabase db)
{
  m_categoryPicker->setDatabase(db, CATEGORY_SQL_TABLE_NAME);
  m_imagePicker->setDatabase(db, IMAGE_SQL_TABLE_NAME);
  m_database->setDatabase(db, PRODUCT_SQL_TABLE_NAME);
}

Product ProductView::getProduct() const
{
  Product product;
  product.m_id = m_currentId;
  product.m_name = m_edName->text();
  product.m_unity = m_edUnity->text();
  product.m_packageUnity = m_edPackageUnity->text();
  product.m_packageAmmount = m_spnPackageAmmount->value();
  product.m_details = m_edDetails->text();
  product.m_bAvailableAtNotes = m_cbAvailableAtNotes->isChecked();
  product.m_bAvailableAtShop = m_cbAvailableAtShop->isChecked();
  product.m_bAvailableAtConsumption = m_cbAvailableAtConsumption->isChecked();
  product.m_bAvailableToBuy = m_cbAvailableToBuy->isChecked();
  product.m_bAvailableToSell = m_cbAvailableToSell->isChecked();
  product.m_category.m_id = m_categoryPicker->getId();
  product.m_image.m_id = m_imagePicker->getId();
  return product;
}

void ProductView::setProduct(const Product &product)
{
  QString strIcon = product.isValidId()
                    ? ":/icons/res/saveas.png"
                    : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(strIcon));
  m_currentId = product.m_id;
  m_edName->setText(product.m_name);
  m_edUnity->setText(product.m_unity);
  m_edPackageUnity->setText(product.m_packageUnity);
  m_spnPackageAmmount->setValue(product.m_packageAmmount);
  m_edDetails->setText(product.m_details);
  m_cbAvailableAtNotes->setChecked(product.m_bAvailableAtNotes);
  m_cbAvailableAtShop->setChecked(product.m_bAvailableAtShop);
  m_cbAvailableAtConsumption->setChecked(product.m_bAvailableAtConsumption);
  m_cbAvailableToBuy->setChecked(product.m_bAvailableToBuy);
  m_cbAvailableToSell->setChecked(product.m_bAvailableToSell);
  m_categoryPicker->setItem(product.m_category.m_id, product.m_category.m_name, product.m_category.m_image.m_image);
  m_imagePicker->setItem(product.m_image.m_id, product.m_image.m_name, product.m_image.m_image);
}

void ProductView::create()
{
  Product product;
  setProduct(product);
}

void ProductView::itemSelected(const JItem& jItem)
{
  const Product& product = dynamic_cast<const Product&>(jItem);
  if (product.isValidId())
    setProduct(product);
}

void ProductView::itemRemoved(qlonglong id)
{
  if (id == m_currentId)
    create();
}

void ProductView::save()
{
  if (m_database->save(getProduct()))
    create();
}

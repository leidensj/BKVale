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
  , m_edDetails(nullptr)
  , m_cbAvailableAtNotes(nullptr)
  , m_cbAvailableAtShop(nullptr)
  , m_cbAvailableAtConsumption(nullptr)
  , m_cbAvailableToBuy(nullptr)
  , m_cbAvailableToSell(nullptr)
  , m_categoryPicker(nullptr)
  , m_imagePicker(nullptr)
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

  m_edName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                           JLineEdit::st_defaultFlags1);
  m_edName->setMaxLength(PRODUCT_MAX_NAME_LENGTH);
  m_edName->setPlaceholderText("*");

  m_edUnity = new JLineEdit(JLineEdit::Input::Alphanumeric,
                            JLineEdit::st_defaultFlags1);
  m_edUnity->setMaxLength(PRODUCT_MAX_UNITY_LENGTH);
  m_edUnity->setPlaceholderText("*");

  m_edDetails = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                              JLineEdit::st_defaultFlags1);
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

  m_categoryPicker = new JDatabasePicker(CATEGORY_SQL_TABLE_NAME,
                                         tr("Categoria"),
                                         QIcon(":/icons/res/category.png"),
                                         true,
                                         true,
                                         false);
  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME,
                                      tr("Imagem"),
                                      QIcon(":/icons/res/icon.png"),
                                      true,
                                      true,
                                      false);

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->setAlignment(Qt::AlignLeft);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSave);

  QFormLayout* formlayout = new QFormLayout;
  formlayout->setContentsMargins(0, 0, 0, 0);
  formlayout->addRow(tr("Nome:"), m_edName);
  formlayout->addRow(tr("Unidade:"), m_edUnity);
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

  m_tab = new QTabWidget;

  m_tab->addTab(tabInfoFrame,
                QIcon(":/icons/res/details.png"),
                tr("Informações"));

  m_tab->addTab(tabAvailableFrame,
                QIcon(":/icons/res/check.png"),
                tr("Disponibilidade"));

  QVBoxLayout* viewlayout = new QVBoxLayout;
  viewlayout->setContentsMargins(9, 0, 0, 0);
  viewlayout->setAlignment(Qt::AlignTop);
  viewlayout->addLayout(buttonlayout);
  viewlayout->addWidget(m_tab);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewlayout);

  m_database = new JDatabase(PRODUCT_SQL_TABLE_NAME);

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

Product ProductView::getProduct() const
{
  Product product;
  product.m_id = m_currentId;
  product.m_name = m_edName->text();
  product.m_unity = m_edUnity->text();
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
  m_tab->setCurrentIndex(0);
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

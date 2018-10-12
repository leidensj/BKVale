#include "productview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "jdoublespinbox.h"
#include <QLayout>
#include <QCheckBox>
#include <QFormLayout>
#include <QSplitter>

ProductView::ProductView(QWidget* parent)
  : JItemView(PRODUCT_SQL_TABLE_NAME, parent)
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
{
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

  m_tab->addTab(tabInfoFrame,
                QIcon(":/icons/res/item.png"),
                tr("Produto"));

  m_tab->addTab(tabAvailableFrame,
                QIcon(":/icons/res/check.png"),
                tr("Disponibilidade"));
}

ProductView::~ProductView()
{

}

const JItem& ProductView::getItem() const
{
  static Product o;
  o.m_id = m_currentId;
  o.m_name = m_edName->text();
  o.m_unity = m_edUnity->text();
  o.m_details = m_edDetails->text();
  o.m_bAvailableAtNotes = m_cbAvailableAtNotes->isChecked();
  o.m_bAvailableAtShop = m_cbAvailableAtShop->isChecked();
  o.m_bAvailableAtConsumption = m_cbAvailableAtConsumption->isChecked();
  o.m_bAvailableToBuy = m_cbAvailableToBuy->isChecked();
  o.m_bAvailableToSell = m_cbAvailableToSell->isChecked();
  o.m_category.m_id = m_categoryPicker->getId();
  o.m_image.m_id = m_imagePicker->getId();
  return o;
}

void ProductView::setItem(const JItem &o)
{
  auto _o = dynamic_cast<const Product&>(o);
  m_currentId = _o.m_id;
  m_edName->setText(_o.m_name);
  m_edUnity->setText(_o.m_unity);
  m_edDetails->setText(_o.m_details);
  m_cbAvailableAtNotes->setChecked(_o.m_bAvailableAtNotes);
  m_cbAvailableAtShop->setChecked(_o.m_bAvailableAtShop);
  m_cbAvailableAtConsumption->setChecked(_o.m_bAvailableAtConsumption);
  m_cbAvailableToBuy->setChecked(_o.m_bAvailableToBuy);
  m_cbAvailableToSell->setChecked(_o.m_bAvailableToSell);
  m_categoryPicker->setItem(_o.m_category);
  m_imagePicker->setItem(_o.m_image);
}

void ProductView::create()
{
  selectItem(Product());
  m_tab->setCurrentIndex(0);
  m_edName->setFocus();
}

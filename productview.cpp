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
  , m_cbBuy(nullptr)
  , m_cbSell(nullptr)
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

  m_cbBuy = new QCheckBox;
  m_cbBuy->setIcon(QIcon(":/icons/res/buy.png"));

  m_cbSell = new QCheckBox;
  m_cbSell->setIcon(QIcon(":/icons/res/sell.png"));

  m_categoryPicker = new JDatabasePicker(CATEGORY_SQL_TABLE_NAME,
                                         tr("Categoria"),
                                         QIcon(":/icons/res/category.png"));
  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME,
                                      tr("Imagem"),
                                      QIcon(":/icons/res/icon.png"));

  QFormLayout* formlayout = new QFormLayout;
  formlayout->addRow(tr("Nome:"), m_edName);
  formlayout->addRow(tr("Unidade:"), m_edUnity);
  formlayout->addRow(tr("Detalhes:"), m_edDetails);
  formlayout->addRow(tr("Categoria:"), m_categoryPicker);
  formlayout->addRow(tr("Imagem:"), m_imagePicker);
  formlayout->addRow(tr("Compra:"), m_cbBuy);
  formlayout->addRow(tr("Venda:"), m_cbSell);

  QFrame* tabInfoFrame = new QFrame;
  tabInfoFrame->setLayout(formlayout);

  m_tab->addTab(tabInfoFrame,
                QIcon(":/icons/res/item.png"),
                tr("Produto"));
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
  o.m_bBuy = m_cbBuy->isChecked();
  o.m_bSell = m_cbSell->isChecked();
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
  m_cbBuy->setChecked(_o.m_bBuy);
  m_cbSell->setChecked(_o.m_bSell);
  m_categoryPicker->setItem(_o.m_category);
  m_imagePicker->setItem(_o.m_image);
}

void ProductView::create()
{
  selectItem(Product());
  m_tab->setCurrentIndex(0);
  m_edName->setFocus();
}

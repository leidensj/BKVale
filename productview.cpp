#include "productview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "jdoublespinbox.h"
#include "productcodetablewidget.h"
#include "jaddremovebuttons.h"
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
  , m_tbCode(nullptr)
  , m_btns(nullptr)
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
  m_cbBuy->setText(tr("Compra"));

  m_cbSell = new QCheckBox;
  m_cbSell->setIcon(QIcon(":/icons/res/sell.png"));
  m_cbSell->setText(tr("Venda"));

  m_categoryPicker = new JDatabasePicker(CATEGORY_SQL_TABLE_NAME);
  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME);

  m_tbCode = new ProductCodeTableWidget;
  m_btns = new JAddRemoveButtons;

  QVBoxLayout* ltCode = new QVBoxLayout;
  ltCode->addWidget(m_btns);
  ltCode->addWidget(m_tbCode);

  QFormLayout* formlayout = new QFormLayout;
  formlayout->addRow(tr("Nome:"), m_edName);
  formlayout->addRow(tr("Unidade:"), m_edUnity);
  formlayout->addRow(tr("Detalhes:"), m_edDetails);
  formlayout->addRow(tr("Categoria:"), m_categoryPicker);
  formlayout->addRow(tr("Imagem:"), m_imagePicker);
  formlayout->addRow("", m_cbBuy);
  formlayout->addRow("", m_cbSell);

  QFrame* tabInfoFrame = new QFrame;
  tabInfoFrame->setLayout(formlayout);

  QFrame* tabCodeFrame = new QFrame;
  tabCodeFrame->setLayout(ltCode);

  m_tab->addTab(tabInfoFrame, QIcon(":/icons/res/item.png"), tr("Produto"));
  m_tab->addTab(tabCodeFrame, QIcon(":/icons/res/barcode.png"), tr("Códigos"));
  connect(m_tbCode, SIGNAL(changedSignal(bool)), m_btns, SLOT(enableRemoveButton(bool)));
  connect(m_btns->m_btnAdd, SIGNAL(clicked(bool)), m_tbCode, SLOT(addItem()));
  connect(m_btns->m_btnRemove, SIGNAL(clicked(bool)), m_tbCode, SLOT(removeItem()));

}

ProductView::~ProductView()
{

}

const JItemSQL& ProductView::getItem() const
{
  m_ref.clear(false);
  m_ref.m_name = m_edName->text();
  m_ref.m_unity = m_edUnity->text();
  m_ref.m_details = m_edDetails->text();
  m_ref.m_bBuy = m_cbBuy->isChecked();
  m_ref.m_bSell = m_cbSell->isChecked();
  m_ref.m_category.m_id = m_categoryPicker->getId();
  m_ref.m_image.m_id = m_imagePicker->getId();
  for (int i = 0; i != m_tbCode->rowCount(); ++i)
    m_ref.m_vCode.push_back(dynamic_cast<const ProductCode&>(m_tbCode->getItem(i)));
  return m_ref;
}

void ProductView::setItem(const JItemSQL &o)
{
  m_ref = dynamic_cast<const Product&>(o);
  m_edName->setText(m_ref.m_name);
  m_edUnity->setText(m_ref.m_unity);
  m_edDetails->setText(m_ref.m_details);
  m_cbBuy->setChecked(m_ref.m_bBuy);
  m_cbSell->setChecked(m_ref.m_bSell);
  m_categoryPicker->setItem(m_ref.m_category);
  m_imagePicker->setItem(m_ref.m_image);
  m_tbCode->removeAllItems();
  for (int i = 0; i != m_ref.m_vCode.size(); ++i)
    m_tbCode->addItem(m_ref.m_vCode.at(i));
}

void ProductView::create()
{
  selectItem(Product());
  m_tab->setCurrentIndex(0);
  m_edName->setFocus();
}

Id ProductView::getId() const
{
  return m_ref.m_id;
}

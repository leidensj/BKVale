#include "productview.h"
#include "widgets/jlineedit.h"
#include "widgets/jdatabasepicker.h"
#include "widgets/jdoublespinbox.h"
#include "widgets/jaddremovebuttons.h"
#include "tables/productcodetable.h"
#include "packageeditor.h"
#include <QLayout>
#include <QAction>
#include <QCheckBox>
#include <QFormLayout>
#include <QMessageBox>

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
  , m_actUnity(nullptr)
{
  m_edName = new JLineEdit(Text::Input::AlphanumericAndSpaces,
                           JLineEdit::st_defaultFlags1);
  m_edName->setMaxLength(PRODUCT_MAX_NAME_LENGTH);
  m_edName->setPlaceholderText("*");

  m_edUnity = new JLineEdit(Text::Input::Alphanumeric,
                            JLineEdit::st_defaultFlags1);
  m_edUnity->setMaxLength(PRODUCT_MAX_UNITY_LENGTH);
  m_edUnity->setPlaceholderText("*");

  m_edDetails = new JLineEdit(Text::Input::AlphanumericAndSpaces,
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

  m_btns = new JAddRemoveButtons;
  m_tbCode = new ProductCodeTable(m_btns);

  m_actUnity = m_edUnity->addAction(QIcon(":/icons/res/package.png"), QLineEdit::TrailingPosition);
  connect(m_actUnity, SIGNAL(triggered(bool)), this, SLOT(editUnity()));

  addViewButton(CATEGORY_SQL_TABLE_NAME);
  addViewButton(IMAGE_SQL_TABLE_NAME);

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

  setFocusWidgetOnCreate(m_edName);
  create();
}

ProductView::~ProductView()
{

}

void ProductView::getItem(JItemSQL& o) const
{
  Product& _o = dynamic_cast<Product&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_name = m_edName->text();
  _o.m_unity = m_edUnity->text();
  _o.m_details = m_edDetails->text();
  _o.m_bBuy = m_cbBuy->isChecked();
  _o.m_bSell = m_cbSell->isChecked();
  _o.m_category.m_id = m_categoryPicker->getId();
  _o.m_image.m_id = m_imagePicker->getId();
  m_tbCode->getCodes(_o.m_vCode);
}

void ProductView::setItem(const JItemSQL &o)
{
  const Product& _o = dynamic_cast<const Product&>(o);
  m_edName->setText(_o.m_name);
  m_edUnity->setText(_o.m_unity);
  m_edDetails->setText(_o.m_details);
  m_cbBuy->setChecked(_o.m_bBuy);
  m_cbSell->setChecked(_o.m_bSell);
  m_categoryPicker->setItem(_o.m_category);
  m_imagePicker->setItem(_o.m_image);
  m_tbCode->setCodes(_o.m_vCode);
  const bool bNewItem = _o.m_id.isValid();
  m_edUnity->setReadOnly(bNewItem);
  m_actUnity->setVisible(bNewItem);
  m_actUnity->setEnabled(bNewItem);
}

void ProductView::editUnity()
{
  PackageEditor dlg(true);
  Package pck;
  dlg.setPackage(pck, m_edUnity->text());
  if (dlg.exec())
  {
    pck = dlg.getPackage();
    if (QMessageBox::question(this,
                              tr("Modificar Unidade"),
                              tr("Tem certeza de que deseja modificar a unidade deste produto e atualizar todas suas referências?\n"
                                 "Unidade atual: %1\n"
                                 "Nova unidade: %2\n"
                                 "1%3=%4%5").arg(
                                m_edUnity->text(),
                                pck.m_unity,
                                pck.m_unity,
                                Data::strAmmount(pck.m_ammount),
                                m_edUnity->text()),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {

    }
  }
}

#include "inventoryview.h"
#include "widgets/jlineedit.h"
#include "controls/databasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

InventoryView::InventoryView(QWidget* parent)
  : JItemView(INVENTORY_SQL_TABLE_NAME, parent)
{
  addViewButton(PRODUCT_SQL_TABLE_NAME);

  QFormLayout* ltForm = new QFormLayout;

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltForm);

  m_tab->addTab(tabframe, QIcon(":/icons/res/inventory.png"), tr("Estoque"));
  //setFocusWidgetOnClear(m_storePicker);
  setContentsMargins(9, 9, 9, 9);
  clear();
}

void InventoryView::getItem(JItemSQL& o) const
{
  /*Category& _o = dynamic_cast<Category&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_image.m_id = m_imagePicker->getFirstId();
  _o.m_name = m_edName->text();*/
}

void InventoryView::setItem(const JItemSQL& o)
{
  /*const Category& _o = static_cast<const Category&>(o);
  m_edName->setText(_o.m_name);
  m_imagePicker->addItem(_o.m_image);*/
}

#include "supplierview.h"
#include "jdatabasepicker.h"
#include <QFormLayout>

SupplierView::SupplierView(QWidget* parent)
  : JItemView(SUPPLIER_SQL_TABLE_NAME, parent)
  , m_formPicker(nullptr)
{
  m_formPicker = new JDatabasePicker(FORM_SQL_TABLE_NAME);
  m_formPicker->getDatabase()->setFixedFilter(SUPPLIER_UNIQUE_FORM_FILTER);

  QFormLayout* lt = new QFormLayout;
  lt->addRow(m_formPicker->getText() + ":", m_formPicker);

  QFrame* fr = new QFrame;
  fr->setLayout(lt);

  m_tab->addTab(fr, QIcon(":/icons/res/supplier.png"), tr("Fornecedor"));
}

void SupplierView::create()
{
  selectItem(Supplier());
}

const JItemSQL& SupplierView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
  m_ref.m_form.m_id = m_formPicker->getId();
  return m_ref;
}

void SupplierView::setItem(const JItemSQL& o)
{
  auto ref = static_cast<const Supplier&>(o);
  m_currentId = o.m_id;
  m_formPicker->setItem(ref.m_form);
}

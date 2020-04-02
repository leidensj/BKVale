#include "supplierview.h"
#include "editors/formeditor.h"
#include <QFormLayout>
#include <QRadioButton>
#include "widgets/jlineedit.h"

SupplierView::SupplierView(QWidget* parent)
  : JItemView(SUPPLIER_SQL_TABLE_NAME, parent)
  , m_formInfo(nullptr)
  , m_formDetails(nullptr)
  , m_formPhone(nullptr)
  , m_formAddress(nullptr)
{
  addViewButton(IMAGE_SQL_TABLE_NAME);

  m_formInfo = new FormInfoEditor;
  m_formDetails = new FormDetailsEditor;
  m_formPhone = new FormPhoneEditor;
  m_formAddress= new FormAddressEditor;

  m_tab->addTab(m_formInfo, QIcon(":/icons/res/resume.png"), tr("Informações"));
  m_tab->addTab(m_formDetails, QIcon(":/icons/res/details.png"), tr("Detalhes"));
  m_tab->addTab(m_formPhone, QIcon(":/icons/res/phone.png"), tr("Telefone"));
  m_tab->addTab(m_formAddress, QIcon(":/icons/res/address.png"), tr("Endereço"));

  connect(m_formInfo, SIGNAL(userTypeChangedSignal(bool)), m_formDetails, SLOT(switchUserType(bool)));
  m_formInfo->setCompany(true);

  // TODO WHY JLINEEDIT* TO QWIDGET NOT WORKING
  setFocusWidgetOnCreate(m_formInfo->m_edName);
  create();
}

void SupplierView::getItem(JItemSQL& o) const
{
  Supplier& _o = dynamic_cast<Supplier&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_form.m_id = m_formId;
  m_formInfo->fillForm(_o.m_form);
  m_formDetails->fillForm(_o.m_form);
  m_formPhone->fillForm(_o.m_form);
  m_formAddress->fillForm(_o.m_form);
}

void SupplierView::setItem(const JItemSQL& o)
{
  const Supplier& _o= static_cast<const Supplier&>(o);
  m_formId = _o.m_form.m_id;
  m_formInfo->setForm(_o.m_form);
  m_formDetails->setForm(_o.m_form);
  m_formPhone->setForm(_o.m_form);
  m_formAddress->setForm(_o.m_form);
}

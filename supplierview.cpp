#include "supplierview.h"
#include "formwidget.h"
#include <QFormLayout>
#include <QRadioButton>

SupplierView::SupplierView(QWidget* parent)
  : JItemView(SUPPLIER_SQL_TABLE_NAME, parent)
  , m_formInfo(nullptr)
  , m_formDetails(nullptr)
  , m_formPhone(nullptr)
  , m_formAddress(nullptr)
{
  m_formInfo = new FormInfoWidget;
  m_formDetails = new FormDetailsWidget;
  m_formPhone = new FormPhoneWidget;
  m_formAddress= new FormAddressWidget;

  m_tab->addTab(m_formInfo, QIcon(":/icons/res/resume.png"), tr("Informações"));
  m_tab->addTab(m_formDetails, QIcon(":/icons/res/details.png"), tr("Detalhes"));
  m_tab->addTab(m_formPhone, QIcon(":/icons/res/phone.png"), tr("Telefone"));
  m_tab->addTab(m_formAddress, QIcon(":/icons/res/address.png"), tr("Endereço"));

  connect(m_formInfo, SIGNAL(userTypeChangedSignal(bool)), m_formDetails, SLOT(switchUserType(bool)));
  m_formInfo->setCompany(true);
}

void SupplierView::create()
{
  selectItem(Supplier());
}

const JItemSQL& SupplierView::getItem() const
{
  m_ref.clear(false);
  m_formInfo->fillForm(m_ref.m_form);
  m_formDetails->fillForm(m_ref.m_form);
  m_formPhone->fillForm(m_ref.m_form);
  m_formAddress->fillForm(m_ref.m_form);
  return m_ref;
}

void SupplierView::setItem(const JItemSQL& o)
{
  m_ref = static_cast<const Supplier&>(o);
  m_formInfo->setForm(m_ref.m_form);
  m_formDetails->setForm(m_ref.m_form);
  m_formPhone->setForm(m_ref.m_form);
  m_formAddress->setForm(m_ref.m_form);
}

Id SupplierView::getId() const
{
  return m_ref.m_id;
}

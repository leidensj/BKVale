#include "storeview.h"
#include "widgets/jlineedit.h"
#include "widgets/jplaintextedit.h"
#include "widgets/jaddremovebuttons.h"
#include "controls/databaseviewer.h"
#include "tables/storeemployeestable.h"
#include "editors/formeditor.h"
#include <QLayout>
#include <QFormLayout>
#include <QRadioButton>
#include <QLabel>
#include <QDateEdit>

StoreView::StoreView(QWidget* parent)
  : JItemView(STORE_SQL_TABLE_NAME, parent)
  , m_formInfo(nullptr)
  , m_formDetails(nullptr)
  , m_formPhone(nullptr)
  , m_formAddress(nullptr)
  , m_tbEmployee(nullptr)
  , m_btnAddRemove(nullptr)
{
  addViewButton(EMPLOYEE_SQL_TABLE_NAME);
  addViewButton(IMAGE_SQL_TABLE_NAME);

  m_formInfo = new FormInfoEditor;
  m_formDetails = new FormDetailsEditor;
  m_formPhone = new FormPhoneEditor;
  m_formAddress= new FormAddressEditor;
  m_btnAddRemove = new JAddRemoveButtons;
  m_tbEmployee = new StoreEmployeesTable(m_btnAddRemove);

  QVBoxLayout* ltTable = new QVBoxLayout;
  ltTable->addWidget(m_btnAddRemove);
  ltTable->addWidget(m_tbEmployee);

  QFrame* frTable = new QFrame;
  frTable->setLayout(ltTable);

  m_tab->addTab(m_formInfo, QIcon(":/icons/res/resume.png"), tr("Informações"));
  m_tab->addTab(m_formDetails, QIcon(":/icons/res/details.png"), tr("Detalhes"));
  m_tab->addTab(m_formPhone, QIcon(":/icons/res/phone.png"), tr("Telefone"));
  m_tab->addTab(m_formAddress, QIcon(":/icons/res/address.png"), tr("Endereço"));
  m_tab->addTab(frTable, QIcon(":/icons/res/employee.png"), tr("Funcionários"));

  connect(m_formInfo, SIGNAL(userTypeChangedSignal(bool)), m_formDetails, SLOT(switchUserType(bool)));

  m_formInfo->m_lblCreationDate->hide();
  m_formInfo->m_dtCreationDate->hide();
  m_formInfo->m_lblType->hide();
  m_formInfo->m_type->hide();

  setFocusWidgetOnCreate(m_formInfo->m_edName);
  create();
}

void StoreView::getItem(JItemSQL& o) const
{
  Store& _o = dynamic_cast<Store&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_form.m_id = m_formId;
  _o.m_vEmployee.clear();
  m_formInfo->fillForm(_o.m_form);
  m_formDetails->fillForm(_o.m_form);
  m_formPhone->fillForm(_o.m_form);
  m_formAddress->fillForm(_o.m_form);
  m_tbEmployee->getEmployees(_o.m_vEmployee);
}

void StoreView::setItem(const JItemSQL& o)
{
  const Store& _o = static_cast<const Store&>(o);
  m_formId = _o.m_form.m_id;
  m_formInfo->setForm(_o.m_form);
  m_formDetails->setForm(_o.m_form);
  m_formPhone->setForm(_o.m_form);
  m_formAddress->setForm(_o.m_form);
  m_tbEmployee->setEmployees(_o.m_vEmployee);
}

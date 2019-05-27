#include "storeview.h"
#include "jlineedit.h"
#include "jplaintextedit.h"
#include "formwidget.h"
#include "jtable.h"
#include "jaddremovebuttons.h"
#include "jdatabase.h"
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
  m_formInfo = new FormInfoWidget;
  m_formDetails = new FormDetailsWidget;
  m_formPhone = new FormPhoneWidget;
  m_formAddress= new FormAddressWidget;
  m_tbEmployee = new JTable;
  m_btnAddRemove = new JAddRemoveButtons;

  m_tbEmployee->setColumnCount(1);
  QStringList headers;
  headers << tr("Funcionário");
  m_tbEmployee->setHorizontalHeaderLabels(headers);
  m_tbEmployee->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

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
  connect(m_btnAddRemove->m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(addEmployee()));
  connect(m_btnAddRemove->m_btnRemove, SIGNAL(clicked(bool)), m_tbEmployee, SLOT(removeItem()));
  connect(m_tbEmployee, SIGNAL(itemSelectionChanged()), this, SLOT(updateControls()));
  m_formInfo->m_lblCreationDate->hide();
  m_formInfo->m_dtCreationDate->hide();
  m_formInfo->m_lblType->hide();
  m_formInfo->m_type->hide();
  m_formInfo->setCompany(true);
  updateControls();
}

void StoreView::create()
{
  selectItem(Store());
  updateControls();
}

void StoreView::updateControls()
{
  m_btnAddRemove->m_btnRemove->setEnabled(m_tbEmployee->isValidCurrentRow());
}

const JItemSQL& StoreView::getItem() const
{
  m_ref.clear(false);
  m_ref.m_vEmployee.clear();
  m_formInfo->fillForm(m_ref.m_form);
  m_formDetails->fillForm(m_ref.m_form);
  m_formPhone->fillForm(m_ref.m_form);
  m_formAddress->fillForm(m_ref.m_form);
  for (int i = 0; i != m_tbEmployee->rowCount(); ++i)
  {
    Employee e;
    int row = m_tbEmployee->verticalHeader()->logicalIndex(i);
    e.m_id.set(m_tbEmployee->item(row, 0)->data(Qt::UserRole).toLongLong());
    m_ref.m_vEmployee.push_back(e);
  }
  return m_ref;
}

void StoreView::setItem(const JItemSQL& o)
{
  m_ref = static_cast<const Store&>(o);
  m_formInfo->setForm(m_ref.m_form);
  m_formDetails->setForm(m_ref.m_form);
  m_formPhone->setForm(m_ref.m_form);
  m_formAddress->setForm(m_ref.m_form);
  m_tbEmployee->removeAllItems();
  for (int i = 0; i != m_ref.m_vEmployee.size(); ++i)
    addEmployee(m_ref.m_vEmployee.at(i));
}

Id StoreView::getId() const
{
  return m_ref.m_id;
}

void StoreView::addEmployee()
{
  QString filter = EMPLOYEE_SQL_TABLE_NAME "." SQL_COLID " NOT IN ("
                   "SELECT " STORE_EMPLOYEES_SQL_TABLE_NAME "." STORE_EMPLOYEES_SQL_COL01
                   " FROM " STORE_EMPLOYEES_SQL_TABLE_NAME ")";
  if (m_tbEmployee->rowCount() != 0)
  {
    filter += " AND " EMPLOYEE_SQL_TABLE_NAME "." SQL_COLID " NOT IN (";
    for (int i = 0; i != m_tbEmployee->rowCount(); ++i)
      filter += m_tbEmployee->item(i, 0)->data(Qt::UserRole).toString() + ",";
    filter.chop(1);
    filter += ")";
  }

  JDatabaseSelector dlg(EMPLOYEE_SQL_TABLE_NAME);
  dlg.getDatabase()->setFixedFilter(filter);
  dlg.exec();
  auto pt = dynamic_cast<Employee*>(dlg.getDatabase()->getCurrentItem());
  if (pt != nullptr)
    addEmployee(*pt);
}

void StoreView::addEmployee(const Employee& e)
{
  m_tbEmployee->insertRow(m_tbEmployee->rowCount());
  auto pt = new QTableWidgetItem(e.m_form.m_name);
  pt->setFlags(pt ->flags() & ~Qt::ItemFlag::ItemIsEditable);
  pt->setData(Qt::UserRole, e.m_id.get());
  m_tbEmployee->setItem(m_tbEmployee->rowCount() - 1, 0, pt);
}

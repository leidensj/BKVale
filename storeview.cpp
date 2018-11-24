#include "storeview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "storeemployeetablewidget.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

StoreView::StoreView(QWidget* parent)
  : JItemView(STORE_SQL_TABLE_NAME, parent)
  , m_edName(nullptr)
  , m_personPicker(nullptr)
  , m_addressPicker(nullptr)
  , m_phonePicker(nullptr)
  , m_employeeTable(nullptr)
  , m_btnAddEmployee(nullptr)
  , m_btnRemoveEmployee(nullptr)
{
  m_edName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
  m_edName->setPlaceholderText(tr("*"));

  m_personPicker = new JDatabasePicker(PERSON_SQL_TABLE_NAME,
                                       tr("Cadastro"),
                                       QIcon(":/icons/res/person.png"));
  m_personPicker->getDatabase()->setFixedFilter(PERSON_FILTER_COMPANY);

  m_employeeTable = new StoreEmployeeTableWidget;

  m_btnAddEmployee = new QPushButton;
  m_btnAddEmployee->setFlat(true);
  m_btnAddEmployee->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAddEmployee->setIconSize(QSize(24, 24));

  m_btnRemoveEmployee = new QPushButton;
  m_btnRemoveEmployee->setFlat(true);
  m_btnRemoveEmployee->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemoveEmployee->setIconSize(QSize(24, 24));

  QFormLayout* tablayout = new QFormLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addRow(tr("Nome:"), m_edName);
  tablayout->addRow(tr("Cadastro:"), m_personPicker);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  QVBoxLayout* employeebuttonlayout = new QVBoxLayout;
  employeebuttonlayout->setContentsMargins(0, 0, 0, 0);
  employeebuttonlayout->setAlignment(Qt::AlignTop);
  employeebuttonlayout->addWidget(m_btnAddEmployee);
  employeebuttonlayout->addWidget(m_btnRemoveEmployee);

  QHBoxLayout* employeelayout = new QHBoxLayout;
  employeelayout->setAlignment(Qt::AlignTop);
  employeelayout->addWidget(m_employeeTable);
  employeelayout->addLayout(employeebuttonlayout);

  QFrame* tabemployee = new QFrame;
  tabemployee->setLayout(employeelayout);

  m_tab->addTab(tabframe,
                QIcon(":/icons/res/store.png"),
                tr("Loja"));

  m_tab->addTab(tabemployee,
                QIcon(":/icons/res/employee.png"),
                tr("Funcionários"));

  QObject::connect(m_btnAddEmployee,
                   SIGNAL(clicked(bool)),
                   m_employeeTable,
                   SLOT(addItem()));

  QObject::connect(m_btnRemoveEmployee,
                   SIGNAL(clicked(bool)),
                   m_employeeTable,
                   SLOT(removeItem()));

  QObject::connect(m_employeeTable,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));
}

void StoreView::create()
{
  selectItem(Store());
  m_edName->setFocus();
}

const JItem& StoreView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
  m_ref.m_person.m_id = m_personPicker->getId();
  m_ref.m_name = m_edName->text();
  for (int i = 0; i != m_employeeTable->rowCount(); ++i)
    m_ref.m_vEmployee.push_back(dynamic_cast<const StoreEmployee&>(m_employeeTable->getItem(i)));
  return m_ref;
}

void StoreView::setItem(const JItem& o)
{
  const Store& ref = static_cast<const Store&>(o);
  m_currentId = o.m_id;
  m_edName->setText(ref.m_name);
  m_personPicker->setItem(ref.m_person);
  m_employeeTable->removeAllItems();
  for (int i = 0; i != ref.m_vEmployee.size(); ++i)
    m_employeeTable->addItem(ref.m_vEmployee.at(i));
}

void StoreView::updateControls()
{
  m_btnRemoveEmployee->setEnabled(m_employeeTable->isValidCurrentRow());
}

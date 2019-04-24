#include "employeeview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>
#include "jspinbox.h"
#include "jtimeedit.h"
#include <QPushButton>
#include "timeinterval.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QRadioButton>
#include "jaddremovebuttons.h"
#include "formwidget.h"

EmployeeView::EmployeeView(QWidget* parent)
  : JItemView(EMPLOYEE_SQL_TABLE_NAME, parent)
  , m_formInfo(nullptr)
  , m_formDetails(nullptr)
  , m_formPhone(nullptr)
  , m_formAddress(nullptr)
  , m_storePicker(nullptr)
  , m_edPincode(nullptr)
  , m_cbNoteEdit(nullptr)
  , m_cbNoteRemove(nullptr)
  , m_tbHours(nullptr)
  , m_btnAddRemove(nullptr)
{
  m_formInfo = new FormInfoWidget;
  m_formDetails = new FormDetailsWidget;
  m_formPhone = new FormPhoneWidget;
  m_formAddress= new FormAddressWidget;
  m_storePicker = new JDatabasePicker(STORE_SQL_TABLE_NAME);
  m_edPincode = new JLineEdit(JLineEdit::Input::Alphanumeric);
  m_cbNoteEdit = new QCheckBox;
  m_cbNoteEdit->setText(tr("Criar e editar"));
  m_cbNoteEdit->setIcon(QIcon(":/icons/res/file.png"));
  m_cbNoteRemove = new QCheckBox;
  m_cbNoteRemove->setText(tr("Remover"));
  m_cbNoteRemove->setIcon(QIcon(":/icons/res/remove.png"));
  m_tbHours = new QTableWidget();
  m_tbHours->setColumnCount(2);
  QStringList headers;
  headers << "Início" << "Fim";
  m_tbHours->setHorizontalHeaderLabels(headers);
  m_tbHours->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
  m_tbHours->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
  m_tbHours->setSelectionBehavior(QAbstractItemView::SelectItems);
  m_tbHours->setSelectionMode(QAbstractItemView::SingleSelection);
  m_btnAddRemove = new JAddRemoveButtons;

  QFormLayout* lt = new QFormLayout;
  lt->addRow(m_storePicker->getText() + ":", m_storePicker);
  lt->addRow(tr("Código PIN:"), m_edPincode);
  lt->addRow(tr("Vales:"), m_cbNoteEdit);
  lt->addRow("", m_cbNoteRemove);

  QFrame* fr = new QFrame;
  fr->setLayout(lt);

  QVBoxLayout* ltHours = new QVBoxLayout;
  ltHours->addWidget(m_btnAddRemove);
  ltHours->addWidget(m_tbHours);

  QFrame* frHours = new QFrame;
  frHours->setLayout(ltHours);

  m_tab->addTab(m_formInfo, QIcon(":/icons/res/resume.png"), tr("Informações"));
  m_tab->addTab(fr, QIcon(":/icons/res/employee.png"), tr("Funcionário"));
  m_tab->addTab(frHours, QIcon(":/icons/res/clock.png"), tr("Horário"));
  m_tab->addTab(m_formDetails, QIcon(":/icons/res/details.png"), tr("Detalhes"));
  m_tab->addTab(m_formPhone, QIcon(":/icons/res/phone.png"), tr("Telefone"));
  m_tab->addTab(m_formAddress, QIcon(":/icons/res/address.png"), tr("Endereço"));

  connect(m_btnAddRemove->m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(addHour()));
  connect(m_btnAddRemove->m_btnRemove, SIGNAL(clicked(bool)), this, SLOT(removeHour()));
  connect(m_tbHours, SIGNAL(itemSelectionChanged()), this, SLOT(updateControls()));
  connect(m_formInfo, SIGNAL(userTypeChangedSignal(bool)), m_formDetails, SLOT(switchUserType(bool)));

  m_formInfo->m_lblCreationDate->hide();
  m_formInfo->m_dtCreationDate->hide();
  m_formInfo->m_lblType->hide();
  m_formInfo->m_type->hide();
  m_formInfo->setCompany(false);

  updateControls();
}

void EmployeeView::create()
{
  selectItem(Employee());
}

const JItemSQL& EmployeeView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
  m_formInfo->fillForm(m_ref.m_form);
  m_formDetails->fillForm(m_ref.m_form);
  m_formPhone->fillForm(m_ref.m_form);
  m_formAddress->fillForm(m_ref.m_form);
  m_ref.m_store.m_id = m_storePicker->getId();
  m_ref.m_pincode = m_edPincode->text();
  m_ref.m_bNoteEdit = m_cbNoteEdit->isChecked();
  m_ref.m_bNoteRemove = m_cbNoteRemove->isChecked();
  for (int i = 0; i != m_tbHours->rowCount(); ++i)
  {
    TimeInterval t;
    t.m_tmBegin = dynamic_cast<JTimeEdit*>(m_tbHours->cellWidget(i, 0))->time();
    t.m_tmEnd = dynamic_cast<JTimeEdit*>(m_tbHours->cellWidget(i, 1))->time();
    m_ref.m_hours.push_back(t);
  }
  return m_ref;
}

void EmployeeView::setItem(const JItemSQL& o)
{
  auto ref = static_cast<const Employee&>(o);
  m_currentId = o.m_id;
  m_formInfo->setForm(ref.m_form);
  m_formDetails->setForm(ref.m_form);
  m_formPhone->setForm(ref.m_form);
  m_formAddress->setForm(ref.m_form);
  m_storePicker->setItem(ref.m_store);
  m_edPincode->setText(ref.m_pincode);
  m_cbNoteEdit->setChecked(ref.m_bNoteEdit);
  m_cbNoteRemove->setChecked(ref.m_bNoteRemove);
  m_tbHours->setRowCount(0);
  for (int i = 0; i != ref.m_hours.size(); ++i)
  {
    addHour();
    dynamic_cast<JTimeEdit*>(m_tbHours->cellWidget(i, 0))->setTime(ref.m_hours.at(i).m_tmBegin);
    dynamic_cast<JTimeEdit*>(m_tbHours->cellWidget(i, 1))->setTime(ref.m_hours.at(i).m_tmEnd);
  }
}

void EmployeeView::addHour()
{
  JTimeEdit* begin = new JTimeEdit;
  JTimeEdit* end = new JTimeEdit;
  m_tbHours->insertRow(m_tbHours->rowCount());
  int row = m_tbHours->rowCount() - 1;
  m_tbHours->setCellWidget(row, 0, begin);
  m_tbHours->setCellWidget(row, 1, end);
  m_tbHours->setCurrentCell(row, 0, QItemSelectionModel::ClearAndSelect);
  begin->setFocus();
  updateControls();
}

void EmployeeView::removeHour()
{
  if (m_tbHours->currentRow() >= 0)
    m_tbHours->removeRow(m_tbHours->currentRow());
  updateControls();
}

void EmployeeView::updateControls()
{
  m_btnAddRemove->m_btnRemove->setEnabled(m_tbHours->currentRow() >= 0);
}

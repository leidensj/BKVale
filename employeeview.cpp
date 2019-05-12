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
#include "jtable.h"
#include <QHeaderView>
#include <QRadioButton>
#include <QTreeWidget>
#include "jaddremovebuttons.h"
#include "formwidget.h"

EmployeeView::EmployeeView(QWidget* parent)
  : JItemView(EMPLOYEE_SQL_TABLE_NAME, parent)
  , m_formInfo(nullptr)
  , m_formDetails(nullptr)
  , m_formPhone(nullptr)
  , m_formAddress(nullptr)
  , m_edPincode(nullptr)
  , m_trPermissions(nullptr)
  , m_triNoteEdit(nullptr)
  , m_triNoteRemove(nullptr)
  , m_tbHours(nullptr)
  , m_btnAddRemove(nullptr)
{
  m_formInfo = new FormInfoWidget;
  m_formDetails = new FormDetailsWidget;
  m_formPhone = new FormPhoneWidget;
  m_formAddress= new FormAddressWidget;
  m_edPincode = new JLineEdit(JLineEdit::Input::Alphanumeric);
  m_tbHours = new JTable;
  m_tbHours->setColumnCount(2);
  QStringList headers;
  headers << "Início" << "Fim";
  m_tbHours->setHorizontalHeaderLabels(headers);
  m_tbHours->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
  m_tbHours->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
  m_tbHours->setSelectionBehavior(QAbstractItemView::SelectItems);
  m_tbHours->setSelectionMode(QAbstractItemView::SingleSelection);
  m_btnAddRemove = new JAddRemoveButtons;

  m_trPermissions = new QTreeWidget;
  m_trPermissions->setColumnCount(1);
  m_trPermissions->header()->setVisible(false);
  auto root = new QTreeWidgetItem;
  root->setText(0, tr("Vales"));
  root->setIcon(0, QIcon(":/icons/res/note.png"));
  m_trPermissions->insertTopLevelItem(0, root);
  m_triNoteEdit = new QTreeWidgetItem;
  m_triNoteRemove = new QTreeWidgetItem;
  m_triNoteEdit->setText(0, tr("Criar/editar"));
  m_triNoteEdit->setIcon(0, QIcon(":/icons/res/file.png"));
  m_triNoteEdit->setCheckState(0, Qt::Unchecked);
  m_triNoteRemove->setText(0, tr("Remover"));
  m_triNoteRemove->setIcon(0, QIcon(":/icons/res/remove.png"));
  m_triNoteRemove->setCheckState(0, Qt::Unchecked);
  root->addChild(m_triNoteEdit);
  root->addChild(m_triNoteRemove);
  root->setExpanded(true);

  m_formInfo->addWidget(tr("Código PIN:"), m_edPincode);

  QVBoxLayout* ltHours = new QVBoxLayout;
  ltHours->addWidget(m_btnAddRemove);
  ltHours->addWidget(m_tbHours);

  QFrame* frHours = new QFrame;
  frHours->setLayout(ltHours);

  m_tab->addTab(m_formInfo, QIcon(":/icons/res/resume.png"), tr("Informações"));
  m_tab->addTab(m_formDetails, QIcon(":/icons/res/details.png"), tr("Detalhes"));
  m_tab->addTab(m_trPermissions, QIcon(":/icons/res/usershield.png"), tr("Permissões"));
  m_tab->addTab(frHours, QIcon(":/icons/res/clock.png"), tr("Horário"));
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
  m_tab->setCurrentIndex(0);
  m_formInfo->m_edName->setFocus();
}

const JItemSQL& EmployeeView::getItem() const
{
  m_ref.clear(false);
  m_formInfo->fillForm(m_ref.m_form);
  m_formDetails->fillForm(m_ref.m_form);
  m_formPhone->fillForm(m_ref.m_form);
  m_formAddress->fillForm(m_ref.m_form);
  m_ref.m_pincode = m_edPincode->text();
  m_ref.m_bNoteEdit = m_triNoteEdit->checkState(0) == Qt::Checked;
  m_ref.m_bNoteRemove = m_triNoteRemove->checkState(0) == Qt::Checked;
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
  m_ref = static_cast<const Employee&>(o);
  m_formInfo->setForm(m_ref.m_form);
  m_formDetails->setForm(m_ref.m_form);
  m_formPhone->setForm(m_ref.m_form);
  m_formAddress->setForm(m_ref.m_form);
  m_edPincode->setText(m_ref.m_pincode);
  m_triNoteEdit->setCheckState(0, m_ref.m_bNoteEdit ? Qt::Checked : Qt::Unchecked);
  m_triNoteRemove->setCheckState(0, m_ref.m_bNoteRemove ? Qt::Checked : Qt::Unchecked);
  m_tbHours->setRowCount(0);
  for (int i = 0; i != m_ref.m_hours.size(); ++i)
  {
    addHour();
    dynamic_cast<JTimeEdit*>(m_tbHours->cellWidget(i, 0))->setTime(m_ref.m_hours.at(i).m_tmBegin);
    dynamic_cast<JTimeEdit*>(m_tbHours->cellWidget(i, 1))->setTime(m_ref.m_hours.at(i).m_tmEnd);
  }
}

void EmployeeView::addHour()
{
  auto begin = new QTableWidgetItem("00:00");
  auto end = new QTableWidgetItem("00:00");
  begin->setToolTip(tr("Hora e minutos hh:mm"));
  end->setToolTip(tr("Hora e minutos hh:mm"));
  m_tbHours->insertRow(m_tbHours->rowCount());
  int row = m_tbHours->rowCount() - 1;
  m_tbHours->setItem(row, 0, begin);
  m_tbHours->setItem(row, 1, end);
  m_tbHours->setCurrentItem(begin);
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
  for (int i = 0; i != m_tbHours->rowCount(); ++i)
  {
    QTime begin = QTime::fromString(m_tbHours->item(i, 0)->text());
    QTime end = QTime::fromString(m_tbHours->item(i, 1)->text());
    QString str = begin.toString("HH:mm");
    m_tbHours->item(i, 0)->setText(str.isEmpty() ? "00:00" : str);
    str = end.toString("HH:mm");
    m_tbHours->item(i, 1)->setText(str.isEmpty() ? "00:00" : str);
  }
}

Id EmployeeView::getId() const
{
  return m_ref.m_id;
}

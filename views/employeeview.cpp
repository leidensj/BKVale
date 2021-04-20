#include "employeeview.h"
#include "controls/databasepicker.h"
#include "widgets/jlineedit.h"
#include "widgets/jspinbox.h"
#include "widgets/jtimeedit.h"
#include "widgets/jaddremovebuttons.h"
#include "tables/employeehourstable.h"
#include "items/jitemhelper.h"
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include "timeinterval.h"
#include <QHeaderView>
#include <QRadioButton>
#include <QTreeWidget>
#include "editors/formeditor.h"

EmployeeView::EmployeeView(QWidget* parent)
  : JItemView(EMPLOYEE_SQL_TABLE_NAME, parent)
  , m_formInfo(nullptr)
  , m_formDetails(nullptr)
  , m_formPhone(nullptr)
  , m_formAddress(nullptr)
  , m_edPincode(nullptr)
  , m_trPermissions(nullptr)
  , m_tbHours(nullptr)
  , m_btnAddRemove(nullptr)
{
  addViewButton(IMAGE_SQL_TABLE_NAME);

  m_formInfo = new FormInfoEditor;
  m_formDetails = new FormDetailsEditor;
  m_formPhone = new FormPhoneEditor;
  m_formAddress= new FormAddressEditor;
  m_edPincode = new JLineEdit(Text::Input::Alphanumeric, false);
  m_btnAddRemove = new JAddRemoveButtons;
  m_tbHours = new EmployeeHoursTable(m_btnAddRemove);

  m_trPermissions = new QTreeWidget;
  m_trPermissions->setColumnCount(1);
  m_trPermissions->header()->setVisible(false);
  auto rootCreateEdit = new QTreeWidgetItem;
  rootCreateEdit->setText(0, tr("Criar/editar"));
  rootCreateEdit->setIcon(0, QIcon(":/icons/res/file.png"));
  rootCreateEdit->setFlags(rootCreateEdit->flags() | Qt::ItemIsAutoTristate);
  auto rootRemove = new QTreeWidgetItem;
  rootRemove->setText(0, tr("Remover"));
  rootRemove->setIcon(0, QIcon(":/icons/res/remove.png"));
  rootRemove->setFlags(rootRemove->flags() | Qt::ItemIsAutoTristate);
  m_trPermissions->insertTopLevelItem(0, rootCreateEdit);
  m_trPermissions->insertTopLevelItem(1, rootRemove);
  createTreeItem(Functionality::Idx::Purchase, rootCreateEdit);
  createTreeItem(Functionality::Idx::Coupon, rootCreateEdit);
  createTreeItem(Functionality::Idx::Purchase, rootRemove);
  createTreeItem(Functionality::Idx::Coupon, rootRemove);
  rootCreateEdit->setExpanded(true);
  rootRemove->setExpanded(true);

  QHBoxLayout* ltPincode = new QHBoxLayout;
  ltPincode->setContentsMargins(0, 0, 0, 0);
  ltPincode->addWidget(new QLabel(tr("Código PIN:")));
  ltPincode->addWidget(m_edPincode);
  QVBoxLayout* ltPermissions = new QVBoxLayout;
  ltPermissions->addLayout(ltPincode);
  ltPermissions->addWidget(m_trPermissions);

  QFrame* frPermissions = new QFrame;
  frPermissions->setLayout(ltPermissions);

  QVBoxLayout* ltHours = new QVBoxLayout;
  ltHours->addWidget(m_btnAddRemove);
  ltHours->addWidget(m_tbHours);

  QFrame* frHours = new QFrame;
  frHours->setLayout(ltHours);

  m_tab->addTab(m_formInfo, QIcon(":/icons/res/resume.png"), tr("Informações"));
  m_tab->addTab(m_formDetails, QIcon(":/icons/res/details.png"), tr("Detalhes"));
  m_tab->addTab(frPermissions, QIcon(":/icons/res/usershield.png"), tr("Permissões"));
  m_tab->addTab(frHours, QIcon(":/icons/res/clock.png"), tr("Horário"));
  m_tab->addTab(m_formPhone, QIcon(":/icons/res/phone.png"), tr("Telefone"));
  m_tab->addTab(m_formAddress, QIcon(":/icons/res/address.png"), tr("Endereço"));

  connect(m_formInfo, SIGNAL(userTypeChangedSignal(bool)), m_formDetails, SLOT(switchUserType(bool)));

  m_formInfo->m_lblCreationDate->hide();
  m_formInfo->m_dtCreationDate->hide();
  m_formInfo->m_lblType->hide();
  m_formInfo->m_type->hide();
  m_formInfo->setCompany(false);

  setFocusWidgetOnClear(m_formInfo->m_edName);
  clear();
}

void EmployeeView::createTreeItem(Functionality::Idx idx, QTreeWidgetItem* parent)
{
  if (parent != nullptr)
  {
    auto pt = new QTreeWidgetItem;
    pt->setText(0, JItemHelper::text(idx));
    pt->setIcon(0, QIcon(JItemHelper::icon(idx)));
    pt->setCheckState(0, Qt::Unchecked);
    pt->setData(0, Qt::UserRole, (int)idx);
    parent->addChild(pt);
  }
}

void EmployeeView::getItem(JItemSQL& o) const
{
  Employee& _o = dynamic_cast<Employee&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_form.m_id = m_formId;
  m_formInfo->fillForm(_o.m_form);
  m_formDetails->fillForm(_o.m_form);
  m_formPhone->fillForm(_o.m_form);
  m_formAddress->fillForm(_o.m_form);
  _o.m_pincode = m_edPincode->text();
  auto ptCreateEdit = m_trPermissions->topLevelItem(0);
  for (int i = 0; i != ptCreateEdit->childCount(); ++i)
    _o.setPermissionToCreateEdit((Functionality::Idx)ptCreateEdit->child(i)->data(0, Qt::UserRole).toInt(), ptCreateEdit->child(i)->checkState(0) == Qt::Checked);
  auto ptRemove = m_trPermissions->topLevelItem(1);
  for (int i = 0; i != ptRemove->childCount(); ++i)
    _o.setPermissionToRemove((Functionality::Idx)ptRemove->child(i)->data(0, Qt::UserRole).toInt(), ptRemove->child(i)->checkState(0) == Qt::Checked);
  m_tbHours->getHours(_o.m_hours);
}

void EmployeeView::setItem(const JItemSQL& o)
{
  const Employee& _o = dynamic_cast<const Employee&>(o);
  m_formId = _o.m_form.m_id;
  m_formInfo->setForm(_o.m_form);
  m_formDetails->setForm(_o.m_form);
  m_formPhone->setForm(_o.m_form);
  m_formAddress->setForm(_o.m_form);
  m_edPincode->setText(_o.m_pincode);
  m_tbHours->setHours(_o.m_hours);

  QTreeWidgetItemIterator it(m_trPermissions);
  while (*it)
  {
    (*it)->setCheckState(0, Qt::Unchecked);
    ++it;
  }

  {
    auto ptCreateEdit = m_trPermissions->topLevelItem(0);
    QMapIterator<Functionality::Idx, bool> i(_o.m_createEditPermissions);
    while (i.hasNext())
    {
      i.next();
      for (int j = 0; j != ptCreateEdit->childCount(); ++j)
        if (ptCreateEdit->child(j)->data(0, Qt::UserRole).toInt() == (int)i.key())
          ptCreateEdit->child(j)->setCheckState(0, i.value() ? Qt::Checked : Qt::Unchecked);
    }
  }

  {
    auto ptRemove = m_trPermissions->topLevelItem(1);
    QMapIterator<Functionality::Idx, bool> i(_o.m_removePermissions);
    while (i.hasNext())
    {
      i.next();
      for (int j = 0; j != ptRemove->childCount(); ++j)
        if (ptRemove->child(j)->data(0, Qt::UserRole).toInt() == (int)i.key())
          ptRemove->child(j)->setCheckState(0, i.value() ? Qt::Checked : Qt::Unchecked);
    }
  }
}

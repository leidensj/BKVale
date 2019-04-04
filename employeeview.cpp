#include "employeeview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>

EmployeeView::EmployeeView(QWidget* parent)
  : JItemView(CATEGORY_SQL_TABLE_NAME, parent)
  , m_formPicker(nullptr)
  , m_edPincode(nullptr)
  , m_cbNoteEdit(nullptr)
  , m_cbNoteRemove(nullptr)
{
  m_formPicker = new JDatabasePicker(FORM_SQL_TABLE_NAME);
  m_edPincode = new JLineEdit(JLineEdit::Input::Alphanumeric);
  m_cbNoteEdit = new QCheckBox;
  m_cbNoteEdit->setText(tr("Criar e editar"));
  m_cbNoteEdit->setIcon(QIcon(":/icons/res/file.png"));
  m_cbNoteRemove = new QCheckBox;
  m_cbNoteRemove->setText(tr("Remover"));
  m_cbNoteRemove->setIcon(QIcon(":/icons/res/remove.png"));

  QFormLayout* lt = new QFormLayout;
  lt->addRow(tr("Cadastro:"), m_formPicker);
  lt->addRow(tr("Código PIN:"), m_edPincode);
  lt->addWidget(new QLabel(tr("Notas")));
  lt->addRow("", m_cbNoteEdit);
  lt->addRow("", m_cbNoteRemove);

  QFrame* frame = new QFrame;
  frame->setLayout(lt);

  m_tab->addTab(frame, QIcon(":/icons/res/employee.png"), tr("Funcionário"));
}

void EmployeeView::create()
{
  selectItem(Employee());
}

const JItemSQL& EmployeeView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
  m_ref.m_form.m_id = m_formPicker->getId();
  m_ref.m_pincode = m_edPincode->text();
  m_ref.m_bNoteEdit = m_cbNoteEdit->isChecked();
  m_ref.m_bNoteRemove = m_cbNoteRemove->isChecked();
  return m_ref;
}

void EmployeeView::setItem(const JItemSQL& o)
{
  auto ref = static_cast<const Employee&>(o);
  m_currentId = o.m_id;
  m_formPicker->setItem(ref.m_form);
  m_edPincode->setText(ref.m_pincode);
  m_cbNoteEdit->setChecked(ref.m_bNoteEdit);
  m_cbNoteRemove->setChecked(ref.m_bNoteRemove);
}

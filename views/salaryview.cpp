#include "salaryview.h"
#include "widgets/jlineedit.h"
#include "controls/databasepicker.h"
#include "tables/salaryemployeestable.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

SalaryView::SalaryView(QWidget* parent)
  : JItemView(SALARY_SQL_TABLE_NAME, parent)
  , m_edName(nullptr)
  , m_imagePicker(nullptr)
  , m_table(nullptr)
{
  addViewButton(IMAGE_SQL_TABLE_NAME);
  addViewButton(EMPLOYEE_SQL_TABLE_NAME);

  m_edName = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edName->setPlaceholderText(tr("*"));
  m_edName->setMaxLength(SALARY_MAX_NAME_LENGTH);

  m_imagePicker = new DatabasePicker(IMAGE_SQL_TABLE_NAME);

  QFormLayout* ltForm = new QFormLayout;
  ltForm->addRow(tr("Nome:"), m_edName);
  ltForm->addRow(tr("Imagem:"), m_imagePicker);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltForm);

  m_table = new SalaryEmployeesTable;
  QVBoxLayout* ltTable = new QVBoxLayout;
  ltTable->addWidget(m_table);

  QFrame* tabframe2 = new QFrame;
  tabframe2->setLayout(ltTable);

  m_tab->addTab(tabframe, QIcon(":/icons/res/salary.png"), tr("Salário"));
  m_tab->addTab(tabframe2, QIcon(":/icons/res/employee.png"), tr("Funcionários"));
  setFocusWidgetOnClear(m_edName);
  clear();
}

void SalaryView::getItem(JItemSQL& o) const
{
  Salary& _o = dynamic_cast<Salary&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_image.m_id = m_imagePicker->getFirstId();
  _o.m_name = m_edName->text();
  m_table->get(_o.m_vse);
}

void SalaryView::setItem(const JItemSQL& o)
{
  const Salary& _o = static_cast<const Salary&>(o);
  m_edName->setText(_o.m_name);
  m_imagePicker->addItem(_o.m_image);
  m_table->set(_o.m_vse);

  Ids ids;
  QString error;
  bool ok = Employee::SQL_select_all(ids, error);
  if (!ok)
  {
    QMessageBox::critical(this, tr("Erro"), tr("Erro ao buscar funcionários, por favor tente novamente.\n%1").arg(error), QMessageBox::Ok);
    return;
  }

  for (int i = 0; i != ids.size(); ++i)
  {
    bool bfound = false;
    for (int j = 0; j != _o.m_vse.size(); ++j)
    {
      if (ids.at(i) == _o.m_vse.at(j).m_employee.m_id)
      {
        bfound = true;
        break;
      }
    }
    if (!bfound)
    {
      SalaryEmployee se;
      se.m_employee.m_id = ids.at(i);
      ok = se.m_employee.SQL_select(error);
      if (!ok)
      {
        QMessageBox::critical(this, tr("Erro"), tr("Erro ao buscar funcionário, por favor tente novamente.\n%1").arg(error), QMessageBox::Ok);
        continue;
      }
      m_table->addRow(se);
    }
  }
  m_table->order();
}

#include "salaryformulaview.h"
#include "widgets/jlineedit.h"
#include "widgets/jplaintextedit.h"
#include "controls/databasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

SalaryFormulaView::SalaryFormulaView(QWidget* parent)
  : JItemView(SALARY_FORMULA_SQL_TABLE_NAME, parent)
  , m_edName(nullptr)
  , m_imagePicker(nullptr)
  , m_edFormula(nullptr)
  , m_edDescription(nullptr)
{
  addViewButton(IMAGE_SQL_TABLE_NAME);
  addViewButton(SALARY_SQL_TABLE_NAME);

  m_edName = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edName->setPlaceholderText(tr("*"));
  m_edName->setMaxLength(SALARY_FORMULA_MAX_NAME_LENGTH);

  m_imagePicker = new DatabasePicker(IMAGE_SQL_TABLE_NAME);

  m_edFormula = new JLineEdit(Text::Input::All, true);

  m_edDescription = new JPlainTextEdit(false);

  QFormLayout* ltForm = new QFormLayout;
  ltForm->addRow(tr("Nome:"), m_edName);
  ltForm->addRow(tr("Imagem:"), m_imagePicker);
  ltForm->addRow(tr("Fórmula:"), m_edFormula);
  ltForm->addRow(tr("Descrição:"), m_edDescription);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltForm);

  m_tab->addTab(tabframe, QIcon(":/icons/res/salaryformula.png"), tr("Fórmula de Salário"));
  setFocusWidgetOnClear(m_edName);
  clear();
}

void SalaryFormulaView::getItem(JItemSQL& o) const
{
  SalaryFormula& _o = dynamic_cast<SalaryFormula&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_image.m_id = m_imagePicker->getFirstId();
  _o.m_name = m_edName->text();
  _o.m_formula = m_edFormula->text();
  _o.m_description = m_edDescription->toPlainText();
}

void SalaryFormulaView::setItem(const JItemSQL& o)
{
  const SalaryFormula& _o = static_cast<const SalaryFormula&>(o);
  m_edName->setText(_o.m_name);
  m_imagePicker->addItem(_o.m_image);
  m_edFormula->setText(_o.m_formula);
  m_edDescription->setPlainText(_o.m_description);
}

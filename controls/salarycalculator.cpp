#include "salarycalculator.h"
#include <QFormLayout>
#include "databasepicker.h"
#include "widgets/jlineedit.h"
#include "items/jitemhelper.h"
#include "items/employee.h"
#include "items/salary.h"
#include "items/salaryformula.h"
#include <QDateTimeEdit>
#include <QMessageBox>
#include <QPushButton>

SalaryCalculator::SalaryCalculator(QWidget* parent)
 : QDialog(parent)
 , m_employee(nullptr)
 , m_formula(nullptr)
 , m_dti(nullptr)
 , m_dtf(nullptr)
 , m_result(nullptr)
 , m_calculate(nullptr)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(JItemHelper::text(Functionality::Idx::SalaryCalculator));
  setWindowIcon(QIcon(JItemHelper::icon(Functionality::Idx::SalaryCalculator)));

  m_employee = new DatabasePicker(EMPLOYEE_SQL_TABLE_NAME);
  m_formula = new DatabasePicker(SALARY_FORMULA_SQL_TABLE_NAME);
  m_dti = new QDateTimeEdit;
  m_dtf = new QDateTimeEdit;
  m_result = new JExpLineEdit(Data::Type::Money);
  m_result->setReadOnly(true);
  m_result->setToolTip(tr("Resultado"));

  m_dti->setDisplayFormat("dd/MM/yyyy HH:mm");
  m_dtf->setDisplayFormat("dd/MM/yyyy HH:mm");
  m_dti->setDateTime(QDateTime::currentDateTime());
  m_dtf->setDateTime(QDateTime::currentDateTime());

  m_calculate = new QPushButton;
  m_calculate->setIconSize(QSize(24,24));
  m_calculate->setIcon(QIcon(":/icons/res/process.png"));
  m_calculate->setFlat(true);
  m_calculate->setToolTip(tr("Calcular"));

  auto form = new QFormLayout;
  form->addRow(tr("Funcionário:"), m_employee);
  form->addRow(tr("Fórmula:"), m_formula);
  form->addRow(tr("Início:"), m_dti);
  form->addRow(tr("Fim:"), m_dtf);

  auto hbox = new QHBoxLayout;
  hbox->addWidget(m_calculate);
  hbox->addWidget(m_result);
  auto vbox = new QVBoxLayout;
  vbox->addLayout(form);
  vbox->addLayout(hbox);
  setLayout(vbox);

  connect(m_calculate, SIGNAL(clicked(bool)), this, SLOT(calculate()));
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
}

void SalaryCalculator::displayError(const QString& error)
{
  QMessageBox::critical(this, tr("Erro"), tr("Erro ao calcular salário: %1").arg(error), QMessageBox::Ok);
}

#include <qdebug.h>

void SalaryCalculator::calculate()
{
  Employee e;
  e.m_id = m_employee->getFirstId();
  SalaryFormula sf;
  sf.m_id = m_formula->getFirstId();

  QString error;
  e.SQL_select(error);
  sf.SQL_select(error);

  QVector<QString> vname;
  QVector<double> vvalue;
  Salary::SQL_select_all_employee_salaries(e.m_id, vname, vvalue, error);
  QString formula = sf.m_formula;
  if (vname.size() == vvalue.size())
    for (int i = 0; i != vname.size(); ++i)
      formula.replace(vname.at(i), Data::strFmt(vvalue.at(i)));

  qDebug() << "DIAS " << m_dti->date().daysTo(m_dtf->date());
  qDebug() << "HORAS " << m_dti->dateTime().secsTo(m_dtf->dateTime())/3600;
  qDebug() << "MINUTOS " << m_dti->dateTime().secsTo(m_dtf->dateTime())/60;

  formula.replace("DIAS", Data::strFmt(m_dti->date().daysTo(m_dtf->date())));
  formula.replace("HORAS", Data::strFmt(m_dti->dateTime().secsTo(m_dtf->dateTime())/3600));
  formula.replace("MINUTOS", Data::strFmt(m_dti->dateTime().secsTo(m_dtf->dateTime())/60));
}

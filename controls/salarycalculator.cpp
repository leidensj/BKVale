#include "salarycalculator.h"
#include <QFormLayout>
#include "databasepicker.h"
#include "widgets/jlineedit.h"
#include "items/jitemhelper.h"
#include "items/employee.h"
#include "items/salary.h"
#include "items/salaryformula.h"
#include "tables/salarycalculatortable.h"
#include "escpos.h"
#include "escposprinter.h"
#include <QMessageBox>
#include <QDateTimeEdit>
#include <QPushButton>

SalaryCalculator::SalaryCalculator(QWidget* parent)
 : QDialog(parent)
 , m_employee(nullptr)
 , m_formula(nullptr)
 , m_dti(nullptr)
 , m_dtf(nullptr)
 , m_table(nullptr)
 , m_calculate(nullptr)
 , m_print(nullptr)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(JItemHelper::text(Functionality::Idx::SalaryCalculator));
  setWindowIcon(QIcon(JItemHelper::icon(Functionality::Idx::SalaryCalculator)));

  m_employee = new DatabasePicker(EMPLOYEE_SQL_TABLE_NAME, true);
  m_formula = new DatabasePicker(SALARY_FORMULA_SQL_TABLE_NAME);
  m_dti = new QDateTimeEdit;
  m_dtf = new QDateTimeEdit;
  m_table = new SalaryCalculatorTable;

  m_dti->setDisplayFormat("dd/MM/yyyy HH:mm");
  m_dtf->setDisplayFormat("dd/MM/yyyy HH:mm");
  m_dti->setDateTime(QDateTime::currentDateTime());
  m_dtf->setDateTime(QDateTime::currentDateTime());

  m_calculate = new QPushButton;
  m_calculate->setIconSize(QSize(24,24));
  m_calculate->setIcon(QIcon(":/icons/res/process.png"));
  m_calculate->setFlat(true);
  m_calculate->setToolTip(tr("Calcular"));

  m_print = new QPushButton;
  m_print->setIconSize(QSize(24,24));
  m_print->setIcon(QIcon(":/icons/res/printer.png"));
  m_print->setFlat(true);
  m_print->setToolTip(tr("Imprimir"));
  m_print->setCheckable(true);

  auto form = new QFormLayout;
  form->addRow(tr("Fórmula:"), m_formula);
  form->addRow(tr("Funcionário:"), m_employee);
  form->addRow(tr("Início:"), m_dti);
  form->addRow(tr("Fim:"), m_dtf);

  auto hbox = new QHBoxLayout;
  hbox->addWidget(m_calculate);
  hbox->addWidget(m_print);
  hbox->setAlignment(Qt::AlignLeft);
  auto vbox = new QVBoxLayout;
  vbox->addLayout(form);
  vbox->addLayout(hbox);
  vbox->addWidget(m_table);
  setLayout(vbox);

  connect(m_calculate, SIGNAL(clicked(bool)), this, SLOT(calculate()));
  connect(m_employee, SIGNAL(changedSignal()), this, SLOT(update()));
  connect(m_formula, SIGNAL(changedSignal()), this, SLOT(update()));
  connect(m_dti, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(update()));
  connect(m_dtf, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(update()));
  connect(m_table, SIGNAL(changedSignal(bool)), this, SLOT(update()));
  update();
}

void SalaryCalculator::update()
{
  bool ok = m_dti->dateTime() <= m_dtf->dateTime() && m_employee->getFirstId().isValid() && m_formula->getFirstId().isValid();
  m_calculate->setEnabled(ok);
}

void SalaryCalculator::calculate()
{
  Ids eids = m_employee->getIds();
  QString error;
  QVector<Employee> ve;
  for (const auto& id : eids)
  {
    Employee e;
    e.m_id = id;
    if (e.SQL_select(error))
      ve.push_back(e);
  }

  SalaryFormula sf;
  sf.m_id = m_formula->getFirstId();
  sf.SQL_select(error);

  QVector<SalaryCalculatorResult> vscr;
  for (auto& e : ve)
  {
    QVector<QString> vname;
    QVector<double> vvalue;
    Salary::SQL_select_all_employee_salaries(e.m_id, vname, vvalue, error);
    QString formula = sf.m_formula;
    if (vname.size() == vvalue.size())
      for (int i = 0; i != vname.size(); ++i)
        formula.replace(vname.at(i), Data::strFmt(vvalue.at(i)));
    formula.replace("DIAS", Data::strFmt(m_dti->date().daysTo(m_dtf->date())));
    formula.replace("HORAS", Data::strFmt(double(m_dti->dateTime().secsTo(m_dtf->dateTime()))/3600.0));
    formula.replace("MINUTOS", Data::strFmt(double(m_dti->dateTime().secsTo(m_dtf->dateTime()))/60.0));
    SalaryCalculatorResult scr;
    scr.name = e.name();
    scr.value = formula;
    vscr.push_back(scr);
  }
  m_table->set(vscr);
  if (m_print->isChecked())
    print();
}

void SalaryCalculator::print()
{
  EscPos ep;
  QVector<SalaryCalculatorResult> vsre;
  m_table->get(vsre);
  for (auto& r : vsre)
  {
    ep.align(true);
    ep.str(tr("Funcionário\n"));
    ep.expand(true);
    ep.str(r.name);
    ep.expand(false);
    ep.str(tr("\n\nFórmula de Salario\n"));
    ep.expand(true);
    ep.str(m_formula->getFirstName());
    ep.expand(false);
    ep.str(tr("\n\nInício: "));
    ep.bold(true);
    ep.str(m_dti->dateTime().toString("dd/MM/yyyy HH:mm"));
    ep.bold(false);
    ep.str(tr("\nFim: "));
    ep.bold(true);
    ep.str(m_dtf->dateTime().toString("dd/MM/yyyy HH:mm"));
    ep.bold(false);
    ep.str(tr("\n\nResultado\n"));
    ep.doublefont(true);
    ep.str(r.value);
    ep.cut(true);
  }


  EscPosPrinter printer;
  QString error;
  bool ok = printer.connectToPrinter(error);
  if (ok)
    ok = printer.printRawData(ep.m_ba, error);
  if (!ok)
    QMessageBox::warning(this, tr("Erro ao imprimir"), error, QMessageBox::Ok);
}

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

  connect(m_calculate, SIGNAL(clicked(bool)), this, SLOT(process()));
  connect(m_print, SIGNAL(clicked(bool)), this, SLOT(print()));
  connect(m_employee, SIGNAL(changedSignal()), this, SLOT(update()));
  connect(m_formula, SIGNAL(changedSignal()), this, SLOT(update()));
  connect(m_dti, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(update()));
  connect(m_dtf, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(update()));
  connect(m_table, SIGNAL(changedSignal(int, int)), this, SLOT(calculate(int,int)));
  update();
}

void SalaryCalculator::update()
{
  bool ok = m_dti->dateTime() <= m_dtf->dateTime() && m_employee->getFirstId().isValid() && m_formula->getFirstId().isValid();
  m_calculate->setEnabled(ok);
}

void SalaryCalculator::process()
{
  Ids eids = m_employee->getIds();
  Names enames = m_employee->getNames();
  if (eids.size() != enames.size())
    return;

  QString error;
  SalaryFormula sf;
  sf.m_id = m_formula->getFirstId();
  sf.SQL_select(error);

  QVector<SalaryCalculatorResult> v;
  for (int i = 0; i != eids.size(); ++i)
  {
    SalaryCalculatorResult o;
    o.id = eids.at(i).get();
    o.name = enames.at(i);
    o.dtBegin = m_dti->date();
    o.tmBegin = m_dti->time();
    o.dtEnd = m_dtf->date();
    o.tmEnd = m_dtf->time();
    v.push_back(o);
  }
  m_table->set(v);
}

void SalaryCalculator::calculate(int row, int /*column*/)
{
  if (!m_table->isValidRow(row))
    return;

  QString error;
  SalaryFormula sf;
  sf.m_id = m_formula->getFirstId();
  sf.SQL_select(error);

  SalaryCalculatorResult o;
  m_table->get(row, o);
  if (QDateTime(o.dtBegin, o.tmBegin) > QDateTime(o.dtEnd, o.tmEnd))
  {
    o.value = "0";
    m_table->set(row, o);
    return;
  }

  Names snames;
  Values svalues;
  Salary::SQL_select_all_employee_salaries(o.id, snames, svalues, error);
  QString formula = sf.m_formula;
  if (snames.size() == svalues.size())
    for (int i = 0; i != snames.size(); ++i)
      formula.replace(snames.at(i), Data::strFmt(svalues.at(i)));
  formula.replace("DIAS", Data::strFmt(o.dtBegin.daysTo(o.dtEnd)));
  formula.replace("HORAS", Data::strFmt(QDateTime(o.dtBegin, o.tmBegin).secsTo(QDateTime(o.dtEnd, o.tmEnd))/3600.0));
  formula.replace("MINUTOS", Data::strFmt(QDateTime(o.dtBegin, o.tmBegin).secsTo(QDateTime(o.dtEnd, o.tmEnd))/60.0));
  o.value = formula;
  m_table->set(row, o);
}

void SalaryCalculator::print()
{
  if (!m_table->hasItems())
    return;

  EscPos ep;
  QVector<SalaryCalculatorResult> v;
  m_table->get(v);
  for (auto& o : v)
  {
    ep.align(true);
    ep.str(tr("Funcionário\n"));
    ep.expand(true);
    ep.str(o.name);
    ep.expand(false);
    ep.str(tr("\n\nFórmula de Salario\n"));
    ep.expand(true);
    ep.str(m_formula->getFirstName());
    ep.expand(false);
    ep.str(tr("\n\nInício: "));
    ep.bold(true);
    ep.str(QDateTime(o.dtBegin, o.tmBegin).toString("dd/MM/yyyy HH:mm"));
    ep.bold(false);
    ep.str(tr("\nFim: "));
    ep.bold(true);
    ep.str(QDateTime(o.dtEnd, o.tmEnd).toString("dd/MM/yyyy HH:mm"));
    ep.bold(false);
    ep.str(tr("\n\nResultado\n"));
    ep.doublefont(true);
    ep.str(o.value);
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

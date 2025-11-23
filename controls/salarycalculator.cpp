#include "salarycalculator.h"
#include <QFormLayout>
#include "databasepicker.h"
#include "widgets/jlineedit.h"
#include "items/jitemhelper.h"
#include "items/employee.h"
#include "items/salary.h"
#include "items/salaryformula.h"
#include "tables/salarycalculatortable.h"
#include "widgets/jaddremovebuttons.h"
#include "escpos.h"
#include "escposprinter.h"
#include <QMessageBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QDialogButtonBox>

SalaryCalculatorBatch::SalaryCalculatorBatch(QWidget* parent)
 : QDialog(parent)
 , m_formula(nullptr)
 , m_employee(nullptr)
 , m_dti(nullptr)
 , m_dtf(nullptr)
 , m_bb(nullptr)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(JItemHelper::text(Functionality::Idx::SalaryCalculator));
  setWindowIcon(QIcon(JItemHelper::icon(Functionality::Idx::SalaryCalculator)));

  m_employee = new DatabasePicker(EMPLOYEE_SQL_TABLE_NAME, true);
  m_formula = new DatabasePicker(SALARY_FORMULA_SQL_TABLE_NAME);
  m_dti = new QDateTimeEdit;
  m_dtf = new QDateTimeEdit;

  m_dti->setDisplayFormat("dd/MM/yyyy HH:mm");
  m_dtf->setDisplayFormat("dd/MM/yyyy HH:mm");
  m_dti->setDateTime(QDateTime::currentDateTime());
  m_dtf->setDateTime(QDateTime::currentDateTime());
  m_bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  auto form = new QFormLayout;
  form->addRow(tr("Fórmula:"), m_formula);
  form->addRow(tr("Funcionário:"), m_employee);
  form->addRow(tr("Início:"), m_dti);
  form->addRow(tr("Fim:"), m_dtf);

  auto vbox = new QVBoxLayout;
  vbox->addLayout(form);
  vbox->addWidget(m_bb);
  setLayout(vbox);

  connect(m_bb, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(m_bb, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(m_employee, SIGNAL(changedSignal()), this, SLOT(update()));
  connect(m_formula, SIGNAL(changedSignal()), this, SLOT(update()));
  connect(m_dti, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(update()));
  connect(m_dtf, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(update()));
  update();
}

void SalaryCalculatorBatch::update()
{
  bool ok = m_dti->dateTime() <= m_dtf->dateTime() && m_employee->getFirstId().isValid() && m_formula->getFirstId().isValid();
  m_bb->button(QDialogButtonBox::Ok)->setEnabled(ok);
}

QDateTime SalaryCalculatorBatch::begin() const
{
  return m_dti->dateTime();
}

QDateTime SalaryCalculatorBatch::end() const
{
  return m_dtf->dateTime();
}

Id SalaryCalculatorBatch::formula() const
{
  return m_formula->getFirstId();
}

QString SalaryCalculatorBatch::formulaName() const
{
  return m_formula->getFirstName();
}

Ids SalaryCalculatorBatch::employees() const
{
  return m_employee->getIds();
}

Names SalaryCalculatorBatch::employeesNames() const
{
  return m_employee->getNames();
}

SalaryCalculator::SalaryCalculator(QWidget* parent)
 : QDialog(parent)
 , m_table(nullptr)
 , m_print(nullptr)
 , m_batch(nullptr)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(JItemHelper::text(Functionality::Idx::SalaryCalculator));
  setWindowIcon(QIcon(JItemHelper::icon(Functionality::Idx::SalaryCalculator)));

  m_print = new QPushButton;
  m_print->setIconSize(QSize(24,24));
  m_print->setIcon(QIcon(":/icons/res/printer.png"));
  m_print->setFlat(true);
  m_print->setToolTip(tr("Imprimir"));

  m_batch = new QPushButton;
  m_batch->setIconSize(QSize(24,24));
  m_batch->setIcon(QIcon(":/icons/res/addmitem.png"));
  m_batch->setFlat(true);
  m_batch->setToolTip(tr("Adicionar Vários"));

  m_btns = new JAddRemoveButtons;
  m_table = new SalaryCalculatorTable(m_btns);
  m_btns->m_lt->addWidget(m_batch);
  m_btns->m_lt->addWidget(m_print);

  auto vbox = new QVBoxLayout;
  vbox->addWidget(m_btns);
  vbox->addWidget(m_table);
  setLayout(vbox);
  resize(640, 480);

  connect(m_print, SIGNAL(clicked(bool)), this, SLOT(print()));
  connect(m_batch, SIGNAL(clicked(bool)), this, SLOT(batch()));
  connect(m_table, SIGNAL(changedSignal(bool)), this, SLOT(update()));
  update();
}

void SalaryCalculator::update()
{
  m_print->setEnabled(m_table->isValidCurrentItem());
}

void SalaryCalculator::batch()
{
  SalaryCalculatorBatch dlg;
  if (dlg.exec())
  {
    Ids eids = dlg.employees();
    Names enames = dlg.employeesNames();
    if (eids.size() != enames.size())
      return;
    Id fid = dlg.formula();
    QString fname = dlg.formulaName();
    QDateTime begin = dlg.begin();
    QDateTime end = dlg.end();
    for (int i = 0; i != eids.size(); ++i)
      m_table->addRow(fid.get(), fname, eids.at(i).get(), enames.at(i), begin, end);
  }
}

void SalaryCalculator::print()
{
  int row = m_table->currentRow();
  if (!m_table->isValidRow(row))
    return;

  QString fname;
  QString ename;
  QDateTime begin;
  QDateTime end;
  double value;
  m_table->row(row, fname, ename, begin, end, value);
  EscPos ep;
  ep.align(true);
  ep.str(tr("Fórmula de Salario\n"));
  ep.expand(true);
  ep.str(fname);
  ep.expand(false);
  ep.str(tr("\n\nFuncionário\n"));
  ep.expand(true);
  ep.str(ename);
  ep.expand(false);
  ep.str(tr("\n\nInício: "));
  ep.bold(true);
  ep.str(begin.toString("dd/MM/yyyy HH:mm"));
  ep.bold(false);
  ep.str(tr("\nFim: "));
  ep.bold(true);
  ep.str(end.toString("dd/MM/yyyy HH:mm"));
  ep.bold(false);
  ep.str(tr("\n\nResultado\n"));
  ep.doublefont(true);
  ep.str(Data::strMoney(value));
  ep.cut(true);


  EscPosPrinter printer;
  QString error;
  bool ok = printer.connectToPrinter(error);
  if (ok)
    ok = printer.printRawData(ep.m_ba, error);
  if (!ok)
    QMessageBox::warning(this, tr("Erro ao imprimir"), error, QMessageBox::Ok);
}

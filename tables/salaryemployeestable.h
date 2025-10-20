#ifndef SALARYEMPLOYEESTABLE_H
#define SALARYEMPLOYEESTABLE_H

#include "jtable.h"
#include "items/salaryemployee.h"

class SalaryEmployeesTable : public JTable
{
  Q_OBJECT

  enum class Column
  {
    Employee,
    Salary
  };

public:
  explicit SalaryEmployeesTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<SalaryEmployee>& v) const;
  void set(const QVector<SalaryEmployee>& v);

public slots:
  void addRow();
  void addRowAndActivate();
  void addRow(const SalaryEmployee& o);
  void order();
  void update();
};

#endif // SALARYEMPLOYEESTABLE_H

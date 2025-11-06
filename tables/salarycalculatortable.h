#ifndef SALARYCALCULATORTABLE_H
#define SALARYCALCULATORTABLE_H

#include "jtable.h"

struct SalaryCalculatorResult
{
  QString name;
  QString value;
};

class SalaryCalculatorTable : public JTable
{
  Q_OBJECT

public:

  enum class Column
  {
    Name,
    Value
  };

  explicit SalaryCalculatorTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<SalaryCalculatorResult>& v) const;
  void set(const QVector<SalaryCalculatorResult>& v, bool bClear = true);

public slots:
  void addRow();
  void addRowAndActivate();
};

#endif

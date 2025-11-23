#ifndef SALARYCALCULATORTABLE_H
#define SALARYCALCULATORTABLE_H

#include "jtable.h"

class SalaryCalculatorTable : public JTable
{
  Q_OBJECT

public:

  enum class Column
  {
    Formula,
    Employee,
    DateBegin,
    TimeBegin,
    DateEnd,
    TimeEnd,
    Value
  };

  explicit SalaryCalculatorTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  bool isValidCurrentItem() const;
  void row(int row, QString& fname, QString& ename, QDateTime& begin, QDateTime& end, double& value) const;

private slots:
  void update(int row, int column);

public slots:
  void addRow();
  void addRow(const qlonglong fid, const QString& fname, const qlonglong eid, const QString& ename, const QDateTime& begin, const QDateTime& end);
  void addRowAndActivate();
};

#endif

#ifndef SALARYCALCULATOR_H
#define SALARYCALCULATOR_H

#include <QDialog>
#include "items/jitem.h"

class DatabasePicker;
class QDateTimeEdit;
class JExpLineEdit;
class QPushButton;
class QLabel;
class SalaryCalculatorTable;
class JAddRemoveButtons;
class QDialogButtonBox;

class SalaryCalculatorBatch : public QDialog
{
  Q_OBJECT

public:
  explicit SalaryCalculatorBatch(QWidget* parent = nullptr);
  QDateTime begin() const;
  QDateTime end() const;
  Id formula() const;
  QString formulaName() const;
  Ids employees() const;
  Names employeesNames() const;

private slots:
  void update();

private:
  DatabasePicker* m_formula;
  DatabasePicker* m_employee;
  QDateTimeEdit* m_dti;
  QDateTimeEdit* m_dtf;
  QDialogButtonBox* m_bb;
};

class SalaryCalculator : public QDialog
{
  Q_OBJECT

public:
  explicit SalaryCalculator(QWidget* parent = nullptr);

private slots:
  void batch();
  void print();
  void update();

private:
  SalaryCalculatorTable* m_table;
  JAddRemoveButtons* m_btns;
  QPushButton* m_print;
  QPushButton* m_batch;
};

#endif // SALARYCALCULATOR_H

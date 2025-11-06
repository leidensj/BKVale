#ifndef SALARYCALCULATOR_H
#define SALARYCALCULATOR_H

#include <QDialog>

class DatabasePicker;
class QDateTimeEdit;
class JExpLineEdit;
class QPushButton;
class QLabel;
class SalaryCalculatorTable;

class SalaryCalculator : public QDialog
{
  Q_OBJECT

public:
  explicit SalaryCalculator(QWidget* parent = nullptr);

private slots:
  void update();
  void calculate();
  void print();

private:
  DatabasePicker* m_employee;
  DatabasePicker* m_formula;
  QDateTimeEdit* m_dti;
  QDateTimeEdit* m_dtf;
  SalaryCalculatorTable* m_table;
  QPushButton* m_calculate;
  QPushButton* m_print;
};

#endif // SALARYCALCULATOR_H

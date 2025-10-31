#ifndef SALARYCALCULATOR_H
#define SALARYCALCULATOR_H

#include <QDialog>

class DatabasePicker;
class QDateTimeEdit;
class JExpLineEdit;
class QPushButton;
class QLabel;

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
  QLabel* m_minutes;
  QLabel* m_hours;
  QLabel* m_days;
  JExpLineEdit* m_result;
  QPushButton* m_calculate;
  QPushButton* m_print;
};

#endif // SALARYCALCULATOR_H

#ifndef SALARYVIEW_H
#define SALARYVIEW_H

#include "items/salary.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class DatabasePicker;
class SalaryEmployeesTable;

class SalaryView : public JItemView
{
  Q_OBJECT

public:
  explicit SalaryView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  JLineEdit* m_edName;
  DatabasePicker* m_imagePicker;
  SalaryEmployeesTable* m_table;
};

#endif // SALARYVIEW_H

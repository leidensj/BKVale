#ifndef SALARYFORMULAVIEW_H
#define SALARYFORMULAVIEW_H

#include "items/salaryformula.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class DatabasePicker;
class JPlainTextEdit;

class SalaryFormulaView : public JItemView
{
  Q_OBJECT

public:
  explicit SalaryFormulaView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  JLineEdit* m_edName;
  DatabasePicker* m_imagePicker;
  JLineEdit* m_edFormula;
  JPlainTextEdit* m_edDescription;
};

#endif // SALARYFORMULAVIEW_H

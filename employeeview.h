#ifndef EMPLOYEEVIEW_H
#define EMPLOYEEVIEW_H

#include "employee.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;
class QCheckBox;

class EmployeeView : public JItemView
{
  Q_OBJECT

public:
  explicit EmployeeView(QWidget* parent = 0);
  const JItemSQL& getItem() const;

public slots:
  void create();

private:
  mutable Employee m_ref;
  JDatabasePicker* m_formPicker;
  JLineEdit* m_edPincode;
  QCheckBox* m_cbNoteEdit;
  QCheckBox* m_cbNoteRemove;
  void setItem(const JItemSQL& o);
};

#endif // EMPLOYEEVIEW_H

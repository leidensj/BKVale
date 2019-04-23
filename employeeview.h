#ifndef EMPLOYEEVIEW_H
#define EMPLOYEEVIEW_H

#include "employee.h"
#include "jitemview.h"
#include <QVector>

class QPushButton;
class JLineEdit;
class JDatabasePicker;
class QCheckBox;
class QTableWidget;
class JTimeEdit;
class JSpinBox;
class QLabel;
class JAddRemoveButtons;

class EmployeeView : public JItemView
{
  Q_OBJECT

public:
  explicit EmployeeView(QWidget* parent = 0);
  const JItemSQL& getItem() const;

public slots:
  void create();

private slots:
  void addHour();
  void removeHour();
  void updateControls();

private:
  mutable Employee m_ref;
  JDatabasePicker* m_formPicker;
  JDatabasePicker* m_storePicker;
  JLineEdit* m_edPincode;
  QCheckBox* m_cbNoteEdit;
  QCheckBox* m_cbNoteRemove;
  QTableWidget* m_tbHours;
  JAddRemoveButtons* m_btnAddRemove;
  void setItem(const JItemSQL& o);
};

#endif // EMPLOYEEVIEW_H

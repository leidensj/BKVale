#ifndef EMPLOYEEVIEW_H
#define EMPLOYEEVIEW_H

#include "items/employee.h"
#include "jitemview.h"
#include <QVector>

class QPushButton;
class JLineEdit;
class JDatabasePicker;
class QCheckBox;
class QTableWidget;
class QTreeWidgetItem;
class JTimeEdit;
class JSpinBox;
class QLabel;
class JAddRemoveButtons;
class FormInfoEditor;
class FormDetailsEditor;
class FormPhoneEditor;
class FormAddressEditor;
class EmployeeHoursTable;
class QTreeWidget;
class QTableWidgetItem;

class EmployeeView : public JItemView
{
  Q_OBJECT

public:
  explicit EmployeeView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  Id m_formId;
  FormInfoEditor* m_formInfo;
  FormDetailsEditor* m_formDetails;
  FormPhoneEditor* m_formPhone;
  FormAddressEditor* m_formAddress;
  JLineEdit* m_edPincode;
  QTreeWidget* m_trPermissions;
  QTreeWidgetItem* m_triPurchaseEdit;
  QTreeWidgetItem* m_triPurchaseRemove;
  EmployeeHoursTable* m_tbHours;
  JAddRemoveButtons* m_btnAddRemove;
};

#endif // EMPLOYEEVIEW_H

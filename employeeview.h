#ifndef EMPLOYEEVIEW_H
#define EMPLOYEEVIEW_H

#include "items/employee.h"
#include "widgets/jitemview.h"
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
class FormInfoWidget;
class FormDetailsWidget;
class FormPhoneWidget;
class FormAddressWidget;
class JTable;
class QTreeWidget;
class QTableWidgetItem;

class EmployeeView : public JItemView
{
  Q_OBJECT

public:
  explicit EmployeeView(QWidget* parent = 0);

public slots:
  void create();
  const JItemSQL& getItem() const;
  Id getId() const;

protected slots:
  void setItem(const JItemSQL& o);

private slots:
  void addHour();
  void removeHour();
  void updateControls();
  void updateHoursTable(QTableWidgetItem* p);

private:
  mutable Employee m_ref;
  FormInfoWidget* m_formInfo;
  FormDetailsWidget* m_formDetails;
  FormPhoneWidget* m_formPhone;
  FormAddressWidget* m_formAddress;
  JLineEdit* m_edPincode;
  QTreeWidget* m_trPermissions;
  QTreeWidgetItem* m_triNoteEdit;
  QTreeWidgetItem* m_triNoteRemove;
  JTable* m_tbHours;
  JAddRemoveButtons* m_btnAddRemove;
};

#endif // EMPLOYEEVIEW_H

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
class FormInfoWidget;
class FormDetailsWidget;
class FormPhoneWidget;
class FormAddressWidget;
class JTable;
class QTreeWidget;

class EmployeeView : public JItemView
{
  Q_OBJECT

public:
  explicit EmployeeView(QWidget* parent = 0);
  const JItemSQL& getItem() const;
  Id getId() const;

public slots:
  void create();

private slots:
  void addHour();
  void removeHour();
  void updateControls();

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
  void setItem(const JItemSQL& o);
};

#endif // EMPLOYEEVIEW_H

#ifndef STOREVIEW_H
#define STOREVIEW_H

#include "items/store.h"
#include "widgets/jitemview.h"

class JTable;
class FormInfoWidget;
class FormDetailsWidget;
class FormPhoneWidget;
class FormAddressWidget;
class JAddRemoveButtons;

class StoreView : public JItemView
{
  Q_OBJECT

public:
  explicit StoreView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;
  void updateControls();
  void addEmployee();
  void addEmployee(const Employee& e);

protected slots:
  void setItem(const JItemSQL& o);

private:
  Id m_formId;
  FormInfoWidget* m_formInfo;
  FormDetailsWidget* m_formDetails;
  FormPhoneWidget* m_formPhone;
  FormAddressWidget* m_formAddress;
  JTable* m_tbEmployee;
  JAddRemoveButtons* m_btnAddRemove;
};

#endif // STOREVIEW_H

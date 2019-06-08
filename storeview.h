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
  const JItemSQL& getItem() const;
  Id getId() const;

public slots:
  void create();
  void updateControls();
  void addEmployee();
  void addEmployee(const Employee& e);

private:
  mutable Store m_ref;
  FormInfoWidget* m_formInfo;
  FormDetailsWidget* m_formDetails;
  FormPhoneWidget* m_formPhone;
  FormAddressWidget* m_formAddress;
  JTable* m_tbEmployee;
  JAddRemoveButtons* m_btnAddRemove;
  void setItem(const JItemSQL& o);
};

#endif // STOREVIEW_H

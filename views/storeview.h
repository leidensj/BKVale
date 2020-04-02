#ifndef STOREVIEW_H
#define STOREVIEW_H

#include "items/store.h"
#include "jitemview.h"

class StoreEmployeesTable;
class FormInfoEditor;
class FormDetailsEditor;
class FormPhoneEditor;
class FormAddressEditor;
class JAddRemoveButtons;

class StoreView : public JItemView
{
  Q_OBJECT

public:
  explicit StoreView(QWidget* parent = 0);

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
  StoreEmployeesTable* m_tbEmployee;
  JAddRemoveButtons* m_btnAddRemove;
};

#endif // STOREVIEW_H

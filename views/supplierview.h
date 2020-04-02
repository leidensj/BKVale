#ifndef SUPPLIERVIEW_H
#define SUPPLIERVIEW_H

#include "items/supplier.h"
#include "jitemview.h"

class FormInfoEditor;
class FormDetailsEditor;
class FormPhoneEditor;
class FormAddressEditor;

class SupplierView : public JItemView
{
  Q_OBJECT

public:
  explicit SupplierView(QWidget* parent = 0);

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
};

#endif // EMPLOYEEVIEW_H

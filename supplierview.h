#ifndef SUPPLIERVIEW_H
#define SUPPLIERVIEW_H

#include "items/supplier.h"
#include "widgets/jitemview.h"

class FormInfoWidget;
class FormDetailsWidget;
class FormPhoneWidget;
class FormAddressWidget;

class SupplierView : public JItemView
{
  Q_OBJECT

public:
  explicit SupplierView(QWidget* parent = 0);
  const JItemSQL& getItem() const;
  Id getId() const;

public slots:
  void create();

private:
  mutable Supplier m_ref;
  FormInfoWidget* m_formInfo;
  FormDetailsWidget* m_formDetails;
  FormPhoneWidget* m_formPhone;
  FormAddressWidget* m_formAddress;
  void setItem(const JItemSQL& o);
};

#endif // EMPLOYEEVIEW_H

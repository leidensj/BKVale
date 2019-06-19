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

public slots:
  void create();
  const JItemSQL& getItem() const;
  Id getId() const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  mutable Supplier m_ref;
  FormInfoWidget* m_formInfo;
  FormDetailsWidget* m_formDetails;
  FormPhoneWidget* m_formPhone;
  FormAddressWidget* m_formAddress;
};

#endif // EMPLOYEEVIEW_H

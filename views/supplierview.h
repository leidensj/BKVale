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
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  FormInfoWidget* m_formInfo;
  FormDetailsWidget* m_formDetails;
  FormPhoneWidget* m_formPhone;
  FormAddressWidget* m_formAddress;
};

#endif // EMPLOYEEVIEW_H

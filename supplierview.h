#ifndef SUPPLIERVIEW_H
#define SUPPLIERVIEW_H

#include "supplier.h"
#include "jitemview.h"

class JDatabasePicker;

class SupplierView : public JItemView
{
  Q_OBJECT

public:
  explicit SupplierView(QWidget* parent = 0);
  const JItemSQL& getItem() const;

public slots:
  void create();

private:
  mutable Supplier m_ref;
  JDatabasePicker* m_formPicker;
  void setItem(const JItemSQL& o);
};

#endif // EMPLOYEEVIEW_H

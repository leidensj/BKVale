#ifndef PRODUCTBARCODEVIEW_H
#define PRODUCTVIEW_H

#include <QFrame>
#include "productbarcode.h"
#include "jitemview.h"

class JLineEdit;
class JDatabasePicker;

class ProductBarcodeView : public JItemView
{
  Q_OBJECT

public:
  explicit ProductBarcodeView(QWidget* parent = 0);
  ~ProductBarcodeView();
 const JItem& getItem() const;

public slots:
  void create();

private:
  JDatabasePicker* m_productPicker;
  JLineEdit* m_edCode;

  void setItem(const JItem& o);
};

#endif // PRODUCTBARCODEVIEW_H

#ifndef PRODUCTVIEW_H
#define PRODUCTVIEW_H

#include <QFrame>
#include "product.h"
#include "category.h"
#include "jitemview.h"

class QLabel;
class JLineEdit;
class JDoubleSpinBox;
class QCheckBox;
class JDatabasePicker;
class ProductCodeTableWidget;

class ProductView : public JItemView
{
  Q_OBJECT

public:
  explicit ProductView(QWidget* parent = 0);
  ~ProductView();
  const JItemSQL& getItem() const;

public slots:
  void create();

private:
  mutable Product m_ref;
  JLineEdit* m_edName;
  JLineEdit* m_edUnity;
  JLineEdit* m_edDetails;
  QCheckBox* m_cbBuy;
  QCheckBox* m_cbSell;
  JDatabasePicker* m_categoryPicker;
  JDatabasePicker* m_imagePicker;
  ProductCodeTableWidget* m_tbCode;
  void setItem(const JItemSQL& o);
};

#endif // PRODUCTVIEW_H

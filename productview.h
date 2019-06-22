#ifndef PRODUCTVIEW_H
#define PRODUCTVIEW_H

#include <QFrame>
#include "items/product.h"
#include "items/category.h"
#include "widgets/jitemview.h"

class QLabel;
class JLineEdit;
class JDoubleSpinBox;
class QCheckBox;
class JDatabasePicker;
class ProductCodeTableWidget;
class JAddRemoveButtons;

class ProductView : public JItemView
{
  Q_OBJECT

public:
  explicit ProductView(QWidget* parent = 0);
  ~ProductView();

public slots:
  void create();
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  JLineEdit* m_edName;
  JLineEdit* m_edUnity;
  JLineEdit* m_edDetails;
  QCheckBox* m_cbBuy;
  QCheckBox* m_cbSell;
  JDatabasePicker* m_categoryPicker;
  JDatabasePicker* m_imagePicker;
  ProductCodeTableWidget* m_tbCode;
  JAddRemoveButtons* m_btns;
};

#endif // PRODUCTVIEW_H

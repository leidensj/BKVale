#ifndef PRODUCTVIEW_H
#define PRODUCTVIEW_H

#include <QFrame>
#include "items/product.h"
#include "items/category.h"
#include "jitemview.h"

class QLabel;
class JLineEdit;
class JDoubleSpinBox;
class QCheckBox;
class DatabasePicker;
class ProductCodeTable;
class JAddRemoveButtons;

class ProductView : public JItemView
{
  Q_OBJECT

public:
  explicit ProductView(QWidget* parent = 0);
  ~ProductView();

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);
  void editUnity();

private:
  JLineEdit* m_edName;
  JLineEdit* m_edUnity;
  JLineEdit* m_edDetails;
  QCheckBox* m_cbBuy;
  QCheckBox* m_cbSell;
  DatabasePicker* m_categoryPicker;
  DatabasePicker* m_imagePicker;
  ProductCodeTable* m_tbCode;
  JAddRemoveButtons* m_btns;
  QAction* m_actUnity;
};

#endif // PRODUCTVIEW_H

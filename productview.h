#ifndef PRODUCTVIEW_H
#define PRODUCTVIEW_H

#include <QFrame>
#include "product.h"
#include "category.h"

class QLabel;
class JLineEdit;
class QDoubleSpinBox;
class QPushButton;
class QCheckBox;
class JPicker;

class ProductView : public QFrame
{
  Q_OBJECT

public:
  explicit ProductView(QWidget* parent = 0);
  ~ProductView();
  Product getProduct() const;
  void setCategory(int id, const QString& name);
  void setImage(int id, const QString& text, const QByteArray& ar);

private slots:
  void emitSearchCategorySignal();
  void emitSearchImageSignal();
  void emitSaveSignal();
  void updateControls();

public slots:
  void setProduct(const Product& product);
  void create();

signals:
  searchCategorySignal();
  searchImageSignal();
  saveSignal();

private:
  Product m_currentProduct;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edName;
  JLineEdit* m_edUnity;
  JLineEdit* m_edPackageUnity;
  QDoubleSpinBox* m_spnPackageAmmount;
  JLineEdit* m_edDetails;
  JLineEdit* m_edCode;
  QCheckBox* m_cbAvailableAtNotes;
  QCheckBox* m_cbAvailableAtShop;
  QCheckBox* m_cbAvailableAtConsumption;
  QCheckBox* m_cbAvailableToBuy;
  QCheckBox* m_cbAvailableToSell;
  JPicker* m_categoryPicker;
  JPicker* m_imagePicker;
};

#endif // PRODUCTVIEW_H

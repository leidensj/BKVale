#ifndef PRODUCTVIEW_H
#define PRODUCTVIEW_H

#include <QFrame>
#include <QSqlDatabase>
#include "product.h"
#include "category.h"

class QLabel;
class JLineEdit;
class QDoubleSpinBox;
class QPushButton;
class QCheckBox;
class JDatabasePicker;
class JDatabase;

class ProductView : public QFrame
{
  Q_OBJECT

public:
  explicit ProductView(QWidget* parent = 0);
  ~ProductView();
  void setDatabase(QSqlDatabase db);
  Product getProduct() const;

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();

public slots:
  void setProduct(const Product& product);
  void create();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edName;
  JLineEdit* m_edUnity;
  JLineEdit* m_edPackageUnity;
  QDoubleSpinBox* m_spnPackageAmmount;
  JLineEdit* m_edDetails;
  QCheckBox* m_cbAvailableAtNotes;
  QCheckBox* m_cbAvailableAtShop;
  QCheckBox* m_cbAvailableAtConsumption;
  QCheckBox* m_cbAvailableToBuy;
  QCheckBox* m_cbAvailableToSell;
  JDatabasePicker* m_categoryPicker;
  JDatabasePicker* m_imagePicker;
  JDatabase* m_database;
};

#endif // PRODUCTVIEW_H

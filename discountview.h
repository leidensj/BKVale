#ifndef DISCOUNTVIEW_H
#define DISCOUNTVIEW_H

#include <QFrame>
#include "discount.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;
class JDatabase;
class QRadioButton;
class DiscountTableWidget;
class JDoubleSpinBox;
class JSpinBox;
class QDateEdit;
class QCheckBox;
class QPlainTextEdit;
class QDockWidget;

class DiscountView : public QFrame
{
  Q_OBJECT

public:
  explicit DiscountView(QWidget* parent = nullptr);
  Discount getDiscount() const;

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void searchProduct();
  void removeProduct();
  void setProduct(const Product& product, bool bNewProduct);
  void showSearch();
  void redeem();

public slots:
  void setDiscount(const Discount& o);
  void create();
  void updateControls();
  Discount save();

signals:
  void saveSignal();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSearch;
  QPushButton* m_btnRedeem;
  JLineEdit* m_edCode;
  QCheckBox* m_cbExpires;
  QDateEdit* m_dtExp;
  QRadioButton* m_rdValue;
  QRadioButton* m_rdPercentage;
  QRadioButton* m_rdProduct;
  JDoubleSpinBox* m_spnValue;
  JDoubleSpinBox* m_spnPercentage;
  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;
  DiscountTableWidget* m_table;
  JDatabase* m_database;
  QCheckBox* m_cbUsed;
  QPlainTextEdit* m_teDescription;
  QDockWidget* m_dock;
};

#endif // DISCOUNTVIEW_H

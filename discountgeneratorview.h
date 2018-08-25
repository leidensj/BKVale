#ifndef DISCOUNTGENERATORVIEW_H
#define DISCOUNTGENERATORVIEW_H

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

class DiscountGeneratorView : public QFrame
{
  Q_OBJECT

public:
  explicit DiscountGeneratorView(QWidget* parent = nullptr);
  Discount getDiscount() const;

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();
  void updateControls();
  void searchProduct();
  void removeProduct();
  void setProduct(const Product& product, bool bNewProduct);

public slots:
  void setDiscount(const Discount& o);
  void create();

signals:
  void saveSignal();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
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
};

#endif // DISCOUNTGENERATORVIEW_H

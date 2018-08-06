#ifndef PRODUCTBARCODEVIEW_H
#define PRODUCTVIEW_H

#include <QFrame>
#include "productbarcode.h"

class QPushButton;
class JLineEdit;
class JDatabasePicker;
class JDatabase;
class QTabWidget;

class ProductBarcodeView : public QFrame
{
  Q_OBJECT

public:
  explicit ProductBarcodeView(QWidget* parent = 0);
  ~ProductBarcodeView();
  ProductBarcode getProductBarcode() const;

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();

public slots:
  void setProductBarcode(const ProductBarcode& barcode);
  void create();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JDatabasePicker* m_productPicker;
  JLineEdit* m_edCode;
  JDatabase* m_database;
  QTabWidget* m_tab;
};

#endif // PRODUCTBARCODEVIEW_H

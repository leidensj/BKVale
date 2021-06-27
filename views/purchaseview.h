#ifndef PURCHASEVIEW_H
#define PURCHASEVIEW_H

#include <QFrame>
#include <QStringList>
#include "items/purchase.h"
#include "items/employee.h"
#include "jitemview.h"

#define MAX_ITEMS 100

class JDateEdit;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QCheckBox;
class PurchaseProductTable;
class DatabasePicker;
class QDockWidget;
class JDatabase;
class QIODevice;
class JExpLineEdit;
class QPlainTextEdit;
class JDatePicker;
class QLabel;
class JLineEdit;
class QTableWidget;
class PaymentTable;
class QTableWidgetItem;
class QRadioButton;
class PurchaseFilter;
class QToolButton;
class QAction;
class JAddRemoveButtons;

class PaymentWidget : public QWidget
{
  Q_OBJECT

  QRadioButton* m_rdoCash;
  QRadioButton* m_rdoBonus;
  QRadioButton* m_rdoCredit;
  PaymentTable* m_tbPayment;
  QLabel* m_lblPurchaseTotal;
  QLabel* m_lblPaymentTotal;
  JAddRemoveButtons* m_btnAddRemove;

public:
  explicit PaymentWidget(QWidget* parent = nullptr);
  Purchase::PaymentMethod getPaymentMethod() const;
  QVector<PaymentPart> getPayments() const;
  void setPaymentMethod(Purchase::PaymentMethod o);
  void setPayments(const QVector<PaymentPart>& v);
  QIcon getIcon() const;
  QString getText() const;

public slots:
  void setPurchaseDate(const QDate& dt);
  void setPurchaseTotal(double total);

private slots:
  void fillCredit();
  void updateControls();
  void emitMethodChangedSignal();

signals:
  void isValidSignal(bool b);
  void methodChangedSignal();
};

class PurchaseView : public JItemView
{
  Q_OBJECT

public:
  explicit PurchaseView(QWidget *parent = 0);
  ~PurchaseView();
  void save();
  void setItem();

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void itemsRemoved(const Ids& ids);
  void setItem(const JItemSQL& o);

private:
  Id m_lastId;
  QPushButton* m_btnOpenLast;
  QSpinBox* m_snNumber;
  JDatePicker* m_dtPicker;
  JExpLineEdit* m_edTotal;
  DatabasePicker* m_supplierPicker;
  DatabasePicker* m_storePicker;
  PurchaseProductTable* m_table;
  JExpLineEdit* m_edDisccount;
  PaymentWidget* m_wPayment;
  QPlainTextEdit* m_teObservation;
  PurchaseFilter* m_filter;
  QToolButton* m_btnAddMenu;
  QAction* m_actAdd;
  QAction* m_actAddCode;
  QAction* m_actAddHistory;
  QPushButton* m_btnRemove;
  QLabel* m_lblViewerData;

private slots:
  void supplierChanged();
  void lastItemSelected();
  void updateControls();
  void showHistory();
  void print(Purchase& o);

signals:
  void changedSignal();
};

#endif // PURCHASEVIEW_H

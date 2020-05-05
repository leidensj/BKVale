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
class PurchaseTable;
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
class JAddRemoveButtons;
class PaymentTable;
class QTableWidgetItem;
class QRadioButton;
class PurchaseFilter;

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
  QVector<PaymentElement> getPayments() const;
  void setPaymentMethod(Purchase::PaymentMethod o);
  void setPayments(const QVector<PaymentElement>& v);
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
  bool save(Id& id);
  void setDate(const QDate& dt);
  QDate getDate() const;

public slots:
  void selectItem(const JItemSQL& o);
  void getItem(JItemSQL& o) const;

protected slots:
  void itemsRemoved(const QVector<Id>& ids);
  void setItem(const JItemSQL& o);

private:
  Id m_lastId;
  QPushButton* m_btnOpenLast;
  QPushButton* m_btnAddCode;
  JAddRemoveButtons* m_btnAddRemove;
  QSpinBox* m_snNumber;
  JDatePicker* m_dtPicker;
  JExpLineEdit* m_edTotal;
  DatabasePicker* m_supplierPicker;
  DatabasePicker* m_storePicker;
  PurchaseTable* m_table;
  JExpLineEdit* m_edDisccount;
  PaymentWidget* m_wPayment;
  QPlainTextEdit* m_teObservation;
  PurchaseFilter* m_filter;
  QPushButton* m_btnHistory;

private slots:
  void supplierChanged();
  void lastItemSelected();
  void updateControls();
  void showHistory();

signals:
  void changedSignal();
};

#endif // PURCHASEVIEW_H

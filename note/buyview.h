#ifndef NOTEVIEW_H
#define NOTEVIEW_H

#include <QFrame>
#include <QStringList>
#include "items/note.h"
#include "items/employee.h"
#include "widgets/jitemview.h"
#include <QDialog>

#define MAX_ITEMS 100

class JDateEdit;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QCheckBox;
class NoteTableWidget;
class JDatabasePicker;
class QDockWidget;
class JDatabase;
class QIODevice;
class JExpLineEdit;
class JPlainTextEdit;
class JDatePicker;
class QLabel;
class JLineEdit;
class QTableWidget;
class JAddRemoveButtons;
class JTable;
class QTableWidgetItem;
class QRadioButton;

class NoteDetailsDlg : public QDialog
{
  Q_OBJECT

  JPlainTextEdit* m_teDetails;

public:
  explicit NoteDetailsDlg(QWidget* parent = nullptr);
  void setDetails(const QString& str);
  QString getDetails() const;
};

class PaymentDlg : public QDialog
{
  Q_OBJECT

  QRadioButton* m_rdoCash;
  QRadioButton* m_rdoBonus;
  QRadioButton* m_rdoCredit;
  JTable* m_tbCredit;
  QLabel* m_lblNoteTotal;
  QLabel* m_lblPaymentTotal;
  JAddRemoveButtons* m_btnAddRemove;

  double m_noteTotal;
  QDate m_noteDate;

  enum class Column
  {
    Date,
    Value
  };

public:
  explicit PaymentDlg(QWidget* parent = nullptr);
  void fillNote(Note& o) const;
  void setNote(const Note& o);
  QIcon getIcon() const;
  QString getText() const;

private slots:
  void fillCredit();
  void updateControls();
  void addRow();
  void removeRow();
  void updateTable(QTableWidgetItem* p);

private:
  Note::PaymentMethod getPaymentMethod() const;
  QVector<PaymentItem> getPaymentItems() const;
  void setPaymentMethod(Note::PaymentMethod o);
  void setPaymentItems(const QVector<PaymentItem>& v);
  double computeTotal() const;
  bool isDatesValid() const;

signals:
  void isValidSignal(bool b);
};

class BuyView : public JItemView
{
  Q_OBJECT

public:
  explicit BuyView(QWidget *parent = 0);
  ~BuyView();

  const JItemSQL& getItem() const;
  Id getId() const;

  void addNoteItem(const NoteItem& noteItem);

protected slots:
  void create();
  void itemsRemoved(const QVector<Id>& ids);

private:
  Note m_currentNote;
  Id m_lastId;
  QPushButton* m_btnOpenLast;
  QPushButton* m_btnAddCode;
  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;
  QSpinBox* m_snNumber;
  JDatePicker* m_dtPicker;
  JExpLineEdit* m_edTotal;
  JDatabasePicker* m_supplierPicker;
  NoteTableWidget* m_table;
  JExpLineEdit* m_edDisccount;
  NoteDetailsDlg* m_dlgDetails;
  PaymentDlg* m_dlgPayment;
  JLineEdit* m_edEntries;
  JLineEdit* m_edSum;
  void setItem(const JItemSQL& o);

private slots:
  void itemSelected(const JItemSQL& jItem);
  void removeItem();
  void supplierChanged();
  void lastItemSelected();
  void addProduct();
  void openDetailsDialog();
  void openPaymentDialog();
  void updateControls();
  void updateStatistics();

signals:
  void changedSignal();
};

#endif // NOTEVIEW_H

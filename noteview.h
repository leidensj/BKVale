#ifndef NOTEVIEW_H
#define NOTEVIEW_H

#include <QFrame>
#include <QStringList>
#include "note.h"
#include "employee.h"
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
class QRadioButton;
class JAddRemoveButtons;

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

  QTableWidget* m_table;
  QRadioButton* m_rdoCredit;
  QRadioButton* m_rdoCash;
  QRadioButton* m_rdoBonus;
  JAddRemoveButtons* m_btnAddRemove;

  double m_noteTotal;
  QDate m_noteDate;

  double computeTotal() const;

  enum class Column
  {
    Date,
    Value
  };

public:
  explicit PaymentDlg(QWidget* parent = nullptr);
  QVector<Payment> getPayments() const;
  void setPayment(const QVector<Payment>& v);
  Payment::Method getMethod() const;
  void setMethod(Payment::Method m);
  void setNoteTotal(double total);
  void setNoteDate(const QDate& date);

private slots:
  void updateControls();
  void addRow();
  void removeRow();

signals:
  void isValidSignal(bool b);
};

class NoteView : public QFrame
{
  Q_OBJECT

public:
  explicit NoteView(QWidget *parent = 0);
  ~NoteView();
  Note getNote() const;
  void addNoteItem(const NoteItem& noteItem);

private:
  Id m_currentId;
  Id m_lastId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnOpenLast;
  QPushButton* m_btnSearch;
  QPushButton* m_btnAddCode;
  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;
  QSpinBox* m_snNumber;
  JDatePicker* m_dtPicker;
  JExpLineEdit* m_edTotal;
  JDatabasePicker* m_supplierPicker;
  NoteTableWidget* m_table;
  QPushButton* m_btnPayment;
  JDatabase* m_database;
  JExpLineEdit* m_edDisccount;
  QPushButton* m_btnDetails;
  NoteDetailsDlg* m_dlgDetails;
  PaymentDlg* m_dlgPayment;
  JLineEdit* m_edEntries;
  JLineEdit* m_edSum;
  QDialog* m_dlgDb;

private slots:
  void removeItem();
  void supplierChanged();
  void lastItemSelected();
  void itemSelected(const JItemSQL& jItem);
  void itemsRemoved(const QVector<Id>& ids);
  void addProduct();
  void openDetailsDialog();
  void openPaymentDialog();

public slots:
  void create();
  void setNote(const Note& note);
  void updateControls();
  Note save();

signals:
  void changedSignal();
};

#endif // NOTEVIEW_H

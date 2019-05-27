#ifndef NOTEVIEW_H
#define NOTEVIEW_H

#include <QFrame>
#include <QStringList>
#include "items/note.h"
#include "items/employee.h"
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

  QPushButton* m_btnCash;
  QPushButton* m_btnBonus;
  QPushButton* m_btnCredit;
  JExpLineEdit* m_edCash;
  JExpLineEdit* m_edBonus;
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
  Payment getPayment() const;
  void setPayment(const Payment& o);
  void setNoteTotal(double total);
  void setNoteDate(const QDate& date);
  void adjust();

private slots:
  void updateControls();
  void fillCash();
  void fillBonus();
  void fillCredit();
  void addRow();
  void removeRow();
  void updateTable(QTableWidgetItem* p);

private:
  Payment m_payment;

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
  Note m_currentNote;
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
  bool save(Id& id);

signals:
  void changedSignal();
};

#endif // NOTEVIEW_H

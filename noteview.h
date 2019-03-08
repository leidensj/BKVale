#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include "note.h"
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
class QPlainTextEdit;
class JDatePicker;
class QLabel;

class NoteDetailsDlg : public QDialog
{
  Q_OBJECT

  QPlainTextEdit* m_teDetails;

public:
  explicit NoteDetailsDlg(QWidget* parent = nullptr);
  void setDetails(const QString& str);
  QString getDetails() const;
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
  QLineEdit* m_edTotal;
  JDatabasePicker* m_supplierPicker;
  NoteTableWidget* m_table;
  QCheckBox* m_cbCash;
  JDatabase* m_database;
  JExpLineEdit* m_edDisccount;
  QPushButton* m_btnDetails;
  NoteDetailsDlg* m_dlgDetails;
  QLabel* m_lblEntries;
  QLabel* m_lblSum;

private slots:
  void removeItem();
  void supplierChanged();
  void lastItemSelected();
  void itemSelected(const JItem& jItem);
  void itemsRemoved(const QVector<Id>& ids);
  void addProduct();
  void openDetailsDialog();

public slots:
  void create();
  void setNote(const Note& note);
  void updateControls();
  Note save(Person& employee);

signals:
  void changedSignal();
};

#endif // BKFRAME_H

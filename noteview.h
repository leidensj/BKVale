#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include <QSqlDatabase>
#include "note.h"

#define MAX_ITEMS 100

class QTableWidgetItem;
class QDateEdit;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QLabel;
class QCheckBox;
class NoteTableWidget;
class JDatabasePicker;

class NoteView : public QFrame
{
  Q_OBJECT

public:
  explicit NoteView(QWidget *parent = 0);
  ~NoteView();
  void setDatabase(QSqlDatabase db);
  Note getNote() const;
  void setLastID(int lastID);
  int getLastID() const;
  void setProduct(int row, const Product& product);
  void addNoteItem(const NoteItem& noteItem);

private:
  QSqlDatabase m_db;
  QPushButton* m_btnCreate;
  QPushButton* m_btnOpenLast;
  QPushButton* m_btnSearch;
  QPushButton* m_btnSearchItem;
  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;
  QSpinBox* m_snNumber;
  QLabel* m_lblNumberStatus;
  QDateEdit* m_dtDate;
  QPushButton* m_btnToday;
  QLineEdit* m_edTotal;
  JDatabasePicker* m_supplierPicker;
  NoteTableWidget* m_table;
  QCheckBox* m_cbCash;
  int m_currentID;
  int m_lastID;

private slots:
  void setToday();
  void checkDate();
  void emitShowSearchSignal();
  void emitCreateSignal();
  void emitOpenLastSignal();
  void emitSearchNewProductSignal();
  void emitSearchProductSignal();
  void removeItem();
  void supplierEntered();

public slots:
  void create(int number);
  void setNote(const Note& note);
  void updateControls();

signals:
  void changedSignal();
  void showSearchSignal();
  void searchProductSignal(int row);
  void createSignal();
  void openLastSignal(int id);
};

#endif // BKFRAME_H

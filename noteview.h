#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include <QComboBox>
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

class NoteSupplierComboBox : public QComboBox
{
  Q_OBJECT

public:
  explicit NoteSupplierComboBox(QWidget* parent = 0);

protected:
  void keyPressEvent(QKeyEvent *event);

public slots:
  void toUpper();

signals:
  void supplierEnteredSignal();
};

class NoteView : public QFrame
{
  Q_OBJECT

public:
  explicit NoteView(QWidget *parent = 0);
  ~NoteView();
  bool isValid() const;
  Note getNote() const;
  void setLastID(int lastID);
  int getLastID() const;

private:
  QPushButton* m_btnCreate;
  QPushButton* m_btnOpenLast;
  QPushButton* m_btnSearch;
  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;
  QSpinBox* m_snNumber;
  QLabel* m_lblNumberStatus;
  QDateEdit* m_dtDate;
  QPushButton* m_btnToday;
  QLineEdit* m_edTotal;
  NoteSupplierComboBox* m_cbSupplier;
  NoteTableWidget* m_table;
  QCheckBox* m_cbCash;
  int m_currentID;
  int m_lastID;

private slots:
  void setToday();
  void checkDate();
  void emitChangedSignal();
  void emitShowSearchSignal();
  void emitCreateSignal();
  void emitOpenLastSignal();
  void addItem();
  void addItem(const NoteItem& noteItem);
  void removeItem();

public slots:
  void create(int number, const QStringList& suppliers);
  void supplierEntered();
  void setNote(const Note& note, int number, const QStringList& suppliers);
  void updateControls();

signals:
  void changedSignal();
  void showSearchSignal();
  void createSignal();
  void openLastSignal(int id);
};

#endif // BKFRAME_H

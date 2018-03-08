#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
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
class JPicker;

class NoteView : public QFrame
{
  Q_OBJECT

public:
  explicit NoteView(QWidget *parent = 0);
  ~NoteView();
  bool isValid() const;
  Note getNote() const;
  QVector<NoteItem> getNoteItems() const;
  void setLastID(int lastID);
  int getLastID() const;

private:
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
  JPicker* m_supplierPicker;
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
  void emitSearchSupplierSignal();
  void addItem();
  void addItem(const NoteItem& noteItem);
  void removeItem();

public slots:
  void create(int number);
  void setNote(const Note& note,
               int number,
               const Person& supplier,
               const QVector<NoteItem>& vItem,
               const QVector<Product>& vProduct);
  void updateControls();
  void setSupplier(int id, const QString& name, const QByteArray& arImage);

signals:
  void changedSignal();
  void showSearchSignal();
  void searchSupplierSignal();
  void createSignal();
  void openLastSignal(int id);
};

#endif // BKFRAME_H

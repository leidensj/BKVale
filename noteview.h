#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include <QComboBox>
#include <QTableWidget>
#include "note.h"

#define MAX_ITEMS 100

class QTableWidgetItem;
class QDateEdit;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QLabel;

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

class NoteTableWidget : public QTableWidget
{
  Q_OBJECT

public:
  explicit NoteTableWidget(QWidget* parent = 0);
  QString text(int row, int column) const;
  void setText(int row, int column, const QString& str);
  void getItems(QVector<NoteItem>& items) const;

protected:
  void keyPressEvent(QKeyEvent *event);
};

class NoteView : public QFrame
{
  Q_OBJECT

public:
  explicit NoteView(QWidget *parent = 0);
  ~NoteView();
  QStringList getItemDescriptions() const;
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
  QPushButton* m_btnCash;
  NoteSupplierComboBox* m_cbSupplier;
  NoteTableWidget* m_table;
  int m_currentID;
  int m_lastID;
  QString computeUnitValue(int row) const;
  QString computeSubTotal(int row) const;
  QString computeTotal() const;
  double evaluate(int row, int column) const;

private slots:
  void updateTable(int row, int column);
  void setToday();
  void checkDate();
  void emitChangedSignal();
  void emitShowSearchSignal();
  void emitCreateSignal();
  void emitOpenLastSignal();

public slots:
  void addItem();
  void removeItem();
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

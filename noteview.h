#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include "note.h"

#define MAX_ITEMS 100

class QDateEdit;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QCheckBox;
class NoteTableWidget;
class JDatabasePicker;
class QDockWidget;
class JDatabase;
class QIODevice;
class QPlainTextEdit;
class JExpLineEdit;

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
  QDateEdit* m_dtDate;
  QPushButton* m_btnToday;
  QLineEdit* m_edTotal;
  JDatabasePicker* m_supplierPicker;
  NoteTableWidget* m_table;
  QCheckBox* m_cbCash;
  JDatabase* m_database;
  QDockWidget* m_dock;
  QPlainTextEdit* m_teObservation;
  JExpLineEdit* m_edDisccount;

private slots:
  void setToday();
  void checkDate();
  void removeItem();
  void supplierChanged();
  void showSearch();
  void lastItemSelected();
  void itemSelected(const JItem& jItem);
  void itemsRemoved(const QVector<Id>& ids);
  void addProduct();

public slots:
  void create();
  void setNote(const Note& note);
  void updateControls();
  Note save(Person& employee);

signals:
  void changedSignal();
};

#endif // BKFRAME_H

#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include <QSqlDatabase>
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
class JLineEdit;

class NoteView : public QFrame
{
  Q_OBJECT

public:
  explicit NoteView(QWidget *parent = 0);
  ~NoteView();
  void setDatabase(QSqlDatabase db);
  Note getNote() const;
  void addNoteItem(const NoteItem& noteItem);

private:
  qlonglong m_currentId;
  qlonglong m_lastId;
  QSqlDatabase m_db;
  QPushButton* m_btnCreate;
  QPushButton* m_btnOpenLast;
  QPushButton* m_btnSearch;
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
  JLineEdit* m_edDisccount;

private slots:
  void setToday();
  void checkDate();
  void removeItem();
  void supplierChanged();
  void showSearch();
  void lastItemSelected();
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void searchProduct();
  void editPackage(const Package& package,
                   const QString& productUnity);

public slots:
  void create();
  void setNote(const Note& note);
  void updateControls();
  Note save();

signals:
  void changedSignal();
};

#endif // BKFRAME_H

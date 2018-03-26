#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include <QSqlDatabase>
#include "note.h"
#include "settings.h"

#define MAX_ITEMS 100

class QDateEdit;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QLabel;
class QCheckBox;
class NoteTableWidget;
class JDatabasePicker;
class QDockWidget;
class JDatabase;
class QIODevice;

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
  JDatabase* m_database;
  QDockWidget* m_dock;

  bool print(QIODevice* printer,
             InterfaceType type,
             const QString& userName,
             int id);

private slots:
  void setToday();
  void checkDate();
  void removeItem();
  void supplierChanged();
  void showSearch();
  bool save();
  void lastItemSelected();
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void searchProduct();

public slots:
  void create();
  void setNote(const Note& note);
  void updateControls();
  void saveAndPrint(QIODevice* printer,
                    InterfaceType type);

signals:
  void changedSignal();
};

#endif // BKFRAME_H

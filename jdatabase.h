#ifndef JDATABASE_H
#define JDATABASE_H

#include <QFrame>
#include <QDialog>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QVector>
#include <QTableView>
#include "product.h"
#include "person.h"
#include "reminder.h"

class QPushButton;
class QCheckBox;
class JTableView;
class JLineEdit;
class QSortFilterProxyModel;
class NoteFilterDlg;

class JTableView : public QTableView
{
  Q_OBJECT

public:
  explicit JTableView(QWidget *parent = 0);

signals:
  void enterKeyPressedSignal();

protected:
  void keyPressEvent(QKeyEvent* event);
};

class JDatabase : public QFrame
{
  Q_OBJECT

public:
  enum class Mode
  {
    Full,
    SingleSelector,
    MultiSelector,
    ReadOnly
  };

  explicit JDatabase(const QString& tableName,
                     Mode mode = Mode::Full,
                     QWidget *parent = 0);
  ~JDatabase();
  QString getTableName() const;
  JItem* getCurrentItem() const;
  QVector<Id> getSelectedIds() const;
  const QVector<JItem*> getCurrentItems() const;

  bool save(const JItem& jItem, Person* pEmployee = nullptr);

public slots:
  void refresh();
  void selectItems();
  void selectItem(Id id);
  void selectItems(const QVector<Id> ids);
  void setFixedFilter(const QString& fixedFilter);
  void clearFilterSearch();

private:
  const Mode m_mode;
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  QPushButton* m_btnFilter;
  QPushButton* m_btnFilterClear;
  JLineEdit* m_edFilterSearch;
  QCheckBox* m_cbContains;
  JTableView* m_table;
  QString m_tableName;
  QString m_filter;
  QString m_fixedFilter;
  QVector<JItem*> m_currentItems;
  QSortFilterProxyModel* m_proxyModel;
  NoteFilterDlg* m_noteFilter;

  void clearCurrentItems();

private slots:
  void filterSearchChanged();
  void filterSearchEnter();
  void containsPressed();
  void enableControls();
  void removeItems();
  void focusFilterSearch();
  void emitCurrentRowChangedSignal();
  void showFilter();
  void clearFilter();

signals:
  void itemSelectedSignal(const JItem& item);
  void itemsSelectedSignal(const QVector<JItem*>& items);
  void itemsRemovedSignal(const QVector<Id>& ids);
  void currentRowChangedSignal(int row);
};

class JDatabaseSelector : public QDialog
{
  Q_OBJECT

public:
  explicit JDatabaseSelector(const QString& tableName,
                             const QString& title,
                             const QIcon& icon,
                             bool bMultiSelector = false,
                             QWidget* parent = 0);

  JDatabase* getDatabase() const;

protected:
  void closeEvent(QCloseEvent * e);

private slots:
  void itemsSelected(const QVector<JItem*>& jItem);

private:
  JDatabase* m_database;
};

#endif // JDATABASE_H

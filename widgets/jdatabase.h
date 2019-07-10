#ifndef JDATABASE_H
#define JDATABASE_H

#include <QFrame>
#include <QDialog>
#include <QModelIndex>
#include <QVector>
#include <QTableView>
#include "items/jitemsql.h"

class QPushButton;
class QCheckBox;
class JTableView;
class JLineEdit;
class QSortFilterProxyModel;
class Employee;

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
  JItemSQL* getCurrentItem() const;
  QVector<Id> getSelectedIds() const;
  void selectIds(const QVector<Id>& ids);
  const QVector<JItemSQL*> getCurrentItems() const;
  int getNumberOfEntries() const;
  double getSum(int column) const;

  bool save(const JItemSQL& jItem);

public slots:
  void refresh();
  void selectItems();
  void selectItem(Id id);
  void selectItems(const QVector<Id> ids);
  void setFixedFilter(const QString& fixedFilter);
  void setDynamicFilter(const QString& dynamicFilter);
  void clearSearch();

private:
  const Mode m_mode;
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  JLineEdit* m_edSearch;
  QCheckBox* m_cbContains;
  JTableView* m_table;
  QString m_tableName;
  QString m_dynamicFilter;
  QString m_fixedFilter;
  QVector<JItemSQL*> m_currentItems;
  QSortFilterProxyModel* m_proxyModel;

  void clearCurrentItems();

private slots:
  void searchChanged();
  void searchEnter();
  void containsPressed();
  void enableControls();
  void removeItems();
  void focusSearch();
  void emitCurrentRowChangedSignal();

signals:
  void itemSelectedSignal(const JItemSQL& item);
  void itemsSelectedSignal(const QVector<JItemSQL*>& items);
  void itemsRemovedSignal(const QVector<Id>& ids);
  void currentRowChangedSignal(int row);
  void refreshSignal();
};

class JDatabaseSelector : public QDialog
{
  Q_OBJECT

public:
  explicit JDatabaseSelector(const QString& tableName,
                             bool bMultiSelector = false,
                             QWidget* parent = 0);

  JDatabase* getDatabase() const;

protected:
  void closeEvent(QCloseEvent * e);

private slots:
  void itemsSelected(const QVector<JItemSQL*>& v);

private:
  JDatabase* m_database;
};

#endif // JDATABASE_H

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
  explicit JDatabase(bool bSelectorMode = false,
                     QWidget *parent = 0);
  ~JDatabase();
  void setDatabase(QSqlDatabase db,
                   const QString& tableName);
  QSqlDatabase getDatabase() const;
  QString getTableName() const;
  JItem* getCurrentItem() const;

  bool save(const JItem& jItem);

public slots:
  void refresh(bool bSaveIdx = true);
  void selectItem();
  void selectItem(qlonglong id);
  void setCustomFilter(const QString& customFilter);
  void clearFilterSearch();

private:
  const bool m_bSelectorMode;
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  QPushButton* m_btnFilter;
  JLineEdit* m_edFilterSearch;
  QCheckBox* m_cbContains;
  JTableView* m_table;
  QString m_tableName;
  QString m_customFilter;
  JItem* m_currentItem;

private slots:
  void filterSearchChanged();
  void filterSearchEnter();
  void containsPressed();
  void enableControls();
  void removeItem();
  void focusFilterSearch();
  void sortChanged(int column, Qt::SortOrder sortOrder);
  void emitCurrentRowChangedSignal();

signals:
  void itemSelectedSignal(const JItem& jItem);
  void itemRemovedSignal(qlonglong id);
  void currentRowChangedSignal(int row);
};

class JDatabaseSelector : public QDialog
{
  Q_OBJECT

public:
  explicit JDatabaseSelector(const QString& title,
                             const QIcon& icon,
                             QWidget* parent = 0);

  void setDatabase(QSqlDatabase db,
                   const QString& tableName);

  JDatabase* getDatabase() const;

protected:
  void closeEvent(QCloseEvent * e);

private slots:
  void itemSelected(const JItem& jItem);

signals:
  void itemSelectedSignal(const JItem& jItem);

private:
  JDatabase* m_database;
};

#endif // JDATABASE_H

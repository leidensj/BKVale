#ifndef JDATABASE_H
#define JDATABASE_H

#include <QFrame>
#include <QDialog>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QVector>
#include <QTableView>
#include "jtablecolumn.h"
#include "product.h"
#include "person.h"
#include "reminder.h"

class QPushButton;
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
  bool save(const JItem& jItem);

public slots:
  void refresh(bool bSaveIdx = true);
  void selectItem(qlonglong id);
  void setUserFilter(const QString& userFilter);

private:
  const bool m_bSelectorMode;
  QSqlDatabase m_db;
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  QPushButton* m_btnFilter;
  JLineEdit* m_edFilterSearch;
  QPushButton* m_btnContains;
  JTableView* m_table;
  QString m_tableName;
  QString m_userFilter;
  QVector<JTableColumn> m_vColumns;

private slots:
  void selectItem();
  void filterSearchChanged();
  void filterSearchEnter();
  void containsPressed();
  void enableControls();
  void removeItem();
  void focusFilterSearch();

signals:
  void itemSelectedSignal(const JItem& jItem);
  void itemRemovedSignal(qlonglong id);
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

  Product getCurrentProduct() const;
  Person getCurrentPerson() const;
  Category getCurrentCategory() const;
  Image getCurrentImage() const;
  QString getTableName() const;

  void setUserFilter(const QString& userFilter);

private slots:
  void itemSelected(const JItem& jItem);

signals:
  void itemSelectedSignal(const JItem& jItem);

private:
  Product m_currentProduct;
  Person m_currentPerson;
  Category m_currentCategory;
  Image m_currentImage;
  JDatabase* m_database;
};

#endif // JDATABASE_H

#ifndef DATABASEVIEWER_H
#define DATABASEVIEWER_H

#include <QWidget>
#include <QModelIndex>
#include <QVector>
#include <QTableView>
#include "items/jitemsql.h"

class QPushButton;
class QCheckBox;
class JLineEdit;
class QSortFilterProxyModel;
class Employee;
class QHBoxLayout;

class JEnterSignalTable : public QTableView
{
  Q_OBJECT

public:
  explicit JEnterSignalTable(QWidget *parent = nullptr);

signals:
  void enterKeyPressedSignal();

protected:
  void keyPressEvent(QKeyEvent* event);
};

class DatabaseViewer : public QWidget
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

  explicit DatabaseViewer(const QString& tableName,
                          Mode mode = Mode::Full,
                          QWidget *parent = 0);
  ~DatabaseViewer();
  QString getTableName() const;
  JItemSQL* getCurrentItem() const;
  QVector<Id> getSelectedIds() const;
  void selectIds(const QVector<Id>& ids);
  const QVector<JItemSQL*>& getCurrentItems() const;
  int getNumberOfEntries() const;
  double getSum(int column) const;

  bool save(const JItemSQL& jItem);
  QPushButton* addButton(const QString& toolTip, const QIcon& icon, int shortcut = 0);

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
  QHBoxLayout* m_ltButton;
  JLineEdit* m_edSearch;
  QCheckBox* m_cbContains;
  JEnterSignalTable* m_table;
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

#endif // DATABASEVIEWER_H

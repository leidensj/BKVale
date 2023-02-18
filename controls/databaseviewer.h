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
    Edit,
    SingleSelector,
    MultiSelector,
    ReadOnly
  };

  explicit DatabaseViewer(const QString& tableName, Mode mode, QWidget *parent = nullptr);
  ~DatabaseViewer();
  QString getTableName() const;
  Ids getSelectedIds() const;
  Id getFirstSelectedId() const;
  int getRowCount() const;
  double getSum(int column) const;
  void selectId(const Id& id);
  void selectIds(const Ids& ids);
  QPushButton* addButton(const QString& toolTip, const QIcon& icon, int shortcut = 0);

  const Mode m_mode;
  const QString m_tableName;

public slots:
  void refresh();
  void setFixedFilter(const QString& fixedFilter);
  void setDynamicFilter(const QString& dynamicFilter);
  void clearSearch();
  void toCSV();

private:
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  QPushButton* m_btnCopy;
  QPushButton* m_btnCSV;
  QHBoxLayout* m_ltButton;
  JLineEdit* m_edSearch;
  QCheckBox* m_cbContains;
  JEnterSignalTable* m_table;
  QString m_dynamicFilter;
  QString m_fixedFilter;
  QSortFilterProxyModel* m_proxyModel;

private slots:
  void searchChanged();
  void searchEnter();
  void containsPressed();
  void enableControls();
  void removeItems();
  void focusSearch();
  void emitCurrentRowChangedSignal();
  void emitItemsSelectedSignal();
  void emitCopySignal();

signals:
  void itemsSelectedSignal();
  void itemsRemovedSignal(const Ids& ids);
  void currentRowChangedSignal(int row);
  void refreshSignal();
  void copySignal();
};

#endif // DATABASEVIEWER_H

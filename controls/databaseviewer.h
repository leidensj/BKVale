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
                          QWidget *parent = nullptr);
  ~DatabaseViewer();
  QString getTableName() const;
  void selectId(const Id& id);
  void selectIds(const QVector<Id>& ids);
  QVector<Id> selectedIds() const;
  Id firstSelectedId() const;
  int rowCount() const;
  double sum(int column) const;
  QPushButton* addButton(const QString& toolTip, const QIcon& icon, int shortcut = 0);

public slots:
  void refresh();
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

signals:
  void itemsSelectedSignal();
  void itemsRemovedSignal(const QVector<Id>& ids);
  void currentRowChangedSignal(int row);
  void refreshSignal();
};

#endif // DATABASEVIEWER_H

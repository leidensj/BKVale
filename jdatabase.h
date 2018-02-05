#ifndef JDATABASE_H
#define JDATABASE_H

#include <QFrame>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QSqlTableModel>
#include <QVector>
#include <QHeaderView>

class QPushButton;
class QTableView;
class JLineEdit;

#define ID_COLUMN 0

struct SqlTableColumn
{
  SqlTableColumn()
    : m_bHidden(false)
    , m_resizeMode(QHeaderView::ResizeMode::ResizeToContents)
  {

  }

  SqlTableColumn(bool bHidden,
                 bool bSort,
                 const QString& sqlName,
                 const QString& friendlyName,
                 QHeaderView::ResizeMode resizeMode)
  : m_bHidden(bHidden)
  , m_bSort(bSort)
  , m_sqlName(sqlName)
  , m_friendlyName(friendlyName)
  , m_resizeMode(resizeMode)
  {
    if (m_bSort && m_bHidden)
      m_bSort = false;
  }

  bool m_bHidden;
  bool m_bSort;
  QString m_sqlName;
  QString m_friendlyName;
  QHeaderView::ResizeMode m_resizeMode;
};

class JDatabase : public QFrame
{
  Q_OBJECT

public:
  explicit JDatabase(QWidget *parent = 0);
  ~JDatabase();
  void set(QSqlTableModel* model,
           const QString& tableName,
           const QVector<SqlTableColumn>& sqlTableColumns);
  QSqlDatabase get() const;

public slots:
  void refresh();
  void emitItemRemoveSignal();
  void enableControls();

private:
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  QPushButton* m_btnFilter;
  JLineEdit* m_edFilterSearch;
  QPushButton* m_btnContains;
  QTableView* m_table;
  QString m_tableName;
  QVector<SqlTableColumn> m_columns;

private slots:
  void itemSelected(const QModelIndex& idx);
  void itemSelected();
  void filterSearchChanged();
  void filterSearchEnter();
  void containsPressed();

signals:
  void itemSelectedSignal(int id);
  void itemRemoveSignal(int id);
};

#endif // JDATABASE_H

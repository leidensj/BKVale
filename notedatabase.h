#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QFrame>
#include <note.h>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QSqlTableModel>
#include <QVector>

class QPushButton;
class QTableView;
class QCheckBox;
class JLineEdit;

struct SqlTableColumn
{
  SqlTableColumn()
    : m_bHidden(false)
  {

  }

  SqlTableColumn(bool bHidden,
                 const QString& sqlName,
                 const QString& friendlyName)
  : m_bHidden(bHidden)
  , m_sqlName(sqlName)
  , m_friendlyName(friendlyName)
  {

  }

  bool m_bHidden;
  QString m_sqlName;
  QString m_friendlyName;
};

class NoteDatabase : public QFrame
{
  Q_OBJECT

public:
  explicit NoteDatabase(QWidget *parent = 0);
  ~NoteDatabase();
  void set(QSqlDatabase db,
           const QString& tableName,
           const QVector<SqlTableColumn>& sqlTableColumns);
  QSqlDatabase get() const;

public slots:
  void refresh();
  void emitNoteRemoveSignal();
  void enableControls();

private:
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  QPushButton* m_btnFilter;
  JLineEdit* m_edFilterSearch;
  QCheckBox* m_cbContains;
  QTableView* m_table;
  QString m_tableName;
  QVector<SqlTableColumn> m_columns;

private slots:
  void noteSelected(const QModelIndex& idx);
  void noteSelected();
  void filterSearchChanged();
  void filterSearchEnter();
  void containsPressed();

signals:
  void noteSelectedSignal(int id);
  void noteRemoveSignal(int id);
};

#endif // HISTORYWIDGET_H

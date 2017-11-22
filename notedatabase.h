#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QFrame>
#include <note.h>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QSqlTableModel>

class QTableWidgetItem;
class QPushButton;
class QTableView;

class NoteDatabase : public QFrame
{
  Q_OBJECT

public:
  explicit NoteDatabase(QWidget *parent = 0);
  ~NoteDatabase();
  void set(QSqlDatabase db);
  QSqlDatabase get() const;

public slots:
  void refresh();
  void emitNoteRemoveSignal();
  void enableControls();

private:
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  QTableView* m_table;

private slots:
  void noteSelected(const QModelIndex& idx);
  void noteSelected();

signals:
  void noteSelectedSignal(int id);
  void noteRemoveSignal(int id);
};

#endif // HISTORYWIDGET_H

#ifndef USERMGTDATABASE_H
#define USERMGTDATABASE_H

#include <QFrame>
#include <note.h>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QSqlTableModel>

class QPushButton;
class QTableView;

class UserMgtDatabase : public QFrame
{
  Q_OBJECT

public:
  explicit UserMgtDatabase(QWidget *parent = 0);
  ~UserMgtDatabase();
  void set(QSqlDatabase db);
  QSqlDatabase get() const;

public slots:
  void refresh();
  void emitUserRemoveSignal();
  void enableControls();

private:
  QPushButton* m_btnOpen;
  QPushButton* m_btnRefresh;
  QPushButton* m_btnRemove;
  QTableView* m_table;

private slots:
  void userSelected(const QModelIndex& idx);
  void userSelected();

signals:
  void userSelectedSignal(int id);
  void userRemoveSignal(int id);
};

#endif // USERMGTDATABASE_H

#ifndef CONSUMPTIONDATABASE_H
#define CONSUMPTIONDATABASE_H

#include <QFrame>
#include <QSqlDatabase>
#include "consumptionview.h"

class QPushButton;
class QTableView;

class ConsumptionDatabase : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionDatabase(QWidget *parent = 0);
  ~ConsumptionDatabase();
  void setDatabase(QSqlDatabase db);

private slots:

private:
  QPushButton* m_refresh;
  QPushButton* m_remove;
  QTableView* m_table;
  QSqlDatabase m_db;
};

#endif // CONSUMPTIONDATABASE_H

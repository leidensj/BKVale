#ifndef STOREEMPLOYEETABLEWIDGET_H
#define STOREEMPLOYEETABLEWIDGET_H

#include "jitemtable.h"
#include "store.h"

class StoreEmployeeTableWidget : public JTable
{
  Q_OBJECT

  enum class Column
  {
    Name,
    WorkingHours
  };

public:
  explicit StoreEmployeeTableWidget(QWidget* parent = nullptr);
  const JItem& getItem(int row) const;

public slots:
  void addItem(const JItem& o);
  void addItem();

private:
  mutable StoreEmployee m_ref;

protected slots:
  void update(int row, int column);
  void itemActivate(int row, int column);
  void itemDelete(int row, int column);
};

#endif // STOREEMPLOYEETABLEWIDGET_H

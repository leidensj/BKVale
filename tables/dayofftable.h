#ifndef DAYOFFTABLE_H
#define DAYOFFTABLE_H

#include "jtable.h"
#include "items/store.h"

class DayOffTable : public JTable
{
  Q_OBJECT

public:
  explicit DayOffTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void getDayOff(QVector<QVector<bool>>& v) const;
  void setStore(const Store& o, const QDate& date);
  void update();

public slots:
  void addRow();

private:
  QDate m_date;
  Store m_store;
};

#endif // PRODUCTCODETABLE_H

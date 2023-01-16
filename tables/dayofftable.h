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
  void setDayOff(const QVector<QVector<bool>>& v);
  void setStore(const Store& o, const QDate& date);
  void update();
  QString toCSV() const;
  QString message() const;

public slots:
  void addRow();
  void autoComplete();

private:
  QDate m_date;
  Store m_store;
};

#endif // PRODUCTCODETABLE_H

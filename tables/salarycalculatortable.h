#ifndef SALARYCALCULATORTABLE_H
#define SALARYCALCULATORTABLE_H

#include "jtable.h"
#include <QDate>
#include <QTime>

struct SalaryCalculatorResult
{
  qlonglong eid;
  QString ename;
  qlonglong sid;
  QString sname;
  QDate dtBegin;
  QTime tmBegin;
  QDate dtEnd;
  QTime tmEnd;
  QString value;
  void clear ()
  {
    eid = 0;
    ename.clear();
    sid = 0;
    sname.clear();
    dtBegin = QDate::currentDate();
    tmBegin = QTime::currentTime();
    dtEnd = QDate::currentDate();
    tmEnd = QTime::currentTime();
    value.clear();
  }
};

class SalaryCalculatorTable : public JTable
{
  Q_OBJECT

public:

  enum class Column
  {
    Name,
    Formula,
    DateBegin,
    TimeBegin,
    DateEnd,
    TimeEnd,
    Value
  };

  explicit SalaryCalculatorTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<SalaryCalculatorResult>& v) const;
  void get(int row, SalaryCalculatorResult& o) const;
  void set(const QVector<SalaryCalculatorResult>& v, bool bClear = true);
  void set(int row, const SalaryCalculatorResult& o);

public slots:
  void addRow();
  void addRowAndActivate();
};

#endif

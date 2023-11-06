#ifndef TIMECARDCONTROLTABLE_H
#define TIMECARDCONTROLTABLE_H

#include "timeinterval.h"
#include "jtable.h"

class TimeCardControlTable : public JTable
{
  Q_OBJECT

  enum class Column
  {
    Begin1,
    End1,
    Begin2,
    End2,
    Begin3,
    End3,
    Begin4,
    End4
  };

public:
  explicit TimeCardControlTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void getHours(QVector<TimeInterval>& v) const;
  void setHours(const QVector<TimeInterval>& v);

public slots:
  void addRow();
};

#endif // TIMECARDCONTROLTABLE_H

#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#define TIME_INTERVAL_EVERY_DAY 0

#include <QTime>

struct TimeInterval
{
  int m_day;
  QTime m_tmBegin;
  QTime m_tmEnd;

  void clear()
  {
    m_day = TIME_INTERVAL_EVERY_DAY;
  }

  TimeInterval()
  {
    clear();
  }
};

#endif // TIMEINTERVAL_H

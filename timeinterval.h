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

  bool operator != (const TimeInterval& other) const
  {
    return m_day != other.m_day || m_tmBegin != other.m_tmBegin || m_tmEnd != other.m_tmEnd;
  }

  bool operator == (const TimeInterval& other) const
  {
    return !(*this != other);
  }
};

#endif // TIMEINTERVAL_H

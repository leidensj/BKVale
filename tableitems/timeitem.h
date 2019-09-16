#ifndef TIMEITEM_H
#define TIMEITEM_H

#include "jtableitem.h"

class TimeItem : public JTableItem
{
public:
  TimeItem(const QTime& defaultTime);
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

private:
  QTime m_defaultTime;
};

#endif // TIMEITEM_H

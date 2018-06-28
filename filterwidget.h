#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QString>

class FilterWidget
{
public:

  FilterWidget()
  {

  }

  virtual QString getFilter() const = 0;
  virtual void clearFilter() = 0;
};

#endif // FILTERWIDGET_H

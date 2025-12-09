#ifndef CASHCLOSINGFILTER_H
#define CASHCLOSINGFILTER_H

#include "jfilter.h"

class QCheckBox;
class JDateTimeInterval;
class DatabasePicker;

class CashClosingFilter : public JFilter
{
  Q_OBJECT

public:
  explicit CashClosingFilter(QWidget* parent = nullptr);
  virtual QString getFilter() const;

public slots:
  virtual void clear();

private:
  DatabasePicker* m_cash;
  JDateTimeInterval* m_dt;
};

#endif // CASHCLOSINGFILTER_H

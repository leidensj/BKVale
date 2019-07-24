#ifndef PHONEMODEL_H
#define PHONEMODEL_H

#include "jmodel.h"

class PhoneModel : public JModel
{
public:
  PhoneModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
};

#endif // PHONEMODEL_H

#ifndef ACTIVEUSERMODEL_H
#define ACTIVEUSERMODEL_H

#include "jmodel.h"

class ActiveUserModel : public JModel
{
public:
  ActiveUserModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
};

#endif // ACTIVEUSERMODEL_H

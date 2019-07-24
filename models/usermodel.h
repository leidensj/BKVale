#ifndef USERMODEL_H
#define USERMODEL_H

#include "jmodel.h"

class UserModel : public JModel
{
public:
  UserModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
};

#endif // USERMODEL_H

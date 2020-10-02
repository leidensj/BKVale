#ifndef LOGINMODEL_H
#define LOGINMODEL_H

#include "jmodel.h"

class LoginModel : public JModel
{
public:
  LoginModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
};

#endif // LOGINMODEL_H

#ifndef SECTORMODEL_H
#define SECTORMODEL_H

#include "jmodel.h"

class SectorModel : public JModel
{
public:
  SectorModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
};

#endif // SECTORMODEL_H

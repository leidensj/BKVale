#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include "jmodel.h"

class ImageModel : public JModel
{
public:
  ImageModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
  QVariant data(const QModelIndex &idx, int role) const;
};

#endif // IMAGEMODEL_H

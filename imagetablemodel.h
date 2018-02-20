#ifndef IMAGETABLEMODEL_H
#define IMAGETABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>

class ImageTableModel : public QSqlTableModel
{

public:
  ImageTableModel(QObject *parent, QSqlDatabase db);
  QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const;
};

#endif // IMAGETABLEMODEL_H

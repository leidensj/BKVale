#ifndef JTABLEMODEL_H
#define JTABLEMODEL_H

#include <QSqlQueryModel>
#include <QHeaderView>
#include <QString>
#include "defines.h"

class JModel : public QSqlQueryModel
{
public:
  JModel(QObject *parent);
  virtual QString getStrQuery() = 0;
  virtual void select(const QString& query = "");
  virtual void select(QHeaderView* /*header*/) = 0;
  virtual void selectFilter(const QString& filter);
};

#endif // JTABLEMODEL_H

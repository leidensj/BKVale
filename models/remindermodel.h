#ifndef REMINDERMODEL_H
#define REMINDERMODEL_H

#include "jmodel.h"

class ReminderModel : public JModel
{
  enum class Column
  {
    Id,
    Title,
    Message,
    Subject,
    Date,
    Time,
    Favorite,
    HasDate,
    HasTime,
    Capital
  };

public:
  ReminderModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
  QVariant data(const QModelIndex &idx, int role) const;
};
#endif // REMINDERMODEL_H

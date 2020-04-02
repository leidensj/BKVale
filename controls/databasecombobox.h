#ifndef DATABASECOMBOBOX_H
#define DATABASECOMBOBOX_H

#include <QComboBox>
#include "items/jitem.h"

class DatabaseComboBox : public QComboBox
{
  Q_OBJECT

public:
  explicit DatabaseComboBox(const QString& tableName, int modelColumn = 0, QWidget* parent = nullptr);
  Id getCurrentId() const;
  void setCurrentId(const Id& id);

public slots:
  void refresh();
};

#endif // DATABASECOMBOBOX_H

#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QStringList>

#define NUMBER_OF_COLUMNS 5
enum class Column : int
{
  Ammount,
  Unity,
  Description,
  UnitValue,
  SubTotal
};

class SerializedTable
{
  QStringList m_list;
public:
  Table(const QString& serializedTable, QChar separator);
  QString at(int row, Column column);
};

#endif // COMMON_H

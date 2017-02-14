#ifndef CONS_H
#define CONS_H

#include <vector>
#include <QString>

enum class Column : int
{
  Ammount,
  Unity,
  Description,
  UnitValue,
  SubTotal
};

typedef std::vector<std::vector<QString>> TableContent;

#endif // CONS_H

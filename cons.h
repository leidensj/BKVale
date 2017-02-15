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

struct PromissoryNote
{
  int number;
  QString supplier;
  TableContent tableContent;
  QString total;
};

#endif // CONS_H

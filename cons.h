#ifndef CONS_H
#define CONS_H

#include <vector>
#include <QString>

#define NUMBER_OF_COLUMNS 5
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
  int m_number;
  qint64 m_date;
  QString m_supplier;
  TableContent m_tableContent;
  QString m_total;

  static const QChar st_separator;

  PromissoryNote();
  void clear();
  QString serializeItems() const;
  void deserializeItems(const QString& str);
};

#endif // CONS_H

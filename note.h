#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QStringList>
#include <vector>

#define NUMBER_OF_COLUMNS 5
enum class Column : int
{
  Ammount,
  Unity,
  Description,
  UnitValue,
  SubTotal
};

struct Note
{
  int m_id;
  int m_number;
  qint64 m_date;
  QString m_supplier;
  QString m_items;
  double m_total;

  Note();
  void clear();

  static QString format(const QString& str, bool b3places = false);
  static QString format(double d, bool b3places = false);
};

struct NoteItems
{
  const QStringList m_items;
  const int m_size;
  NoteItems(const QString& items);
  QString at(int row, Column column) const;
};

#endif // COMMON_H

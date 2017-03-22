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

  Note(int id,
       int number,
       qint64 date,
       QString supplier,
       QString items,
       double total);

  void clear();

  static QString format(const QString& str, bool b3places = false);
  static QString format(double d, bool b3places = false);
};

typedef std::vector<Note> Notes;

struct NoteItems
{
  const QStringList m_items;
  int m_size;
  NoteItems(const QString& items);
  QString at(int row, Column column) const;
};

#endif // COMMON_H

#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QStringList>
#include <QVector>

#define NUMBER_OF_COLUMNS 5
#define INVALID_NOTE_ID -1

enum class NoteColumn : int
{
  Ammount,
  Unity,
  Description,
  UnitValue,
  SubTotal
};

enum class NoteTableIndex : int
{
  ID = 0,
  Number,
  Date,
  Supplier,
  Items,
  Total
};

struct NoteItem
{
  int m_id;
  int m_noteId;
  double m_ammount;
  double m_price;
  QString m_unity;
  QString m_description;

  NoteItem();
  void clear();
  double subtotal() const;
  QString strSubtotal() const;
  QString strAmmount() const;
  QString strPrice() const;
};

struct Note
{
  int m_id;
  int m_number;
  qint64 m_date;
  QString m_supplier;
  QVector<NoteItem> m_items;
  double m_total;

  Note();

  Note(int id,
       int number,
       qint64 date,
       QString supplier,
       double total);

  void clear();

  static bool isValidID(int id) { return id != INVALID_NOTE_ID; }
};

#endif // COMMON_H

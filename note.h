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
  Price,
  SubTotal
};

enum class NoteTableIndex : int
{
  ID = 0,
  Number,
  Date,
  Supplier,
  Total
};

struct NoteItem
{
  mutable int m_id;
  double m_ammount;
  double m_price;
  QString m_unity;
  QString m_description;

  NoteItem();
  void clear();
  double subtotal() const { return m_ammount * m_price; }
  QString strSubtotal() const { return QString::number(subtotal(), 'f', 2); }
  QString strAmmount() const { return QString::number(m_ammount, 'f', 3); }
  QString strPrice() const { return QString::number(m_price, 'f', 2); }
};

struct Note
{
  mutable int m_id;
  qint64 m_date;
  QString m_supplier;
  QVector<NoteItem> m_items;
  double m_total;
  bool m_bCash;

  Note();

  void clear();
  QString strDate() const;
  QString strDayOfWeek() const;
  QString strId() const { return QString::number(m_id); }
  QString strTotal() const { return QString::number(m_total, 'f', 2); }
  static QString strNumber(int number) { return QString::number(number); }
  static bool isValidID(int id) { return id != INVALID_NOTE_ID; }
};

#endif // COMMON_H

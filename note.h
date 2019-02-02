#ifndef NOTE_H
#define NOTE_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDate>
#include "person.h"
#include "product.h"
#include "defines.h"
#include "jitem.h"

enum class NoteColumn : int
{
  Ammount,
  Unity,
  Description,
  Price,
  SubTotal
};

struct NoteItem : JItem
{
  NoteItem();
  void clear();
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  QString strTableName() const;

  double subtotal() const;
  QString strSubtotal() const;
  QString strAmmount() const;
  QString strPrice() const;

  Product m_product;
  double m_ammount;
  double m_price;
  Package m_package;
};

struct Note : public JItem
{
  Note();
  void clear();
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  QString strTableName() const;

  QString strDate() const;
  QString strDayOfWeek() const;
  QString strNumber() const;
  QString strDisccount() const;
  double total() const;
  double subTotal() const;
  QString strTotal() const;
  QString strSubTotal() const;

  mutable qlonglong m_number;
  QDate m_date;
  Person m_supplier;
  bool m_bCash;
  QString m_observation;
  QVector<NoteItem> m_vNoteItem;
  double m_disccount;
};

#endif // COMMON_H

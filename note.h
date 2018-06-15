#ifndef NOTE_H
#define NOTE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QDate>
#include <QMetaType>
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
  Product m_product;
  double m_ammount;
  double m_price;
  Package m_package;

  void clear()
  {
    m_id = INVALID_ID;
    m_product.clear();
    m_ammount = 0.0;
    m_price = 0.0;
    m_package.clear();
  }

  NoteItem()
  {
    clear();
  }

  double subtotal() const { return m_ammount * m_price; }
  QString strSubtotal() const { return st_strMoney(subtotal()); }
  QString strAmmount() const { return st_strAmmount(m_ammount); }
  QString strPrice() const { return st_strMoney(m_price); }
  bool isValid() const
  {
    return m_product.isValidId() &&
        m_ammount != 0.0 &&
        m_price != 0.0;
  }

  bool operator !=(const JItem& other) const
  {
    const NoteItem& another = dynamic_cast<const NoteItem&>(other);
    return
        m_product.m_id != another.m_product.m_id ||
        m_ammount != another.m_ammount ||
        m_price != another.m_price ||
        m_package != another.m_package;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }
};

Q_DECLARE_METATYPE(NoteItem)

struct Note : public JItem
{
  mutable qlonglong m_number;
  QString m_date;
  Person m_supplier;
  bool m_bCash;
  QString m_observation;
  QVector<NoteItem> m_vNoteItem;
  double m_disccount;

  void clear()
  {
    m_id = INVALID_ID;
    m_number = 0;
    m_supplier.clear();
    m_date = QDate::currentDate().toString(Qt::ISODate);
    m_bCash = false;
    m_vNoteItem.clear();
    m_observation.clear();
    m_disccount = 0.0;
  }

  Note()
  {
    clear();
  }

  QString strDate() const { return QDate::fromString(m_date, Qt::ISODate).toString("dd/MM/yyyy"); }
  QString strDayOfWeek() const { return QDate::fromString(m_date, Qt::ISODate).toString("dddd"); }
  QString strId() const { return QString::number(m_id); }
  QString strNumber() const { return st_strInt(m_number); }
  QString strDisccount() const { return st_strMoney(m_disccount); }

  double total() const
  {
    double total = 0.0;
    for (int i = 0; i != m_vNoteItem.size(); ++i)
      total += m_vNoteItem.at(i).subtotal();
    return total + m_disccount;
  }

  double subTotal() const
  {
    double subTotal = 0.0;
    for (int i = 0; i != m_vNoteItem.size(); ++i)
      subTotal += m_vNoteItem.at(i).subtotal();
    return subTotal;
  }

  QString strTotal() const { return st_strMoney(total()); }
  QString strSubTotal() const { return st_strMoney(subTotal()); }

  bool isValid() const
  {
    return true;
  }

  bool operator !=(const JItem& other) const
  {
    const Note& another = dynamic_cast<const Note&>(other);
    return
        m_number != another.m_number ||
        m_date != another.m_date ||
        m_supplier.m_id != another.m_supplier.m_id ||
        total() != another.total() ||
        m_bCash != another.m_bCash ||
        m_vNoteItem != another.m_vNoteItem ||
        m_disccount != another.m_disccount;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }
};

#endif // COMMON_H

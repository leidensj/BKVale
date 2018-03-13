#ifndef NOTE_H
#define NOTE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QDate>
#include <QMetaType>
#include "jtablecolumn.h"
#include "person.h"
#include "product.h"
#include "defines.h"

enum class NoteColumn : int
{
  Ammount,
  Unity,
  Description,
  Price,
  SubTotal
};

struct NoteItem
{
  mutable qlonglong m_id;
  qlonglong m_productId;
  double m_ammount;
  double m_price;
  bool m_bIsPackageAmmount;

  void clear()
  {
    m_id = INVALID_ID;
    m_productId = INVALID_ID;
    m_ammount = 0.0;
    m_price = 0.0;
    m_bIsPackageAmmount = false;
  }

  NoteItem()
  {
    clear();
  }

  double subtotal() const { return m_ammount * m_price; }
  static QString st_strSubTotal(double subtotal) { return QString::number(subtotal, 'f', 2); }
  static QString st_strAmmount(double ammount) { return QString::number(ammount, 'f', 3); }
  static QString st_strPrice(double price) { return QString::number(price, 'f', 2); }
  QString strSubtotal() const { return st_strSubTotal(subtotal()); }
  QString strAmmount() const { return st_strAmmount(m_ammount); }
  QString strPrice() const { return st_strPrice(m_price); }
  static bool st_isValidID(qlonglong id) { return id != INVALID_ID; }
  bool isValidID() const { return st_isValidID(m_id); }
};

struct Note
{
  mutable qlonglong m_id;
  QString m_date;
  qlonglong m_supplierId;
  double m_total;
  bool m_bCash;

  void clear()
  {
    m_id = INVALID_ID;
    m_supplierId = INVALID_ID;
    m_date.clear();
    m_total = 0.0;
    m_bCash = false;
  }

  Note()
  {
    clear();
  }

  QString strDate() const { return QDate::fromString(m_date, Qt::ISODate).toString("dd/MM/yyyy"); }
  QString strDayOfWeek() const { return QDate::fromString(m_date, Qt::ISODate).toString("dddd"); }
  QString strId() const { return QString::number(m_id); }
  QString strTotal() const { return QString::number(m_total, 'f', 2); }
  static QString st_strNumber(qlonglong number) { return QString::number(number); }
  static bool st_isValidID(qlonglong id) { return id != INVALID_ID; }
  bool isValidID() const { return st_isValidID(m_id); }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(NOTE_SQL_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(NOTE_SQL_COL01, QObject::tr("Número"), false, true));
    c.push_back(JTableColumn(NOTE_SQL_COL02, QObject::tr("Data"), false));
    c.push_back(JTableColumn(NOTE_SQL_COL03, QObject::tr("Fornecedor"), false, false, JResizeMode::Stretch));
    c.push_back(JTableColumn(NOTE_SQL_COL04, QObject::tr("Total"), false));
    c.push_back(JTableColumn(NOTE_SQL_COL05, QObject::tr("À Vista")));
    return c;
  }
};

struct FullNoteItem
{
  NoteItem m_item;
  FullProduct m_fProduct;

  void clear()
  {
    m_item.clear();
    m_fProduct.clear();
  }

  FullNoteItem()
  {
    clear();
  }
};

Q_DECLARE_METATYPE(FullNoteItem)

struct FullNote
{
  Note m_note;
  qlonglong m_number;
  FullPerson m_fSupplier;
  QVector<FullNoteItem> m_vfNoteItem;

  void clear()
  {
    m_note.clear();
    m_number = NOTE_SQL_DEFAULT_NUMBER;
    m_fSupplier.clear();
    m_vfNoteItem.clear();
  }

  FullNote()
  {
    clear();
  }
};

#endif // COMMON_H

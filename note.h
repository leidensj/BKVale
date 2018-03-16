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
  bool m_bIsPackageAmmount;

  void clear()
  {
    m_id = INVALID_ID;
    m_product.clear();
    m_ammount = 0.0;
    m_price = 0.0;
    m_bIsPackageAmmount = true;
  }

  NoteItem()
  {
    clear();
  }

  QString strUnity() const { return m_bIsPackageAmmount
                                    ? m_product.m_packageUnity
                                    : m_product.m_unity; }
  double subtotal() const { return m_ammount * m_price; }
  static QString st_strSubTotal(double subtotal) { return QString::number(subtotal, 'f', 2); }
  static QString st_strAmmount(double ammount) { return QString::number(ammount, 'f', 3); }
  static QString st_strPrice(double price) { return QString::number(price, 'f', 2); }
  QString strSubtotal() const { return st_strSubTotal(subtotal()); }
  QString strAmmount() const { return st_strAmmount(m_ammount); }
  QString strPrice() const { return st_strPrice(m_price); }
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
        m_bIsPackageAmmount != another.m_bIsPackageAmmount;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }
};

Q_DECLARE_METATYPE(NoteItem)

struct Note : public JItem
{
  qlonglong m_number;
  QString m_date;
  Person m_supplier;
  double m_total;
  bool m_bCash;
  QVector<NoteItem> m_vNoteItem;

  void clear()
  {
    m_id = INVALID_ID;
    m_number = NOTE_SQL_DEFAULT_NUMBER;
    m_supplier.clear();
    m_date.clear();
    m_total = 0.0;
    m_bCash = false;
    m_vNoteItem.clear();
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
  QString strNumber() const { return st_strNumber(m_number); }

  bool isValid() const
  {
    bool b = m_supplier.isValidId() &&
             m_total != 0.0 &&
             !m_vNoteItem.isEmpty();
    for (int i = 0; i != m_vNoteItem.size(); ++i)
      b &= m_vNoteItem.at(i).isValid();
    return b;
  }

  bool operator !=(const JItem& other) const
  {
    const Note& another = dynamic_cast<const Note&>(other);
    return
        m_number != another.m_number ||
        m_date != another.m_date ||
        m_supplier.m_id != another.m_supplier.m_id ||
        m_total != another.m_total ||
        m_bCash != another.m_bCash ||
        m_vNoteItem != another.m_vNoteItem;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }

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

#endif // COMMON_H

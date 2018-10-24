#ifndef REMINDER_H
#define REMINDER_H

#include <QString>
#include <QObject>
#include "jitem.h"

struct Reminder : public JItem
{
  enum class Capitalization : int
  {
    Normal,
    AllUppercase,
    AllLowercase
  };

  enum class Size : int
  {
    Normal,
    Large
  };

  QString m_title;
  QString m_message;
  bool m_bFavorite;
  Size m_size;
  Capitalization m_capitalization;
  bool m_bBarcodeHRI;
  QString m_barcode;

  void clear()
  {
    m_id.clear();
    m_title.clear();
    m_message.clear();
    m_bFavorite = false;
    m_size = Size::Large;
    m_capitalization = Capitalization::AllUppercase;
    m_bBarcodeHRI = true;
    m_barcode.clear();
  }

  bool operator != (const JItem& other) const
  {
    const Reminder& another = dynamic_cast<const Reminder&>(other);
    return
        m_title != another.m_title ||
        m_message != another.m_message ||
        m_bFavorite != another.m_bFavorite ||
        m_size != another.m_size ||
        m_capitalization != another.m_capitalization ||
        m_bBarcodeHRI != another.m_bBarcodeHRI ||
        m_barcode != another.m_barcode;
  }

  bool operator == (const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    return !m_title.isEmpty() || !m_message.isEmpty() || !m_barcode.isEmpty();
  }

  Reminder()
  {
    clear();
  }
};

#endif // REMINDER_H

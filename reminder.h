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

  Reminder();
  void clear();
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString strTableName() const;

  QString m_title;
  QString m_message;
  bool m_bFavorite;
  Size m_size;
  Capitalization m_capitalization;
  bool m_bBarcodeHRI;
  QString m_barcode;
};

#endif // REMINDER_H

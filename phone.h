#ifndef PHONE_H
#define PHONE_H

#include <QObject>
#include <QString>
#include "defines.h"
#include "jitem.h"

struct Phone : public SQL_JItem
{
  Phone();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString strTableName() const;

  QString strFormattedPhone() const;
  QString strFormattedPhoneWithName() const;

  int m_countryCode;
  int m_code;
  QString m_number;
  QString m_name;
};

#endif // PHONENUMBER_H

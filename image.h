#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVector>
#include "defines.h"
#include "jitem.h"

struct Image : public SQL_JItem
{
  Image();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString strTableName() const;

  QString m_name;
  QByteArray m_image;
};

#endif // IMAGE_H

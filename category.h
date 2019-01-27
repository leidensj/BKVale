#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QString>
#include <QVector>
#include "image.h"
#include "defines.h"
#include "jitem.h"

struct Category : public JItem
{
  Category();
  void clear();
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString strTableName() const;

  QString m_name;
  Image m_image;
};

#endif // CATEGORY_H

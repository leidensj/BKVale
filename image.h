#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVector>
#include "defines.h"
#include "jitem.h"

struct Image : public JItem
{
  QString m_name;
  QByteArray m_image;

  Image()
  {
    clear();
  }

  void clear()
  {
    m_id = INVALID_ID;
    m_name.clear();
    m_image.clear();
  }

  bool operator !=(const JItem& other) const
  {
    const Image& another = dynamic_cast<const Image&>(other);
    return
        m_name != another.m_name ||
        m_image != another.m_image;
  }

  bool operator ==(const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const { return !m_name.isEmpty() && !m_image.isEmpty(); }
};

#endif // IMAGE_H

#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QByteArray>

#define MAX_IMAGE_NAME_LENGTH 35
#define MAX_IMAGE_SIZE        1048576 //1MB

struct Image
{
  mutable int m_id;
  QString m_name;
  QByteArray m_image;
};

#endif // IMAGE_H

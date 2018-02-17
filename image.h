#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QByteArray>

#define INVALID_IMAGE_ID      -1
#define MAX_IMAGE_NAME_LENGTH 35
#define MAX_IMAGE_SIZE        1048576 //1MB

struct Image
{
  mutable int m_id;
  QString m_name;
  QByteArray m_image;

  Image()
  {
    clear();
  }

  void clear()
  {
    m_id = INVALID_IMAGE_ID;
    m_name.clear();
    m_image.clear();
  }

  bool operator !=(const Image& img)
  {
    return
        m_name != img.m_name ||
        m_image != img.m_image;
  }

  static bool st_isValidId(int id) { return id != INVALID_IMAGE_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
};

#endif // IMAGE_H

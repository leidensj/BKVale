#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVector>
#include "jtablecolumn.h"

#define SQL_IMAGE_TABLE_NAME "_IMAGES"
#define SQL_IMAGE_COL00      "_ID"
#define SQL_IMAGE_COL01      "_NAME"
#define SQL_IMAGE_COL02      "_IMAGE"

#define INVALID_IMAGE_ID      -1
#define MAX_IMAGE_NAME_LENGTH 35
#define MAX_IMAGE_SIZE        1048576 //1MB

struct Image
{
  mutable qlonglong m_id;
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

  static bool st_isValidId(qlonglong id) { return id != INVALID_IMAGE_ID; }
  bool isValidId() const { return st_isValidId(m_id); }

  static bool st_isValid(const Image& img) { return !img.m_name.isEmpty() && !img.m_image.isEmpty(); }
  bool isValid() const { return st_isValid(*this); }

  enum class Column
  {
    Id,
    Name,
    Image
  };

  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(SQL_IMAGE_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(SQL_IMAGE_COL01, QObject::tr("Imagem"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn(SQL_IMAGE_COL02, QObject::tr("Imagem")));
    return c;
  }
};

#endif // IMAGE_H

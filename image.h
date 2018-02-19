#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVector>
#include "jtablecolumn.h"

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

  static bool st_isValid(const Image& img) { return !img.m_name.isEmpty() && !img.m_image.isEmpty(); }
  bool isValid() const { return st_isValid(*this); }
  static QString getTableName() { return "_IMAGES"; }
  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn("_ID", QObject::tr("Id")));
    c.push_back(JTableColumn("_NAME", QObject::tr("Imagem"), false, true, JResizeMode::Stretch));
    c.push_back(JTableColumn("_IMAGE", QObject::tr("Imagem")));
    return c;
  }
};

#endif // IMAGE_H

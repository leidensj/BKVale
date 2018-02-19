#ifndef JTABLECOLUMN_H
#define JTABLECOLUMN_H

#include <QString>

#define ID_COLUMN 0

enum class JResizeMode : int
{
  Interactive = 0,
  Stretch,
  Fixed,
  ResizeToContents
};

struct JTableColumn
{
  JTableColumn()
    : m_bHidden(false)
    , m_bSort(false)
    , m_resizeMode(JResizeMode::Interactive)
  {

  }

  JTableColumn(const QString& sqlName,
               const QString& friendlyName,
               bool bHidden = true,
               bool bSort = false,
               JResizeMode resizeMode = JResizeMode::ResizeToContents)
  : m_sqlName(sqlName)
  , m_friendlyName(friendlyName)
  , m_bHidden(bHidden)
  , m_bSort(bSort)
  , m_resizeMode(resizeMode)
  {
    if (m_bSort && m_bHidden)
      m_bSort = false;
  }

  QString m_sqlName;
  QString m_friendlyName;
  bool m_bHidden;
  bool m_bSort;
  JResizeMode m_resizeMode;
};

#endif // JTABLECOLUMN_H

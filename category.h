#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>

#define INVALID_CATEGORY_ID   -1
#define INVALID_CATEGORY_ICON -1

#define MAX_CATEGORY_NAME_LENGTH 35

struct Category
{
  mutable int m_id;
  QString m_name;
  int m_icon;

  Category()
  {
    clear();
  }

  void clear()
  {
    m_id = INVALID_CATEGORY_ID;
    m_name.clear();
    m_icon = INVALID_CATEGORY_ICON;
  }

  static bool st_isValidId(int id) { return id != INVALID_CATEGORY_ID; }
  bool isValidId() const { return st_isValidId(m_id); }
};

#endif // CATEGORY_H

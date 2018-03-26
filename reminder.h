#ifndef REMINDER_H
#define REMINDER_H

#include <QString>
#include <QObject>
#include "jitem.h"

struct Reminder : public JItem
{
  enum class Capitalization : int
  {
    Normal,
    AllUppercase,
    AllLowercase
  };

  enum class Size : int
  {
    Normal,
    Large
  };

  QString m_title;
  QString m_message;
  bool m_bFavorite;
  Size m_size;
  Capitalization m_capitalization;

  void clear()
  {
    m_id = INVALID_ID;
    m_title.clear();
    m_message.clear();
    m_bFavorite = false;
    m_size = Size::Normal;
    m_capitalization = Capitalization::Normal;
  }

  bool operator != (const JItem& other) const
  {
    const Reminder& another = dynamic_cast<const Reminder&>(other);
    return
        m_title != another.m_title ||
        m_message != another.m_message ||
        m_bFavorite != another.m_bFavorite ||
        m_size != another.m_size ||
        m_capitalization != another.m_capitalization;
  }

  bool operator == (const JItem& other) const
  {
    return !(*this != other);
  }

  bool isValid() const
  {
    return !m_title.isEmpty() || !m_message.isEmpty();
  }

  Reminder()
  {
    clear();
  }

  static QVector<JTableColumn> getColumns()
  {
    QVector<JTableColumn> c;
    c.push_back(JTableColumn(REMINDER_SQL_COL00, QObject::tr("Id")));
    c.push_back(JTableColumn(REMINDER_SQL_COL01, QObject::tr("Título"), false, true, JResizeMode::ResizeToContents));
    c.push_back(JTableColumn(REMINDER_SQL_COL02, QObject::tr("Mensagem"), false, false, JResizeMode::Stretch));
    c.push_back(JTableColumn(REMINDER_SQL_COL03, QObject::tr("Favorito"), false, false, JResizeMode::ResizeToContents));
    c.push_back(JTableColumn(REMINDER_SQL_COL04, QObject::tr("Fonte")));
    c.push_back(JTableColumn(REMINDER_SQL_COL04, QObject::tr("Tamanho")));
    return c;
  }
};

#endif // REMINDER_H

#ifndef REMINDER_H
#define REMINDER_H

#include <QString>

#define INVALID_REMINDER_ID -1

enum class ReminderColumn : int
{
  ID,
  Title,
  Message,
  Favorite,
  Capitalization,
  Size
};

struct Reminder
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

  int m_id;
  QString m_title;
  QString m_message;
  bool m_bFavorite;
  Size m_size;
  Capitalization m_capitalization;

  void clear()
  {
    m_id = INVALID_REMINDER_ID;
    m_title.clear();
    m_message.clear();
    m_bFavorite = false;
    m_size = Size::Normal;
    m_capitalization = Capitalization::Normal;
  }

  Reminder()
  {
    clear();
  }

  bool isValidID() const { return m_id != INVALID_REMINDER_ID; }
};

#endif // REMINDER_H

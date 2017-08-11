#ifndef REMINDER_H
#define REMINDER_H

#include <QString>

enum class ReminderColumn : int
{
  ID,
  Title,
  Message,
  Favorite,
  Capitalization,
  Size
};

enum ReminderCapitalization
{
  Normal,
  AllUppercase,
  AllLowercase
};

struct Reminder
{
  Reminder()
    : m_bFontSmall(false)
    , m_bFavorite(false)
    , m_capitalization(Normal) {}
  QString m_title;
  QString m_message;
  bool m_bFontSmall;
  bool m_bFavorite;
  ReminderCapitalization m_capitalization;
};

#endif // REMINDER_H

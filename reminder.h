#ifndef REMINDER_H
#define REMINDER_H

#include <QString>

struct Reminder
{
  Reminder() : m_bFontSmall(true) {}
  QString m_title;
  QString m_message;
  bool m_bFontSmall;
};

#endif // REMINDER_H

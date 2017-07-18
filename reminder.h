#ifndef REMINDER_H
#define REMINDER_H

#include <QString>

struct Reminder
{
  enum FontType
  {
    AllUppercase,
    AllLowercase,
    Normal
  };

  Reminder()
    : m_bFontSmall(false)
    , m_bfontType(AllUppercase) {}
  QString m_title;
  QString m_message;
  bool m_bFontSmall;
  FontType m_bfontType;
};

#endif // REMINDER_H

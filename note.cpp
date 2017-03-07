#include "note.h"
#include <QStringList>

QString Note::format(const QString& str, bool b3places /*= false*/)
{
  return format(str.toDouble(), b3places);
}

QString Note::format(double d, bool b3places /*= false*/)
{
  return QString::number(d, 'f', b3places ? 3 : 2);
}

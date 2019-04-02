#ifndef JITEMHELPER_H
#define JITEMHELPER_H

#include "jitemsql.h"

namespace JItemHelper
{
JItemSQL* create(const QString& tableName);
JItemSQL* create(const QString& tableName, Id id);
bool authenticationToInsertUpdate(const QString& tableName);
bool authenticationToRemove(const QString& tableName);
QString text(const QString& tableName);
QString icon(const QString& tableName);
}
#endif // JITEMHELPER_H

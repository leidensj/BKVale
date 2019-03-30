#ifndef JITEMHELPER_H
#define JITEMHELPER_H

#include "jitemsql.h"

namespace JItemHelper
{
JItemSQL* create(const QString& tableName);
JItemSQL* create(const QString& tableName, Id id);
}
#endif // JITEMHELPER_H

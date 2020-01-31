#ifndef JItemEx_H
#define JItemEx_H

#include "jitemsql.h"

class QObject;
class JItemView;

namespace JItemEx
{
JItemSQL* create(const QString& tableName);
JItemSQL* create(const QString& tableName, Id id);
bool authenticationToInsertUpdate(const QString& tableName);
bool authenticationToRemove(const QString& tableName);
QString text(const QString& tableName);
QString icon(const QString& tableName);
JModel* model(const QString& tableName, QObject* parent);
JItemView* view(const QString& tableName);
}
#endif // JItemEx_H

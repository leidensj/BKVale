#ifndef JItemEx_H
#define JItemEx_H

#include "jitemsql.h"

class QObject;
class JItemView;
class QWidget;

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
bool select(JItemSQL& o, QWidget* parent = nullptr);
bool remove(const Id& id, const QString& tableName, bool bNeedsConfirmation = true, QWidget* parent = nullptr);
void remove(const QVector<Id>& ids, const QString& tableName, QWidget* parent = nullptr);
bool save(const JItemSQL& o, const QString& tableName, QWidget* parent = nullptr);
}
#endif // JItemEx_H

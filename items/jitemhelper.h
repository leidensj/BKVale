#ifndef JITEM_HELPER_H
#define JITEM_HELPER_H

#include "jitemsql.h"

class QObject;
class JItemView;
class QWidget;

namespace JItemHelper
{
JItemSQL* create(const QString& tableName);
JItemSQL* create(const QString& tableName, Id id);
bool authenticationToSave(const QString& tableName);
bool authenticationToRemove(const QString& tableName);
QString text(Functionality::Idx idx);
QString icon(Functionality::Idx idx);
QString text(const QString& tableName);
QString icon(const QString& tableName);
JModel* model(const QString& tableName, QObject* parent);
JItemView* view(const QString& tableName);
bool select(JItemSQL& o, QWidget* parent = nullptr);
void remove(const Ids& ids, const QString& tableName, QWidget* parent = nullptr);
bool save(const JItemSQL& o, QWidget* parent = nullptr);
bool print(const JItemSQL& o, QVariant* options = nullptr, QWidget* parent = nullptr);
}
#endif // JITEM_HELPER_H

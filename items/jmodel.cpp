#include "jmodel.h"

JModel::JModel(QObject *parent)
  : QSqlQueryModel(parent)
{

}

void JModel::select(const QString& query)
{
  QSqlQueryModel::setQuery(query.isEmpty()
                           ? getStrQuery()
                           : query,
                           QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
}

void JModel::selectFilter(const QString& filter)
{
  QString strQuery = getStrQuery() + " WHERE " + filter;
  JModel::select(strQuery);
}

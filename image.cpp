#include "image.h"
#include "jmodel.h"

class ImageModel : public JModel
{
public:
  ImageModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     IMAGE_SQL_COL01 ","
                     IMAGE_SQL_COL02
                     " FROM "
                     IMAGE_SQL_TABLE_NAME);
    return strQuery;
  }

  void select(QHeaderView* header)
  {
    JModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Imagem"));
    setHeaderData(2, Qt::Horizontal, tr("Imagem"));
    if (header != nullptr && header->count() == 3)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
      header->hideSection(2);
    }
  }

  QVariant data(const QModelIndex &idx, int role) const
  {
    QVariant value = QSqlQueryModel::data(idx, role);
    if (role == Qt::DecorationRole)
    {
      if (idx.column() == 1)
      {
        QPixmap px(QSize(16, 16));
        px.loadFromData(QSqlQueryModel::data(createIndex(idx.row(), 2),
                                             Qt::EditRole).toByteArray());
        value = QVariant::fromValue(QIcon(px));
      }
    }
    return value;
  }
};

Image::Image()
{
  clear();
}

void Image::clear()
{
  m_id.clear();
  m_name.clear();
  m_image.clear();
}

bool Image::operator !=(const JItem& other) const
{
  const Image& another = dynamic_cast<const Image&>(other);
  return
      m_name != another.m_name ||
      m_image != another.m_image;
}

bool Image::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Image::isValid() const
{
  return !m_name.isEmpty() && !m_image.isEmpty();
}

QString Image::SQL_tableName() const
{
  return IMAGE_SQL_TABLE_NAME;
}

bool Image::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " IMAGE_SQL_TABLE_NAME " ("
                IMAGE_SQL_COL01 ","
                IMAGE_SQL_COL02 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");
  query.bindValue(":_v01", m_name);
  query.bindValue(":_v02", m_image);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool Image::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " IMAGE_SQL_TABLE_NAME " SET "
                IMAGE_SQL_COL01 " = (:_v01),"
                IMAGE_SQL_COL02 " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_name);
  query.bindValue(":_v02", m_image);
  return query.exec();
}

bool Image::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                IMAGE_SQL_COL01 ","
                IMAGE_SQL_COL02
                " FROM " IMAGE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_name = query.value(0).toString();
      m_image = query.value(1).toByteArray();
    }
    else
    {
      error = "Imagem não encontrada.";
      bSuccess = false;
    }
  }

  return bSuccess;
}

bool Image::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " IMAGE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

JModel* Image::SQL_table_model(QObject* parent) const
{
  return new ImageModel(parent);
}

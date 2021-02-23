#include "couponmodel.h"
#include "items/coupon.h"
#include <QDate>

CouponModel::CouponModel(QObject *parent)
  : JModel(parent)
{

}

QString CouponModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   COUPON_SQL_COL_TYP ","
                   COUPON_SQL_COL_COD ","
                   COUPON_SQL_COL_CDT ","
                   COUPON_SQL_COL_RED ","
                   COUPON_SQL_COL_EXP ","
                   COUPON_SQL_COL_EDT
                   " FROM "
                   CATEGORY_SQL_TABLE_NAME);
  return strQuery;
}

void CouponModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Tipo"));
  setHeaderData(2, Qt::Horizontal, tr("Código"));
  setHeaderData(3, Qt::Horizontal, tr("Data"));
  setHeaderData(4, Qt::Horizontal, tr("Resgatado"));
  setHeaderData(5, Qt::Horizontal, tr("Expirado"));
  setHeaderData(6, Qt::Horizontal, tr("Data de Expiração"));
  if (header != nullptr && header->count() == 6)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    header->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(5, QHeaderView::ResizeMode::ResizeToContents);
    header->hideSection(6);
  }
}

QVariant CouponModel::data(const QModelIndex &idx, int role) const
{
  QVariant value = QSqlQueryModel::data(idx, role);
  if (role == Qt::DisplayRole)
  {
    switch (idx.column())
    {
      case 1:
        value = Coupon::st_strType((Coupon::Type)value.toInt());
        break;
      case 2:
        break;
      case 3:
        value = QDate::fromString(value.toString(), Qt::ISODate).toString("yyyy/MM/dd");
        break;
      case 4:
        value = value.toBool() ? "X" : "";
        break;
      case 5:
      {
        bool bRedeemed = QSqlQueryModel::data(idx.sibling(idx.row(), idx.column() - 1), role).toBool();
        QDate dtExpiration = QSqlQueryModel::data(idx.sibling(idx.row(), idx.column() + 1), role).toDate();
        bool bExpired = !bRedeemed && DateTime::server().date() > dtExpiration;
        value = bExpired ? "X" : "";
      } break;
      case 6:
        break;
      default:
        break;
    }
  }
  return value;
}

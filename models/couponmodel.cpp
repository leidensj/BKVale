#include "couponmodel.h"
#include "items/coupon.h"
#include <QDate>

CouponModel::CouponModel(QObject *parent)
  : JModel(parent)
{

}

QString CouponModel::getStrQuery()
{
  QString str;
  str += "WITH " COUPON_SQL_TABLE_NAME_TEMP " AS ( "
         "SELECT " SQL_COLID ", "
         COUPON_SQL_COL_TYP ", "
         "TO_CHAR(" COUPON_SQL_COL_CDT ", 'YYYY/MM/DD hh:mm:ss') AS " COUPON_SQL_COL_TEMP_CDT ", "
         "CASE " COUPON_SQL_COL_TYP " WHEN 0 THEN TO_CHAR(" COUPON_SQL_COL_PCT ", '999,999.99%') "
                    "WHEN 1 THEN TO_CHAR(" COUPON_SQL_COL_VAL ", 'R$999,999.99') "
                    "ELSE 'Produtos' END AS " COUPON_SQL_COL_TEMP_CPN " ,"
         COUPON_SQL_COL_COD " ,"
         "CASE " COUPON_SQL_COL_EXP " WHEN FALSE THEN '' "
                                     "ELSE CASE " COUPON_SQL_COL_RED " WHEN TRUE THEN ''"
                                                                     "ELSE TO_CHAR(" COUPON_SQL_COL_EDT ",'YYYY/MM/DD') END END AS " COUPON_SQL_COL_TEMP_EDT ", "
         "CASE " COUPON_SQL_COL_EXP " AND (NOT " COUPON_SQL_COL_RED  ") AND (" COUPON_SQL_COL_EDT "< (SELECT NOW()::Date)) WHEN FALSE THEN FALSE ELSE TRUE END AS " COUPON_SQL_COL_TEMP_EXP ", "
         "CASE " COUPON_SQL_COL_RED " WHEN FALSE THEN '' ELSE TO_CHAR(" COUPON_SQL_COL_RDT ", 'YYYY/MM/DD hh:mm:ss') END AS " COUPON_SQL_COL_TEMP_RDT ", "
         COUPON_SQL_COL_RED " FROM  " COUPON_SQL_TABLE_NAME ") "
         "SELECT * FROM " COUPON_SQL_TABLE_NAME_TEMP;
  return str;
}

void CouponModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData((int)Column::Id, Qt::Horizontal, tr("ID"));
  setHeaderData((int)Column::Type, Qt::Horizontal, tr("Tipo"));
  setHeaderData((int)Column::Date, Qt::Horizontal, tr("Data"));
  setHeaderData((int)Column::Coupon, Qt::Horizontal, tr("Cupom"));
  setHeaderData((int)Column::Code, Qt::Horizontal, tr("Código"));
  setHeaderData((int)Column::Expiration, Qt::Horizontal, tr("Expiração"));
  setHeaderData((int)Column::Expired, Qt::Horizontal, tr("Expirado"));
  setHeaderData((int)Column::Redemption, Qt::Horizontal, tr("Resgate"));
  setHeaderData((int)Column::Redeemed, Qt::Horizontal, tr("Resgatado"));

  header->hideSection((int)Column::Id);
  header->hideSection((int)Column::Type);
  header->hideSection((int)Column::Expired);
  header->hideSection((int)Column::Redeemed);
  header->setSectionResizeMode((int)Column::Coupon, QHeaderView::ResizeMode::ResizeToContents);
  header->setSectionResizeMode((int)Column::Date, QHeaderView::ResizeMode::ResizeToContents);
  header->setSectionResizeMode((int)Column::Code, QHeaderView::ResizeMode::Stretch);
  header->setSectionResizeMode((int)Column::Redemption, QHeaderView::ResizeMode::ResizeToContents);
  header->setSectionResizeMode((int)Column::Expiration, QHeaderView::ResizeMode::ResizeToContents);
}

QVariant CouponModel::data(const QModelIndex &idx, int role) const
{
  QVariant value = QSqlQueryModel::data(idx, role);
  if (role == Qt::BackgroundRole)
  {
      bool bRedeemed = QSqlQueryModel::data(idx.siblingAtColumn((int)Column::Redeemed), Qt::EditRole).toBool();
      bool bExpired = QSqlQueryModel::data(idx.siblingAtColumn((int)Column::Expired), Qt::EditRole).toBool();
      if (bRedeemed)
        value = QColor(200, 255, 200);
      else if (bExpired)
        value = QColor(255, 200, 200);
  }
  return value;
}

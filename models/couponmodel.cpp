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
                   COUPON_SQL_COL_RDT ","
                   COUPON_SQL_COL_EXP ","
                   COUPON_SQL_COL_EDT ","
                   COUPON_SQL_COL_PCT ","
                   COUPON_SQL_COL_VAL
                   " FROM "
                   COUPON_SQL_TABLE_NAME);
  return strQuery;
}

void CouponModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData((int)Column::Id, Qt::Horizontal, tr("ID"));
  setHeaderData((int)Column::Coupon, Qt::Horizontal, tr("Cupom"));
  setHeaderData((int)Column::Code, Qt::Horizontal, tr("Código"));
  setHeaderData((int)Column::Date, Qt::Horizontal, tr("Data"));
  setHeaderData((int)Column::Redemption, Qt::Horizontal, tr("Resgate"));
  setHeaderData((int)Column::RedemptionDate, Qt::Horizontal, tr("Data do Resgate"));
  setHeaderData((int)Column::Expiration, Qt::Horizontal, tr("Expiração"));
  setHeaderData((int)Column::ExpirationDate, Qt::Horizontal, tr("Data de Expiração"));
  setHeaderData((int)Column::Percentage, Qt::Horizontal, tr("Porcentagem"));
  setHeaderData((int)Column::Value, Qt::Horizontal, tr("Valor"));
  if (header != nullptr && header->count() == 10)
  {
    header->hideSection((int)Column::Id);
    header->setSectionResizeMode((int)Column::Coupon, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode((int)Column::Code, QHeaderView::ResizeMode::Stretch);
    header->setSectionResizeMode((int)Column::Date, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode((int)Column::Redemption, QHeaderView::ResizeMode::ResizeToContents);
    header->hideSection((int)Column::RedemptionDate);
    header->setSectionResizeMode((int)Column::Expiration, QHeaderView::ResizeMode::ResizeToContents);
    header->hideSection((int)Column::ExpirationDate);
    header->hideSection((int)Column::Percentage);
    header->hideSection((int)Column::Value);
  }
}

QVariant CouponModel::data(const QModelIndex &idx, int role) const
{
  QVariant value = QSqlQueryModel::data(idx, role);
  if (role == Qt::DisplayRole)
  {
    switch ((Column)idx.column())
    {
      case Column::Coupon:
        switch ((Coupon::Type)value.toInt())
        {
          case Coupon::Type::Percentage:
            value = Data::strPercentage(QSqlQueryModel::data(idx.sibling(idx.row(), (int)Column::Percentage), role).toInt());
            break;
          case Coupon::Type::Value:
            value = Data::strMoney(QSqlQueryModel::data(idx.sibling(idx.row(), (int)Column::Value), role).toInt());
            break;
          case Coupon::Type::Product:
            value = tr("Produtos");
          default:
            break;
        }
        break;
      case Column::Date:
        value = value.toDateTime().toString("yyyy/MM/dd hh:mm:ss");
        break;
      case Column::Redemption:
      {
          QDateTime dtRedemption = QSqlQueryModel::data(idx.sibling(idx.row(), (int)Column::RedemptionDate), role).toDateTime();
          value = value.toBool() ? dtRedemption.toString("yyyy/MM/dd hh:mm:ss") : "";
      } break;
      case Column::Expiration:
      {
        bool bExpires = value.toBool();
        bool bRedeemed = QSqlQueryModel::data(idx.sibling(idx.row(), (int)Column::Redemption), role).toBool();
        QDate dtExpiration = QSqlQueryModel::data(idx.sibling(idx.row(), (int)Column::ExpirationDate), role).toDate();
        if (!bExpires || bRedeemed)
          value = "";
        else
          value = dtExpiration.toString("yyyy/MM/dd");
      } break;
      default:
        break;
    }
  }
  else if (role == Qt::ToolTipRole)
  {
    if (idx.column() == (int)Column::Coupon)
    {
      if (data(idx, Qt::EditRole).toInt() == (int)Coupon::Type::Product)
      {
        Coupon o(data(idx.sibling(idx.row(), (int)Column::Id), Qt::EditRole).toLongLong());
        QString error;
        o.SQL_select(error);
        value = o.strCoupon();
      }
    }
  }
  else if (role == Qt::BackgroundRole)
  {
    switch ((Column)idx.column())
    {
      case Column::Expiration:
      case Column::Redemption:
      {
        bool bExpires = QSqlQueryModel::data(idx.sibling(idx.row(), (int)Column::Expiration), Qt::DisplayRole).toBool();
        bool bRedeemed = QSqlQueryModel::data(idx.sibling(idx.row(), (int)Column::Redemption), Qt::DisplayRole).toBool();
        QDate dtExpiration = QSqlQueryModel::data(idx.sibling(idx.row(), (int)Column::ExpirationDate), Qt::DisplayRole).toDate();
        bool bExpired = bExpires && !bRedeemed && DateTime::server().date() > dtExpiration;
        if (bExpired)
          value = QColor(255, 200, 200);
      } break;
      default:
        break;
    }
  }
  return value;
}

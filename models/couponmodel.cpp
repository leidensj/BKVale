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
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Cupom"));
  setHeaderData(2, Qt::Horizontal, tr("Código"));
  setHeaderData(3, Qt::Horizontal, tr("Data"));
  setHeaderData(4, Qt::Horizontal, tr("Resgate"));
  setHeaderData(5, Qt::Horizontal, tr("Data do Resgate"));
  setHeaderData(6, Qt::Horizontal, tr("Expiração"));
  setHeaderData(7, Qt::Horizontal, tr("Data de Expiração"));
  setHeaderData(8, Qt::Horizontal, tr("Porcentagem"));
  setHeaderData(9, Qt::Horizontal, tr("Valor"));
  if (header != nullptr && header->count() == 10)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    header->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
    header->hideSection(5);
    header->setSectionResizeMode(6, QHeaderView::ResizeMode::ResizeToContents);
    header->hideSection(7);
    header->hideSection(8);
    header->hideSection(9);
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
        switch ((Coupon::Type)value.toInt())
        {
          case Coupon::Type::Percentage:
            value = Data::strPercentage(QSqlQueryModel::data(idx.sibling(idx.row(), idx.column() + 6), role).toInt());
            break;
          case Coupon::Type::Value:
            value = Data::strMoney(QSqlQueryModel::data(idx.sibling(idx.row(), idx.column() + 7), role).toInt());
            break;
          case Coupon::Type::Product:
            value = tr("Produtos");
          default:
            break;
        }
        break;
      case 3:
        value = value.toDateTime().toString("yyyy/MM/dd hh:mm:ss");
        break;
      case 4:
      {
        QDate dtRedemption = QSqlQueryModel::data(idx.sibling(idx.row(), idx.column() + 1), role).toDate();
        value = value.toBool() ? "Sim " + dtRedemption.toString("yyyy/MM/dd hh:mm:ss") : "";
      } break;
      case 6:
      {
        bool bExpires = value.toBool();
        bool bRedeemed = QSqlQueryModel::data(idx.sibling(idx.row(), idx.column() - 1), role).toBool();
        QDate dtExpiration = QSqlQueryModel::data(idx.sibling(idx.row(), idx.column() + 1), role).toDate();
        bool bExpired = bExpires && !bRedeemed && DateTime::server().date() > dtExpiration;
        if (!bExpires)
          value = "";
        else
          value = (bExpired ? "Sim " : "Não ") + dtExpiration.toString("yyyy/MM/dd");
      } break;
      default:
        break;
    }
  }
  else if (role == Qt::ToolTipRole)
  {
    if (data(idx, Qt::EditRole).toInt() == (int)Coupon::Type::Product)
    {
      Coupon o(data(idx.sibling(idx.row(), 0), Qt::EditRole).toLongLong());
      QString error;
      o.SQL_select(error);
      value = o.strCoupon();
    }
  }
  return value;
}

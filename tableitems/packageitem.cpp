#include "packageitem.h"
#include "editors/packageeditor.h"

Package PackageItem::st_fromVariant(const QVariant& v)
{
  Package pck;
  for (int i = 0; i != v.toList().size(); ++i)
  {
    switch (i)
    {
      case 0:
        pck.m_bIsPackage = v.toList().at(i).toBool();
        break;
      case 1:
        pck.m_unity = v.toList().at(i).toString();
        break;
      case 2:
        pck.m_ammount = v.toList().at(i).toDouble();
        break;
      default:
        break;
    }
  }
  return pck;
}

QVariant PackageItem::st_toVariant(const Package& pck)
{
  QList<QVariant> lst;
  lst.push_back(pck.m_bIsPackage);
  lst.push_back(pck.m_unity);
  lst.push_back(pck.m_ammount);
  return QVariant(lst);
}

PackageItem::PackageItem()
{
  setForeground(QBrush(QColor(Qt::darkGray)));
  setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void PackageItem::setProductUnity(const QString& productUnity)
{
  m_productUnity = productUnity;
  setValue(st_toVariant(Package()));
}

void PackageItem::activate()
{
  PackageEditor dlg;
  dlg.setPackage(st_fromVariant(getValue()), m_productUnity);
  if (dlg.exec())
    setValue(st_toVariant(dlg.getPackage()));
}

void PackageItem::evaluate()
{

}

void PackageItem::erase()
{
  setValue(st_toVariant(Package()));
}

void PackageItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, v);
  setText(st_fromVariant(v).strFormattedUnity(m_productUnity));
}

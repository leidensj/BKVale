#include "packageitem.h"
#include "packageeditor.h"

Package PackageItem::toPackage(const QVariant& v)
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

QVariant PackageItem::toVariant(const Package& pck)
{
  QList<QVariant> lst;
  lst.push_back(pck.m_bIsPackage);
  lst.push_back(pck.m_unity);
  lst.push_back(pck.m_ammount);
  return QVariant(lst);
}

PackageItem::PackageItem()
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void PackageItem::setProductUnity(const QString& productUnity)
{
  m_productUnity = productUnity;
}

void PackageItem::activate()
{
  PackageEditor dlg;
  dlg.setPackage(toPackage(getValue()), m_productUnity);
  if (dlg.exec())
    setValue(toVariant(dlg.getPackage()));
}

void PackageItem::evaluate()
{

}

void PackageItem::erase()
{
  setValue(toVariant(Package()));
}

void PackageItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, v);
  setText(toPackage(v).strFormattedUnity(m_productUnity));
}

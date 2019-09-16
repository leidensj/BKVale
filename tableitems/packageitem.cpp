#include "packageitem.h"
#include "packageeditor.h"

PackageItem::PackageItem()
  : m_dlg(nullptr)
{
  m_dlg = new PackageEditor(tableWidget());
}

void PackageItem::activate()
{
  if (m_dlg->exec())
  {
    QList<QVariant> lst;
    Package pck = m_dlg->getPackage();
    lst.push_back(pck.m_bIsPackage);
    lst.push_back(pck.m_unity);
    lst.push_back(pck.m_ammount);
    setValue(QVariant(lst));
  }
}

void PackageItem::evaluate()
{

}

void PackageItem::erase()
{

  setData(Qt::UserRole, INVALID_ID);
}

void PackageItem::setValue(const QVariant& v)
{
  m_dlg->setParent(tableWidget());
  Package pck;
  QString productUnity;
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
      case 3:
        productUnity = v.toList().at(i).toString();
        break;
      default:
        break;
    }
  }
  m_dlg->setPackage(pck, productUnity);
  setData(Qt::UserRole, v);
  setText(pck.strFormattedUnity(productUnity));
}

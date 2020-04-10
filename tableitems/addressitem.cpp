#include "addressitem.h"
#include "editors/addresseditor.h"

Address AddressItem::st_fromVariant(const QVariant& v)
{
  Address o;
  for (int i = 0; i != v.toList().size(); ++i)
  {
    switch (i)
    {
      case 0:
        o.m_cep = v.toList().at(i).toString();
        break;
      case 1:
        o.m_neighborhood = v.toList().at(i).toString();
        break;
      case 2:
        o.m_street = v.toList().at(i).toString();
        break;
      case 3:
        o.m_number = v.toList().at(i).toInt();
        break;
      case 4:
        o.m_city = v.toList().at(i).toString();
        break;
      case 5:
        o.m_state = (Address::EBRState)v.toList().at(i).toInt();
        break;
      case 6:
        o.m_complement = v.toList().at(i).toString();
        break;
      case 7:
        o.m_reference = v.toList().at(i).toString();
        break;
      default:
        break;
    }
  }
  return o;
}

QVariant AddressItem::st_toVariant(const Address& o)
{
  QList<QVariant> lst;
  lst.push_back(o.m_cep);
  lst.push_back(o.m_neighborhood);
  lst.push_back(o.m_street);
  lst.push_back(o.m_number);
  lst.push_back(o.m_city);
  lst.push_back((int)o.m_state);
  lst.push_back(o.m_complement);
  lst.push_back(o.m_reference);
  return QVariant(lst);
}

AddressItem::AddressItem()
{
  setForeground(QBrush(QColor(Qt::darkGray)));
  setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void AddressItem::activate()
{
  AddressEditor dlg;
  dlg.setAddress(st_fromVariant(getValue()));
  if (dlg.exec())
    setValue(st_toVariant(dlg.getAddress()));
}

void AddressItem::evaluate()
{

}

void AddressItem::erase()
{
  setValue(st_toVariant(Address()));
}

void AddressItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, v);
  setText(st_fromVariant(v).name());
}

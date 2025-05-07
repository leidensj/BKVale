#include "cashclosinginfotable.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/textitem.h"
#include "tableitems/doubleitem.h"

CashClosingInfoTable::CashClosingInfoTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(3);
  QStringList headers;
  headers << "Informação" << "Tipo" << "Valor";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->hideSection((int)Column::Type);
  horizontalHeader()->setSectionResizeMode((int)Column::Info, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Value, QHeaderView::Stretch);
}

void CashClosingInfoTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itInfo = new TextItem(Text::Input::All, true);
  auto itType = new TextItem(Text::Input::All, true);
  auto itValue = new DoubleItem(Data::Type::Money, DoubleItem::Color::Foreground2, false, false);

  blockSignals(true);
  setItem(row, (int)Column::Info, itInfo);
  setItem(row, (int)Column::Type, itType);
  setItem(row, (int)Column::Value, itValue);
  blockSignals(false);

  setCurrentItem(itValue);
  setFocus();
}

void CashClosingInfoTable::addRowAndActivate()
{
  addRow();
}

void CashClosingInfoTable::get(QVector<CashClosingInfo>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    CashClosingInfo o;
    o.m_iname = getItem(row, (int)Column::Info)->getValue().toString();
    o.m_itype = getItem(row, (int)Column::Type)->getValue().toInt();
    o.m_value = getItem(row, (int)Column::Value)->getValue();
    v.push_back(o);
  }
}

void CashClosingInfoTable::set(const QVector<CashClosingInfo>& v, bool bClear)
{
  if (bClear)
    removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    auto it = takeItem(row, (int)Column::Value);
    delete it;
    switch ((Data::Type)v.at(i).m_itype)
    {
      case Data::Type::Money:
        it = new DoubleItem(Data::Type::Money, DoubleItem::Color::None);
        break;
      case Data::Type::Integer:
        it = new DoubleItem(Data::Type::Integer, DoubleItem::Color::None);
        break;
      case Data::Type::Percentage:
        it = new DoubleItem(Data::Type::Percentage, DoubleItem::Color::None);
        break;
      case Data::Type::Ammount:
        it = new DoubleItem(Data::Type::Ammount, DoubleItem::Color::None);
        break;
      case Data::Type::Text:
      default:
        it = new TextItem(Text::Input::ASCII, true);
        break;
    }

    blockSignals(true);
    setItem(row, (int)Column::Value, it);
    blockSignals(false);

    auto iti = getItem(row, (int)Column::Info);
    iti->setReadOnly(true);
    iti->setValue(v.at(i).m_iname);
    auto itt = getItem(row, (int)Column::Type);
    itt->setReadOnly(true);
    itt->setValue(v.at(i).m_itype);
    auto itv =getItem(row, (int)Column::Value);
    if (itt->getValue().toInt() != (int)Data::Type::Text || !itv->getValue().toString().isEmpty())
        itv->setValue(v.at(i).m_value);
  }
}

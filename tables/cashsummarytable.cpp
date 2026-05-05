#include "cashsummarytable.h"
#include <QHeaderView>
#include <QKeyEvent>
#include "tableitems/textitem.h"
#include "tableitems/toggleitem.h"

CashSummaryTable::CashSummaryTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(2);
  QStringList headers;
  headers << "Nome" << "Status";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Name, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode((int)Column::Status, QHeaderView::ResizeToContents);
}

void CashSummaryTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itName = new TextItem(Text::Input::All, false);
  auto itStatus = new ToggleItem(tr("Exibir"), Qt::black, QColor(200, 255, 200));
  itName->setReadOnly(true);

  blockSignals(true);
  setItem(row, (int)Column::Name, itName);
  setItem(row, (int)Column::Status, itStatus);
  blockSignals(false);

  setCurrentItem(itStatus);
  setFocus();
}

void CashSummaryTable::addRowAndActivate()
{
  addRow();
}

void CashSummaryTable::get(QVector<CashSummary>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(i);
    CashSummary o;
    o.m_item = (CashSummary::Item)getItem(row, (int)Column::Name)->getCustomData().toInt();
    o.m_bShow = getItem(row, (int)Column::Status)->getValue().toBool();
    v.push_back(o);
  }
}

void CashSummaryTable::set(const QVector<CashSummary>& v, bool bClear)
{
  if (bClear)
    removeAllItems();

  auto v2 = v;

  if (v2.isEmpty())
  {
    for (int i = 0; i != (int)CashSummary::Item::NONE; ++i)
    {
      CashSummary o;
      o.m_item = (CashSummary::Item)i;
      o.m_bShow = false;
      v2.push_back(o);
    }
  }

  for (int i = 0; i != v2.size(); ++i)
  {
    addRow();
    int row = rowCount() - 1;
    getItem(row, (int)Column::Name)->setValue(CashSummary::st_ItemToText(v2.at(i).m_item));
    getItem(row, (int)Column::Name)->setCustomData((int)v2.at(i).m_item);
    getItem(row, (int)Column::Status)->setValue(v2.at(i).m_bShow);
  }
}

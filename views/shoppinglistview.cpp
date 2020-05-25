#include "shoppinglistview.h"
#include <QCheckBox>
#include <QLayout>
#include "controls/databasepicker.h"
#include "widgets/jlineedit.h"
#include "widgets/jspinbox.h"
#include "widgets/jaddremovebuttons.h"
#include "tables/shoppinglisttable.h"
#include "items/jitemex.h"
#include "editors/packageeditor.h"
#include <QSplitter>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QLabel>
#include <QFormLayout>
#include <QGroupBox>

namespace {
QPushButton* monthButtonFactory(int n)
{
  QPushButton* btn = new QPushButton;
  if (n >= 0 && n < 31)
  {
    btn = new QPushButton;
    btn->setFlat(true);
    btn->setIconSize(QSize(32, 32));
    btn->setCheckable(true);
    QString strIcon = ":/icons/res/cal" + QString::number(n + 1) + ".png";
    btn->setIcon(QIcon(strIcon));
  }
  return btn;
}

QPushButton* weekButtonFactory(int n)
{
  QPushButton* btn = new QPushButton;
  if (n >= 0 && n < 7)
  {
    btn = new QPushButton;
    btn->setFlat(true);
    btn->setIconSize(QSize(32, 32));
    btn->setCheckable(true);
    QString strIcon = ":/icons/res/calw" + QString::number(n + 1) + ".png";
    btn->setIcon(QIcon(strIcon));
  }
  return btn;
}
}

ShoppingListView::ShoppingListView(QWidget* parent)
  : JItemView(SHOPPING_LIST_SQL_TABLE_NAME, parent)
  , m_edTitle(nullptr)
  , m_snLines(nullptr)
  , m_teDescription(nullptr)
  , m_supplierPicker(nullptr)
  , m_imagePicker(nullptr)
  , m_table(nullptr)
  , m_btns(nullptr)
{
  m_supplierPicker = new DatabasePicker(SUPPLIER_SQL_TABLE_NAME);
  m_imagePicker = new DatabasePicker(IMAGE_SQL_TABLE_NAME);
  m_edTitle = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);

  addViewButton(PRODUCT_SQL_TABLE_NAME);
  addViewButton(SUPPLIER_SQL_TABLE_NAME);
  addViewButton(IMAGE_SQL_TABLE_NAME);

  m_snLines = new JSpinBox;
  m_snLines->setMinimum(0);
  m_snLines->setMaximum(30);

  m_teDescription = new QPlainTextEdit;

  for (int i = 0; i != 31; ++i)
    m_vbtnMonth[i] = monthButtonFactory(i);

  for (int i = 0; i != 7; ++i)
    m_vbtnWeek[i] = weekButtonFactory(i);

  QHBoxLayout* monthLayout1 = new QHBoxLayout;
  monthLayout1->setContentsMargins(0, 0, 0, 0);
  monthLayout1->setAlignment(Qt::AlignLeft);
  for (int i = 0; i != 7; ++i)
    monthLayout1->addWidget(m_vbtnMonth[i]);

  QHBoxLayout* monthLayout2 = new QHBoxLayout;
  monthLayout2->setContentsMargins(0, 0, 0, 0);
  monthLayout2->setAlignment(Qt::AlignLeft);
  for (int i = 7; i != 14; ++i)
    monthLayout2->addWidget(m_vbtnMonth[i]);

  QHBoxLayout* monthLayout3 = new QHBoxLayout;
  monthLayout3->setContentsMargins(0, 0, 0, 0);
  monthLayout3->setAlignment(Qt::AlignLeft);
  for (int i = 14; i != 21; ++i)
    monthLayout3->addWidget(m_vbtnMonth[i]);

  QHBoxLayout* monthLayout4 = new QHBoxLayout;
  monthLayout4->setContentsMargins(0, 0, 0, 0);
  monthLayout4->setAlignment(Qt::AlignLeft);
  for (int i = 21; i != 28; ++i)
    monthLayout4->addWidget(m_vbtnMonth[i]);

  QHBoxLayout* monthLayout5 = new QHBoxLayout;
  monthLayout5->setContentsMargins(0, 0, 0, 0);
  monthLayout5->setAlignment(Qt::AlignLeft);
  for (int i = 28; i != 31; ++i)
    monthLayout5->addWidget(m_vbtnMonth[i]);

  QHBoxLayout* weekLayout1 = new QHBoxLayout;
  weekLayout1->setContentsMargins(0, 0, 0, 0);
  weekLayout1->setAlignment(Qt::AlignLeft);
  for (int i = 0; i != 7; ++i)
    weekLayout1->addWidget(m_vbtnWeek[i]);

  QFormLayout* viewFormLayout = new QFormLayout;
  viewFormLayout->addRow(tr("Título:"), m_edTitle);
  viewFormLayout->addRow(m_supplierPicker->getTableText() + ":", m_supplierPicker);
  viewFormLayout->addRow(m_imagePicker->getTableText() + ":", m_imagePicker);
  viewFormLayout->addRow(tr("Linhas:"), m_snLines);
  viewFormLayout->addRow(tr("Descrição:"), m_teDescription);

  QVBoxLayout* calendarLayout = new QVBoxLayout;
  calendarLayout->setAlignment(Qt::AlignTop);
  calendarLayout->addWidget(new QLabel(tr("Dias que a lista de compras deve ser preenchida:")));
  calendarLayout->addWidget(new QLabel(tr("Semanal:")));
  calendarLayout->addLayout(weekLayout1);
  calendarLayout->addWidget(new QLabel(tr("Mensal:")));
  calendarLayout->addLayout(monthLayout1);
  calendarLayout->addLayout(monthLayout2);
  calendarLayout->addLayout(monthLayout3);
  calendarLayout->addLayout(monthLayout4);
  calendarLayout->addLayout(monthLayout5);

  m_btns = new JAddRemoveButtons;
  m_table = new ShoppingListTable(m_btns);

  QVBoxLayout* ltTable = new QVBoxLayout;
  ltTable->addWidget(m_btns);
  ltTable->addWidget(m_table);

  QFrame* tabView = new QFrame;
  tabView->setLayout(viewFormLayout);

  QFrame* tabTable = new QFrame;
  tabTable->setLayout(ltTable);

  QFrame* tabCalendar = new QFrame;
  tabCalendar->setLayout(calendarLayout);

  m_tab->addTab(tabView, QIcon(":/icons/res/details.png"), tr("Informações"));
  m_tab->addTab(tabTable, QIcon(":/icons/res/item.png"), tr("Produtos"));
  m_tab->addTab(tabCalendar, QIcon(":/icons/res/calendar.png"), tr("Calendário"));

  connect(m_supplierPicker, SIGNAL(changedSignal()), this, SLOT(updateControls()));

  setFocusWidgetOnClear(m_edTitle);
  clear();
}

void ShoppingListView::updateControls()
{
  m_table->showSupplierColumn(!m_supplierPicker->getFirstId().isValid());
}

void ShoppingListView::setItem(const JItemSQL& o)
{
  const ShoppingList& _o = dynamic_cast<const ShoppingList&>(o);
  m_edTitle->setText(_o.m_title);
  m_teDescription->setPlainText(_o.m_description);
  m_supplierPicker->addItem(_o.m_supplier);
  m_imagePicker->addItem(_o.m_image);
  m_snLines->setValue(_o.m_nLines);
  for (int i = 0; i != 7; ++i)
    m_vbtnWeek[i]->setChecked(_o.m_weekDays[i]);
  for (int i = 0; i != 31; ++i)
    m_vbtnMonth[i]->setChecked(_o.m_monthDays[i]);
  m_table->setListElements(_o.m_vItem);
  updateControls();
}

void ShoppingListView::getItem(JItemSQL& o) const
{
  ShoppingList& _o = dynamic_cast<ShoppingList&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_title = m_edTitle->text();
  _o.m_description = m_teDescription->toPlainText();
  _o.m_supplier.m_id = m_supplierPicker->getFirstId();
  _o.m_image.m_id = m_imagePicker->getFirstId();
  _o.m_nLines = m_snLines->value();
  m_table->getListElements(_o.m_vItem);
  for (int i = 0; i != 7; ++i)
    _o.m_weekDays[i] = m_vbtnWeek[i]->isChecked();
  for (int i = 0; i != 31; ++i)
    _o.m_monthDays[i] = m_vbtnMonth[i]->isChecked();
}

#include "shoppinglistview.h"
#include <QCheckBox>
#include <QLayout>
#include "widgets/jdatabasepicker.h"
#include "widgets/jlineedit.h"
#include "widgets/jspinbox.h"
#include "widgets/jaddremovebuttons.h"
#include "shoppinglisttable.h"
#include "packageeditor.h"
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
  m_supplierPicker = new JDatabasePicker(SUPPLIER_SQL_TABLE_NAME);
  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME);
  m_edTitle = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                            JLineEdit::st_defaultFlags1);

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
  viewFormLayout->addRow(m_supplierPicker->getText() + ":", m_supplierPicker);
  viewFormLayout->addRow(m_imagePicker->getText() + ":", m_imagePicker);
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
  m_table = new ShoppingListTable;

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
  connect(m_table, SIGNAL(changedSignal(bool)), this, SLOT(updateControls()));
  connect(m_table, SIGNAL(changedSignal(bool)), m_btns->m_btnRemove, SLOT(setEnabled(bool)));
  connect(m_btns->m_btnAdd, SIGNAL(clicked(bool)), m_table, SLOT(addItem()));
  connect(m_btns->m_btnRemove, SIGNAL(clicked(bool)), m_table, SLOT(removeItem()));
}

void ShoppingListView::updateControls()
{
  m_table->showSupplierColumn(!m_supplierPicker->getId().isValid());
}

void ShoppingListView::create()
{
  selectItem(ShoppingList());
  m_tab->setCurrentIndex(0);
  m_edTitle->setFocus();
  updateControls();
}

void ShoppingListView::setItem(const JItemSQL& o)
{
  m_ref = dynamic_cast<const ShoppingList&>(o);
  m_edTitle->setText(m_ref.m_title);
  m_teDescription->setPlainText(m_ref.m_description);
  m_supplierPicker->setItem(m_ref.m_supplier);
  m_imagePicker->setItem(m_ref.m_image);
  m_snLines->setValue(m_ref.m_nLines);
  for (int i = 0; i != 7; ++i)
    m_vbtnWeek[i]->setChecked(m_ref.m_weekDays[i]);
  for (int i = 0; i != 31; ++i)
    m_vbtnMonth[i]->setChecked(m_ref.m_monthDays[i]);
  m_table->removeAllItems();
  for (int i = 0; i != m_ref.m_vItem.size(); ++i)
    m_table->addItem(m_ref.m_vItem.at(i));
  updateControls();
}

const JItemSQL& ShoppingListView::getItem() const
{
  m_ref.clear(false);
  m_ref.m_title = m_edTitle->text();
  m_ref.m_description = m_teDescription->toPlainText();
  m_ref.m_supplier.m_id = m_supplierPicker->getId();
  m_ref.m_image.m_id = m_imagePicker->getId();
  m_ref.m_nLines = m_snLines->value();
  for (int i = 0; i != 7; ++i)
    m_ref.m_weekDays[i] = m_vbtnWeek[i]->isChecked();
  for (int i = 0; i != 31; ++i)
    m_ref.m_monthDays[i] = m_vbtnMonth[i]->isChecked();
  for (int i = 0; i != m_table->rowCount(); ++i)
    m_ref.m_vItem.push_back(dynamic_cast<const ShoppingListItem&>(m_table->getItem(i)));

  return m_ref;
}

Id ShoppingListView::getId() const
{
  return m_ref.m_id;
}

#include "shoppinglistview.h"
#include <QCheckBox>
#include <QLayout>
#include "jdatabasepicker.h"
#include "shoppinglisttable.h"
#include "packageeditor.h"
#include "jlineedit.h"
#include "jspinbox.h"
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
  , m_tableButtons(nullptr)
  , m_edTitle(nullptr)
  , m_snLines(nullptr)
  , m_teDescription(nullptr)
  , m_supplierPicker(nullptr)
  , m_imagePicker(nullptr)
  , m_table(nullptr)
{
  m_supplierPicker = new JDatabasePicker(PERSON_SQL_TABLE_NAME,
                                         tr("Fornecedor"),
                                         QIcon(":/icons/res/supplier.png"));
  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME,
                                      tr("Imagem"),
                                      QIcon(":/icons/res/icon.png"));
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
  viewFormLayout->addRow(m_supplierPicker->m_text + ":", m_supplierPicker);
  viewFormLayout->addRow(m_imagePicker->m_text + ":", m_imagePicker);
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

  m_table = new ShoppingListTable;
  m_tableButtons = new JTableButtons(m_table, SHOPPING_LIST_MAX_NUMBER_OF_ITEMS);
  QHBoxLayout* listLayout = new QHBoxLayout;
  listLayout->addWidget(m_table);
  listLayout->addWidget(m_tableButtons);

  QFrame* tabView = new QFrame;
  tabView->setLayout(viewFormLayout);

  QFrame* tabList = new QFrame;
  tabList->setLayout(listLayout);

  QFrame* tabCalendar = new QFrame;
  tabCalendar->setLayout(calendarLayout);

  m_tab->addTab(tabView,
                QIcon(":/icons/res/shopmgt.png"),
                tr("Lista de Compras"));
  m_tab->addTab(tabList,
                QIcon(":/icons/res/item.png"),
                tr("Produtos"));
  m_tab->addTab(tabCalendar,
                QIcon(":/icons/res/calendar.png"),
                tr("Calendário"));

  connect(m_supplierPicker, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_table, SIGNAL(changedSignal()), this, SLOT(updateControls()));

  m_supplierPicker->getDatabase()->setFixedFilter(PERSON_FILTER_SUPPLIER);
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

void ShoppingListView::setItem(const JItem& o)
{
  auto _o = dynamic_cast<const ShoppingList&>(o);
  m_currentId = _o.m_id;
  m_edTitle->setText(_o.m_title);
  m_teDescription->setPlainText(_o.m_description);
  m_supplierPicker->setItem(_o.m_supplier);
  m_imagePicker->setItem(_o.m_image);
  m_snLines->setValue(_o.m_nLines);
  for (int i = 0; i != 7; ++i)
    m_vbtnWeek[i]->setChecked(_o.m_weekDays[i]);
  for (int i = 0; i != 31; ++i)
    m_vbtnMonth[i]->setChecked(_o.m_monthDays[i]);
  m_table->removeAllItems();
  for (int i = 0; i != _o.m_vItem.size(); ++i)
    m_table->addItem(_o.m_vItem.at(i));
  updateControls();
}

const JItem& ShoppingListView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
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

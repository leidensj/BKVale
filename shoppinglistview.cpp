#include "shoppinglistview.h"
#include <QPushButton>
#include <QCheckBox>
#include <QLayout>
#include "jdatabase.h"
#include "jdatabasepicker.h"
#include "shoppinglisttable.h"
#include "jlineedit.h"
#include <QSplitter>
#include <QTabWidget>
#include <QLabel>

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
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_btnEdit(nullptr)
  , m_database(nullptr)
  , m_supplierPicker(nullptr)
  , m_edTitle(nullptr)
  , m_edDescription(nullptr)
  , m_cbPrintAmmount(nullptr)
  , m_cbPrintPrice(nullptr)
  , m_cbSupplierCalls(nullptr)
  , m_cbCallSupplier(nullptr)
  , m_cbWhatsapp(nullptr)
  , m_cbVisit(nullptr)
  , m_table(nullptr)
{
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
  m_btnCreate->setToolTip(tr("Novo (Ctrl+N)"));

  m_btnSave = new QPushButton;
  m_btnSave->setFlat(true);
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
  m_btnSave->setToolTip(tr("Salvar (Ctrl+S)"));

  m_btnAdd = new QPushButton;
  m_btnAdd->setFlat(true);
  m_btnAdd->setIconSize(QSize(24, 24));
  m_btnAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAdd->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));
  m_btnAdd->setToolTip(tr("Adicionar (Alt++)"));

  m_btnRemove = new QPushButton;
  m_btnRemove->setFlat(true);
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Minus));
  m_btnRemove->setToolTip(tr("Remover (Alt+-)"));

  m_btnEdit = new QPushButton;
  m_btnEdit->setFlat(true);
  m_btnEdit->setIconSize(QSize(24, 24));
  m_btnEdit->setIcon(QIcon(":/icons/res/binoculars.png"));
  m_btnEdit->setShortcut(QKeySequence(Qt::Key_F3));
  m_btnEdit->setToolTip(tr("Editar (F3)"));

  m_supplierPicker = new JDatabasePicker(tr("Fornecedor"), QIcon(":/icons/res/supplier.png"), true, true);
  m_edTitle = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edDescription = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_cbPrintAmmount = new QCheckBox;
  m_cbPrintAmmount->setText(tr("Imprimir quantidade recomendada"));
  m_cbPrintPrice = new QCheckBox;
  m_cbPrintPrice->setText(tr("Imprimir preço sugerido"));
  m_cbSupplierCalls = new QCheckBox;
  m_cbSupplierCalls->setText(tr("Ele liga"));
  m_cbSupplierCalls->setIcon(QIcon(":/icons/res/phone.png"));
  m_cbCallSupplier = new QCheckBox;
  m_cbCallSupplier->setText(tr("Ligar para ele"));
  m_cbCallSupplier->setIcon(QIcon(":/icons/res/phoneback.png"));
  m_cbWhatsapp = new QCheckBox;
  m_cbWhatsapp->setText(tr("Whatsapp"));
  m_cbWhatsapp->setIcon(QIcon(":/icons/res/whatsapp.png"));
  m_cbVisit = new QCheckBox;
  m_cbVisit->setText(tr("Visita"));
  m_cbVisit->setIcon(QIcon(":/icons/res/visit.png"));

  m_database = new JDatabase;

  for (int i = 0; i != 31; ++i)
    m_vbtnMonth.push_back(monthButtonFactory(i));

  for (int i = 0; i != 7; ++i)
    m_vbtnWeek.push_back(weekButtonFactory(i));

  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->setAlignment(Qt::AlignLeft);
  buttonLayout->setContentsMargins(0, 0, 0, 0);
  buttonLayout->addWidget(m_btnCreate);
  buttonLayout->addWidget(m_btnSave);

  QHBoxLayout* tableButtonLayout = new QHBoxLayout;
  tableButtonLayout->setAlignment(Qt::AlignLeft);
  tableButtonLayout->setContentsMargins(0, 0, 0, 0);
  tableButtonLayout->addWidget(m_btnAdd);
  tableButtonLayout->addWidget(m_btnRemove);
  tableButtonLayout->addWidget(m_btnEdit);

  QHBoxLayout* monthLayout1 = new QHBoxLayout;
  monthLayout1->setContentsMargins(0, 0, 0, 0);
  monthLayout1->setAlignment(Qt::AlignLeft);
  for (int i = 0; i != 7; ++i)
    monthLayout1->addWidget(m_vbtnMonth.at(i));

  QHBoxLayout* monthLayout2 = new QHBoxLayout;
  monthLayout2->setContentsMargins(0, 0, 0, 0);
  monthLayout2->setAlignment(Qt::AlignLeft);
  for (int i = 7; i != 14; ++i)
    monthLayout2->addWidget(m_vbtnMonth.at(i));

  QHBoxLayout* monthLayout3 = new QHBoxLayout;
  monthLayout3->setContentsMargins(0, 0, 0, 0);
  monthLayout3->setAlignment(Qt::AlignLeft);
  for (int i = 14; i != 21; ++i)
    monthLayout3->addWidget(m_vbtnMonth.at(i));

  QHBoxLayout* monthLayout4 = new QHBoxLayout;
  monthLayout4->setContentsMargins(0, 0, 0, 0);
  monthLayout4->setAlignment(Qt::AlignLeft);
  for (int i = 21; i != 28; ++i)
    monthLayout4->addWidget(m_vbtnMonth.at(i));

  QHBoxLayout* monthLayout5 = new QHBoxLayout;
  monthLayout5->setContentsMargins(0, 0, 0, 0);
  monthLayout5->setAlignment(Qt::AlignLeft);
  for (int i = 28; i != 31; ++i)
    monthLayout5->addWidget(m_vbtnMonth.at(i));

  QHBoxLayout* weekLayout1 = new QHBoxLayout;
  weekLayout1->setContentsMargins(0, 0, 0, 0);
  weekLayout1->setAlignment(Qt::AlignLeft);
  for (int i = 0; i != 7; ++i)
    weekLayout1->addWidget(m_vbtnWeek.at(i));

  QVBoxLayout* viewLayout = new QVBoxLayout;
  viewLayout->addWidget(m_edTitle);
  viewLayout->addWidget(m_edDescription);
  viewLayout->addWidget(m_supplierPicker);
  viewLayout->addWidget(new QLabel(tr("Detalhes:")));

  viewLayout->addWidget(m_cbCallSupplier);
  viewLayout->addWidget(m_cbSupplierCalls);
  viewLayout->addWidget(m_cbWhatsapp);
  viewLayout->addWidget(m_cbVisit);
  viewLayout->addWidget(m_cbPrintAmmount);
  viewLayout->addWidget(m_cbPrintPrice);

  QVBoxLayout* contactLayout = new QVBoxLayout;
  contactLayout->setAlignment(Qt::AlignTop);
  contactLayout->addWidget(new QLabel(tr("Como entrar em contato com o fornecedor:")));
  contactLayout->addWidget(m_cbCallSupplier);
  contactLayout->addWidget(m_cbSupplierCalls);
  contactLayout->addWidget(m_cbWhatsapp);
  contactLayout->addWidget(m_cbVisit);

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
  QVBoxLayout* listLayout = new QVBoxLayout;
  listLayout->addLayout(tableButtonLayout);
  listLayout->addWidget(m_table);

  QFrame* tabView = new QFrame;
  tabView->setLayout(viewLayout);

  QFrame* tabList = new QFrame;
  tabList->setLayout(listLayout);

  QFrame* tabContact = new QFrame;
  tabContact->setLayout(contactLayout);

  QFrame* tabCalendar = new QFrame;
  tabCalendar->setLayout(calendarLayout);

  QTabWidget* tabWidget = new QTabWidget;
  tabWidget->addTab(tabView,
                    QIcon(":/icons/res/details.png"),
                    tr("Informações"));
  tabWidget->addTab(tabList,
                    QIcon(":/icons/res/item.png"),
                    tr("Produtos"));
  tabWidget->addTab(tabContact,
                    QIcon(":/icons/res/phone.png"),
                    tr("Contato"));
  tabWidget->addTab(tabCalendar,
                    QIcon(":/icons/res/calendar.png"),
                    tr("Calendário"));

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(buttonLayout);
  mainLayout->addWidget(tabWidget);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(mainLayout);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(viewFrame);

  QVBoxLayout* frameLayout = new QVBoxLayout;
  frameLayout->setContentsMargins(0, 0, 0, 0);
  frameLayout->addWidget(splitter);
  setLayout(frameLayout);

  QObject::connect(m_btnAdd,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(addItem()));
  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeItem()));
  QObject::connect(m_btnEdit,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(editItem()));
  QObject::connect(m_table,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));
}

void ShoppingListView::setDatabase(QSqlDatabase db)
{
  m_supplierPicker->setDatabase(db, PERSON_SQL_TABLE_NAME);
  m_supplierPicker->getDatabase()->setCustomFilter(PERSON_FILTER_SUPPLIER);
}

void ShoppingListView::addItem()
{
  JDatabaseSelector w(tr("Produto"), QIcon(":/icons/res/item.png"), this);
  w.setDatabase(m_database->getDatabase(), PRODUCT_SQL_TABLE_NAME);
  if (w.exec())
  {
    ShoppingListItem* p = static_cast<ShoppingListItem*>(w.getDatabase()->getCurrentItem());
    if (p != nullptr)
      m_table->addShopItem(*p);
  }
}

void ShoppingListView::removeItem()
{
  m_table->removeCurrentItem();
}

void ShoppingListView::editItem()
{
  JDatabaseSelector w(tr("Produto"), QIcon(":/icons/res/item.png"), this);
  if (w.exec())
  {
    ShoppingListItem* p = static_cast<ShoppingListItem*>(w.getDatabase()->getCurrentItem());
    if (p != nullptr)
      m_table->setShopItem(*p);
  }
}

void ShoppingListView::updateControls()
{
  m_btnAdd->setEnabled(m_table->rowCount() < SHOPPING_LIST_MAX_NUMBER_OF_ITEMS);
  m_btnEdit->setEnabled(m_table->currentRow() != -1);
  m_btnRemove->setEnabled(m_table->currentRow() != -1);
}

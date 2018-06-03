#include "shoppinglistview.h"
#include <QPushButton>
#include <QCheckBox>
#include <QLayout>
#include "jdatabase.h"
#include "jdatabasepicker.h"
#include "shoppinglisttable.h"
#include "packageeditor.h"
#include "jlineedit.h"
#include <QSplitter>
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
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_database(nullptr)
  , m_edTitle(nullptr)
  , m_edDescription(nullptr)
  , m_supplierPicker(nullptr)
  , m_imagePicker(nullptr)
  , m_cbPrintAmmount(nullptr)
  , m_cbPrintPrice(nullptr)
  , m_cbSupplierCalls(nullptr)
  , m_cbCallSupplier(nullptr)
  , m_cbWhatsapp(nullptr)
  , m_cbVisit(nullptr)
  , m_table(nullptr)
  , m_tabWidget(nullptr)
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

  m_supplierPicker = new JDatabasePicker(tr("Fornecedor"), QIcon(":/icons/res/supplier.png"), true, true);
  m_imagePicker = new JDatabasePicker(tr("Imagem"), QIcon(":/icons/res/icon.png"), true, true);
  m_edTitle = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                            JLineEdit::st_defaultFlags1);
  m_edDescription = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                  JLineEdit::st_defaultFlags1);
  m_cbPrintAmmount = new QCheckBox;
  m_cbPrintAmmount->setText(tr("Imprimir quantidade recomendada"));
  m_cbPrintPrice = new QCheckBox;
  m_cbPrintPrice->setText(tr("Imprimir preço sugerido"));
  m_cbSupplierCalls = new QCheckBox;
  m_cbSupplierCalls->setText(tr("O fornecedor liga"));
  //m_cbSupplierCalls->setIcon(QIcon(":/icons/res/phone.png"));
  m_cbCallSupplier = new QCheckBox;
  m_cbCallSupplier->setText(tr("Ligar para o fornecedor"));
  //m_cbCallSupplier->setIcon(QIcon(":/icons/res/phoneback.png"));
  m_cbWhatsapp = new QCheckBox;
  m_cbWhatsapp->setText(tr("Whatsapp"));
  //m_cbWhatsapp->setIcon(QIcon(":/icons/res/whatsapp.png"));
  m_cbVisit = new QCheckBox;
  m_cbVisit->setText(tr("Visita presencial"));
  //m_cbVisit->setIcon(QIcon(":/icons/res/visit.png"));

  m_database = new JDatabase;

  for (int i = 0; i != 31; ++i)
    m_vbtnMonth[i] = monthButtonFactory(i);

  for (int i = 0; i != 7; ++i)
    m_vbtnWeek[i] = weekButtonFactory(i);

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
  viewFormLayout->addRow(tr("Descrição:"), m_edDescription);

  QVBoxLayout* viewLayout = new QVBoxLayout;
  viewLayout->setAlignment(Qt::AlignTop);
  viewLayout->addLayout(viewFormLayout);
  viewLayout->addWidget(m_supplierPicker);
  viewLayout->addWidget(m_imagePicker);
  viewLayout->addWidget(m_cbPrintAmmount);
  viewLayout->addWidget(m_cbPrintPrice);
  viewLayout->addWidget(new QLabel(tr("Contato:")));
  viewLayout->addWidget(m_cbCallSupplier);
  viewLayout->addWidget(m_cbSupplierCalls);
  viewLayout->addWidget(m_cbWhatsapp);
  viewLayout->addWidget(m_cbVisit);

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

  QFrame* tabCalendar = new QFrame;
  tabCalendar->setLayout(calendarLayout);

  m_tabWidget = new QTabWidget;
  m_tabWidget->addTab(tabView,
                      QIcon(":/icons/res/details.png"),
                      tr("Informações"));
  m_tabWidget->addTab(tabList,
                      QIcon(":/icons/res/item.png"),
                      tr("Produtos"));
  m_tabWidget->addTab(tabCalendar,
                      QIcon(":/icons/res/calendar.png"),
                      tr("Calendário"));

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(buttonLayout);
  mainLayout->addWidget(m_tabWidget);

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
  QObject::connect(m_table,
                   SIGNAL(productSignal(const Product&)),
                   this,
                   SLOT(editProduct()));
  QObject::connect(m_table,
                   SIGNAL(packageSignal(const Package&,
                                        const QString&)),
                   this,
                   SLOT(editPackage(const Package&, const QString&)));
  QObject::connect(m_table,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_database,
                   SIGNAL(itemRemovedSignal(qlonglong)),
                   this,
                   SLOT(itemRemoved(qlonglong)));
  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));
  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save()));
  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));

}

void ShoppingListView::setDatabase(QSqlDatabase db)
{
  m_database->setDatabase(db, SHOPPING_LIST_SQL_TABLE_NAME);
  m_supplierPicker->setDatabase(db, PERSON_SQL_TABLE_NAME);
  m_supplierPicker->getDatabase()->setCustomFilter(PERSON_FILTER_SUPPLIER);
}

void ShoppingListView::addItem()
{
  JDatabaseSelector w(tr("Produto"), QIcon(":/icons/res/item.png"), this);
  w.setDatabase(m_database->getDatabase(), PRODUCT_SQL_TABLE_NAME);
  w.getDatabase()->setCustomFilter(PRODUCT_FILTER_SHOP);
  if (w.exec())
  {
    Product* p = static_cast<Product*>(w.getDatabase()->getCurrentItem());
    if (p != nullptr && p->isValidId())
    {
      ShoppingListItem o;
      o.m_product = *p;
      m_table->addShoppingItem(o);
    }
  }
}

void ShoppingListView::removeItem()
{
  m_table->removeCurrentItem();
  updateControls();
}

void ShoppingListView::editProduct()
{
  JDatabaseSelector w(tr("Produto"), QIcon(":/icons/res/item.png"), this);
  w.setDatabase(m_database->getDatabase(), PRODUCT_SQL_TABLE_NAME);
  w.getDatabase()->setCustomFilter(PRODUCT_FILTER_SHOP);
  if (w.exec())
  {
    Product* p = static_cast<Product*>(w.getDatabase()->getCurrentItem());
    if (p != nullptr)
    {
      Product o = *p;
      m_table->setProduct(o);
    }
  }
}

void ShoppingListView::editPackage(const Package& package,
                                   const QString& productUnity)
{
  PackageEditor dlg(package, productUnity);
  if (dlg.exec())
    m_table->setPackage(dlg.getPackage());
}

void ShoppingListView::updateControls()
{
  m_btnAdd->setEnabled(m_table->rowCount() < SHOPPING_LIST_MAX_NUMBER_OF_ITEMS);
  m_btnRemove->setEnabled(m_table->currentRow() != -1);
}

void ShoppingListView::create()
{
  setShoppingList(ShoppingList());
  m_tabWidget->setCurrentIndex(0);
}

void ShoppingListView::itemSelected(const JItem& jItem)
{
  const ShoppingList& list = dynamic_cast<const ShoppingList&>(jItem);
  if (list.isValidId())
    setShoppingList(list);
}

void ShoppingListView::itemRemoved(qlonglong id)
{
  if (id == m_currentId)
    create();
}

void ShoppingListView::save()
{
  if (m_database->save(getShoppingList()))
    create();
}

void ShoppingListView::setShoppingList(const ShoppingList& lst)
{
  m_currentId = lst.m_id;
  m_edTitle->setText(lst.m_title);
  m_edDescription->setText(lst.m_description);
  m_supplierPicker->setItem(lst.m_supplier);
  m_imagePicker->setItem(lst.m_image);
  m_cbPrintAmmount->setChecked(lst.m_bPrintAmmount);
  m_cbPrintPrice->setChecked(lst.m_bPrintPrice);
  m_cbCallSupplier->setChecked(lst.m_bCallSupplier);
  m_cbSupplierCalls->setChecked(lst.m_bSupplierCalls);
  m_cbWhatsapp->setChecked(lst.m_bWhatsapp);
  m_cbVisit->setChecked(lst.m_bVisit);
  for (int i = 0; i != 7; ++i)
    m_vbtnWeek[i]->setChecked(lst.m_weekDays[i]);
  for (int i = 0; i != 31; ++i)
    m_vbtnMonth[i]->setChecked(lst.m_monthDays[i]);
  m_table->setShoppingItems(lst.m_vItem);
  updateControls();
}

ShoppingList ShoppingListView::getShoppingList() const
{
  ShoppingList lst;
  lst.m_id = m_currentId;
  lst.m_title = m_edTitle->text();
  lst.m_description = m_edDescription->text();
  lst.m_supplier.m_id = m_supplierPicker->getId();
  lst.m_image.m_id = m_imagePicker->getId();
  lst.m_bPrintAmmount = m_cbPrintAmmount->isChecked();
  lst.m_bPrintPrice = m_cbPrintPrice->isChecked();
  lst.m_bCallSupplier = m_cbCallSupplier->isChecked();
  lst.m_bSupplierCalls = m_cbSupplierCalls->isChecked();
  lst.m_bWhatsapp = m_cbWhatsapp->isChecked();
  lst.m_bVisit = m_cbVisit->isChecked();
  for (int i = 0; i != 7; ++i)
    lst.m_weekDays[i] = m_vbtnWeek[i]->isChecked();
  for (int i = 0; i != 31; ++i)
    lst.m_monthDays[i] = m_vbtnMonth[i]->isChecked();
  lst.m_vItem = m_table->getShoppingItems();
  return lst;
}

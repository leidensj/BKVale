#include "jdatabase.h"
#include "jlineedit.h"
#include "defines.h"
#include "databaseutils.h"
#include "notefilterdlg.h"
#include <QDate>
#include <QLayout>
#include <QSqlRecord>
#include <QPushButton>
#include <QHeaderView>
#include <QShortcut>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QPixmap>
#include <QIcon>
#include <QCheckBox>
#include <QSortFilterProxyModel>

class JTableModel : public QSqlQueryModel
{
public:
  JTableModel(QObject *parent)
    : QSqlQueryModel(parent)
  {

  }

  virtual QString getStrQuery()
  {
    return "";
  }

  virtual void select(const QString& query = "")
  {
    QSqlQueryModel::setQuery(query.isEmpty()
                             ? getStrQuery()
                             : query,
                             QSqlDatabase::database(SQLITE_CONNECTION_NAME));
  }

  virtual void select(QHeaderView* /*header*/)
  {
    select("");
  }

  virtual void selectFilter(const QString& /*filter*/)
  {
    select(getStrQuery());
  }
};

class NoteTableModel : public JTableModel
{
public:
  NoteTableModel(QObject *parent)
    : JTableModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     NOTE_SQL_TABLE_NAME "." SQL_COLID ","
                     NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL01 ","
                     NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL02 ","
                     PERSON_SQL_TABLE_NAME "." PERSON_SQL_COL03 ","
                     "(IFNULL(_TTOTAL._TSUBTOTAL,0) + " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL06 ")"
                     " FROM " NOTE_SQL_TABLE_NAME
                     " LEFT JOIN "
                     "(SELECT " NOTE_ITEMS_SQL_COL01 ","
                     "IFNULL(SUM(" NOTE_ITEMS_SQL_COL03 "*" NOTE_ITEMS_SQL_COL04 "), 0) AS _TSUBTOTAL"
                     " FROM " NOTE_ITEMS_SQL_TABLE_NAME
                     " GROUP BY " NOTE_ITEMS_SQL_COL01 ") AS _TTOTAL"
                     " ON " NOTE_SQL_TABLE_NAME "." SQL_COLID "= _TTOTAL." NOTE_ITEMS_SQL_COL01
                     " LEFT JOIN "
                     PERSON_SQL_TABLE_NAME
                     " ON " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL03 "=" PERSON_SQL_TABLE_NAME "." SQL_COLID
                     " GROUP BY " NOTE_SQL_TABLE_NAME "." SQL_COLID);
    return strQuery;
  }

  void select(QHeaderView* header)
  {
    JTableModel::select(getStrQuery());
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Número"));
    setHeaderData(2, Qt::Horizontal, tr("Data"));
    setHeaderData(3, Qt::Horizontal, tr("Fornecedor"));
    setHeaderData(4, Qt::Horizontal, tr("Total"));
    if (header != nullptr)
    {
      if (header->count() == 5)
      {
        header->hideSection(0);
        header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
        header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
        header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
        header->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
      }
    }
  }

  void selectFilter(const QString& filter)
  {
    JTableModel::select(getStrQuery() + " HAVING " + filter);
  }

  QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const
  {
    QVariant value = QSqlQueryModel::data(idx, role);
    if (role == Qt::DisplayRole)
    {
      if (idx.column() == 2)
        value = QDate::fromString(value.toString(), Qt::ISODate).toString("yyyy/MM/dd");
      else if (idx.column() == 4)
        value = "R$ " + QString::number(value.toDouble(), 'f', 2);
    }
    return value;
  }
};

class ReminderTableModel : public JTableModel
{
public:
  ReminderTableModel(QObject *parent)
    : JTableModel(parent)
  {

  }

  QString getStrQuery()
  {
    return
        "SELECT "
        SQL_COLID ","
        REMINDER_SQL_COL01 ","
        REMINDER_SQL_COL02 ","
        REMINDER_SQL_COL03
        " FROM "
        REMINDER_SQL_TABLE_NAME;
  }

  virtual void select(QHeaderView* header)
  {
    JTableModel::select(getStrQuery());
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Título"));
    setHeaderData(2, Qt::Horizontal, tr("Mensagem"));
    setHeaderData(3, Qt::Horizontal, tr("Favorito"));
    if (header != nullptr && header->count() == 4)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
    }
  }

  virtual void selectFilter(const QString& filter)
  {
    JTableModel::select(getStrQuery() + " WHERE " + filter);
  }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
  {
    if (!index.isValid())
      return QModelIndex();

    QVariant value = QSqlQueryModel::data(index, role);
    if (role == Qt::DecorationRole)
    {
      if (index.column() == 3)
      {
        if (QSqlQueryModel::data(index, Qt::EditRole).toBool())
          value = QVariant::fromValue(QIcon(":/icons/res/favorite.png"));
        else
          value = "";
      }
    }
    else if (role == Qt::DisplayRole)
    {
      if (index.column() == 3)
        value = value.toBool() ? tr("Sim") : "";
      else if (index.column() == 1)
      {
        auto cap = (Reminder::Capitalization)record(index.row()).value(4).toInt();
        switch (cap)
        {
          case Reminder::Capitalization::AllUppercase:
            value = value.toString().toUpper();
            break;
          case Reminder::Capitalization::AllLowercase:
            value = value.toString().toLower();
            break;
          case Reminder::Capitalization::Normal:
          default:
            break;
        }
      }
    }
    return value;
  }
};

class PersonTableModel : public JTableModel
{
public:
  PersonTableModel(QObject *parent)
    : JTableModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     PERSON_SQL_COL02 ","
                     PERSON_SQL_COL03
                     " FROM "
                     PERSON_SQL_TABLE_NAME);
    return strQuery;
  }

  virtual void select(QHeaderView* header)
  {
    JTableModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Nome/Razão Social"));
    setHeaderData(2, Qt::Horizontal, tr("Apelido/Nome Fantasia"));
    if (header != nullptr && header->count() == 3)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    }
  }

  virtual void selectFilter(const QString& filter)
  {
    JTableModel::select(getStrQuery() + " WHERE " + filter);
  }
};

class CategoryTableModel : public JTableModel
{
public:
  CategoryTableModel(QObject *parent)
    : JTableModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     CATEGORY_SQL_COL02
                     " FROM "
                     CATEGORY_SQL_TABLE_NAME);
    return strQuery;
  }

  virtual void select(QHeaderView* header)
  {
    JTableModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Nome"));
    if (header != nullptr && header->count() == 2)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    }
  }

  virtual void selectFilter(const QString& filter)
  {
    JTableModel::select(getStrQuery() + " WHERE " + filter);
  }
};

class ProductTableModel : public JTableModel
{
public:
  ProductTableModel(QObject *parent)
    : JTableModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     PRODUCT_SQL_TABLE_NAME "." SQL_COLID ","
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL01 ","
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL04 ","
                     CATEGORY_SQL_TABLE_NAME "." CATEGORY_SQL_COL02
                     " FROM "
                     PRODUCT_SQL_TABLE_NAME
                     " LEFT JOIN "
                     CATEGORY_SQL_TABLE_NAME
                     " ON "
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL02
                     " = "
                     CATEGORY_SQL_TABLE_NAME "." SQL_COLID
                     " GROUP BY " PRODUCT_SQL_TABLE_NAME "." SQL_COLID);
    return strQuery;
  }

  virtual void select(QHeaderView* header)
  {
    JTableModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Nome"));
    setHeaderData(2, Qt::Horizontal, tr("Unidade"));
    setHeaderData(3, Qt::Horizontal, tr("Categoria"));
    if (header != nullptr && header->count() == 4)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
    }
  }

  virtual void selectFilter(const QString& filter)
  {
    JTableModel::select(getStrQuery() + " HAVING " + filter);
  }
};

class UserTableModel : public JTableModel
{
public:
  UserTableModel(QObject *parent)
    : JTableModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     USER_SQL_COL01
                     " FROM "
                     USER_SQL_TABLE_NAME);
    return strQuery;
  }

  virtual void select(QHeaderView* header)
  {
    JTableModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Nome"));
    if (header != nullptr && header->count() == 2)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    }
  }

  virtual void selectFilter(const QString& filter)
  {
    JTableModel::select(getStrQuery() + " WHERE " + filter);
  }
};

class ImageTableModel : public JTableModel
{
public:
  ImageTableModel(QObject *parent)
    : JTableModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     IMAGE_SQL_COL01 ","
                     IMAGE_SQL_COL02
                     " FROM "
                     IMAGE_SQL_TABLE_NAME);
    return strQuery;
  }

  virtual void select(QHeaderView* header)
  {
    JTableModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Imagem"));
    setHeaderData(2, Qt::Horizontal, tr("Imagem"));
    if (header != nullptr && header->count() == 3)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
      header->hideSection(2);
    }
  }

  virtual void selectFilter(const QString& filter)
  {
    JTableModel::select(getStrQuery() + " WHERE " + filter);
  }

  QVariant data(const QModelIndex &idx, int role) const
  {
    QVariant value = QSqlQueryModel::data(idx, role);
    if (role == Qt::DecorationRole)
    {
      if (idx.column() == 1)
      {
        QPixmap px(QSize(16, 16));
        px.loadFromData(QSqlQueryModel::data(createIndex(idx.row(), 2),
                                             Qt::EditRole).toByteArray());
        value = QVariant::fromValue(QIcon(px));
      }
    }
    return value;
  }
};

class ShoppingListTableModel : public JTableModel
{
public:
  ShoppingListTableModel(QObject *parent)
    : JTableModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SHOPPING_LIST_SQL_TABLE_NAME "." SQL_COLID ","
                     SHOPPING_LIST_SQL_TABLE_NAME "." SHOPPING_LIST_SQL_COL03 ","
                     SHOPPING_LIST_SQL_TABLE_NAME "." SHOPPING_LIST_SQL_COL04
                     " FROM "
                     SHOPPING_LIST_SQL_TABLE_NAME);
    return strQuery;
  }

  virtual void select(QHeaderView* header)
  {
    JTableModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Título"));
    setHeaderData(2, Qt::Horizontal, tr("Descrição"));
    if (header != nullptr && header->count() == 3)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    }
  }

  virtual void selectFilter(const QString& filter)
  {
    JTableModel::select(getStrQuery() + " WHERE " + filter);
  }
};

class ReservationTableModel : public JTableModel
{
public:
  ReservationTableModel(QObject *parent)
    : JTableModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     RESERVATION_SQL_COL01 ","
                     RESERVATION_SQL_COL04 ","
                     RESERVATION_SQL_COL02 ","
                     RESERVATION_SQL_COL03
                     " FROM "
                     RESERVATION_SQL_TABLE_NAME);
    return strQuery;
  }

  virtual void select(QHeaderView* header)
  {
    JTableModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Número"));
    setHeaderData(2, Qt::Horizontal, tr("Data"));
    setHeaderData(3, Qt::Horizontal, tr("Nome"));
    setHeaderData(4, Qt::Horizontal, tr("Local"));
    if (header != nullptr && header->count() == 5)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(4, QHeaderView::ResizeMode::Stretch);
    }
  }

  virtual void selectFilter(const QString& filter)
  {
    JTableModel::select(getStrQuery() + " WHERE " + filter);
  }

  QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const
  {
    if (!idx.isValid())
      return QModelIndex();

    QVariant value = QSqlQueryModel::data(idx, role);
    if (role == Qt::DisplayRole)
    {
      if (idx.column() == 2)
      {
        QDateTime dt = QDateTime::fromString(value.toString(), Qt::ISODate);
        value = dt.toString("yyy//MM/dd HH:mm");
      }
    }
    return value;
  }
};


JTableView::JTableView(QWidget *parent)
  : QTableView(parent)
{

}

void JTableView::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Enter ||
      event->key() == Qt::Key_Return)
  {
    emit enterKeyPressedSignal();
  }
  QTableView::keyPressEvent(event);
}

JDatabase::JDatabase(const QString& tableName,
                     bool bSelectorMode,
                     QWidget *parent)
  : QFrame(parent)
  , m_bSelectorMode(bSelectorMode)
  , m_btnOpen(nullptr)
  , m_btnRefresh(nullptr)
  , m_btnRemove(nullptr)
  , m_btnFilter(nullptr)
  , m_btnFilterClear(nullptr)
  , m_edFilterSearch(nullptr)
  , m_cbContains(nullptr)
  , m_table(nullptr)
  , m_currentItem(nullptr)
  , m_proxyModel(nullptr)
  , m_noteFilter(nullptr)
{
  m_btnOpen = new QPushButton();
  m_btnOpen->setFlat(true);
  m_btnOpen->setText("");
  m_btnOpen->setIconSize(QSize(24, 24));
  m_btnOpen->setIcon(QIcon(":/icons/res/open.png"));
  m_btnOpen->setToolTip(tr("Abrir (Ctrl+O)"));
  m_btnOpen->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));

  m_btnRefresh = new QPushButton();
  m_btnRefresh->setFlat(true);
  m_btnRefresh->setText("");
  m_btnRefresh->setIconSize(QSize(24, 24));
  m_btnRefresh->setIcon(QIcon(":/icons/res/refresh.png"));
  m_btnRefresh->setToolTip(tr("Atualizar (F5)"));
  m_btnRefresh->setShortcut(QKeySequence(Qt::Key_F5));

  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setToolTip(tr("Remover (Del)"));
  m_btnRemove->setIcon(QIcon(":/icons/res/remove.png"));

  m_btnFilter = new QPushButton();
  m_btnFilter->setFlat(true);
  m_btnFilter->setText("");
  m_btnFilter->setIconSize(QSize(24, 24));
  m_btnFilter->setToolTip(tr("Filtro"));
  m_btnFilter->setIcon(QIcon(":/icons/res/filter.png"));

  m_btnFilterClear = new QPushButton();
  m_btnFilterClear->setFlat(true);
  m_btnFilterClear->setText("");
  m_btnFilterClear->setIconSize(QSize(24, 24));
  m_btnFilterClear->setToolTip(tr("Limpar filtro"));
  m_btnFilterClear->setIcon(QIcon(":/icons/res/filterclear.png"));

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->setAlignment(Qt::AlignLeft);
  hlayout0->addWidget(m_btnOpen);
  hlayout0->addWidget(m_btnRefresh);
  hlayout0->addWidget(m_btnRemove);
  hlayout0->addWidget(m_btnFilter);
  hlayout0->addWidget(m_btnFilterClear);

  m_edFilterSearch = new JLineEdit(JLineEdit::Input::All,
                                   (int)JLineEdit::Flags::ToUpper);
  m_edFilterSearch->setToolTip(tr("Procurar (Ctrl+F)"));
  m_edFilterSearch->setClearButtonEnabled(true);

  m_cbContains = new QCheckBox;
  m_cbContains->setText(tr("Contendo"));
  m_cbContains->setIconSize(QSize(16, 16));
  m_cbContains->setIcon(QIcon(":/icons/res/center.png"));
  m_cbContains->setToolTip(tr("Procurar por palavras contendo o termo informado"));

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->addWidget(m_edFilterSearch);
  hlayout1->addWidget(m_cbContains);

  m_table = new JTableView();
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setEditTriggers(QTableView::NoEditTriggers);
  m_table->setSortingEnabled(true);
  m_table->horizontalHeader()->setHighlightSections(false);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->addLayout(hlayout0);
  vlayout1->addLayout(hlayout1);
  vlayout1->addWidget(m_table);
  setLayout(vlayout1);

  JTableModel* model = nullptr;
  if (tableName == IMAGE_SQL_TABLE_NAME)
    model = new ImageTableModel(this);
  else if (tableName == NOTE_SQL_TABLE_NAME)
    model = new NoteTableModel(this);
  else if (tableName == REMINDER_SQL_TABLE_NAME)
    model = new ReminderTableModel(this);
  else if (tableName == PERSON_SQL_TABLE_NAME)
    model = new PersonTableModel(this);
  else if (tableName == USER_SQL_TABLE_NAME)
    model = new UserTableModel(this);
  else if (tableName == PRODUCT_SQL_TABLE_NAME)
    model = new ProductTableModel(this);
  else if (tableName == CATEGORY_SQL_TABLE_NAME)
    model = new CategoryTableModel(this);
  else if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    model = new ShoppingListTableModel(this);
  else if (tableName == RESERVATION_SQL_TABLE_NAME)
    model = new ReservationTableModel(this);
  else
    model = new JTableModel(this);

  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setSourceModel(model);

  m_tableName = tableName;
  m_table->setModel(m_proxyModel);
  model->select(m_table->horizontalHeader());

  QObject::connect(m_table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));

  QObject::connect(m_table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(emitCurrentRowChangedSignal()));
  QObject::connect(m_table,
                   SIGNAL(doubleClicked(const QModelIndex&)),
                   this,
                   SLOT(selectItem()));
  QObject::connect(m_btnOpen,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(selectItem()));
  QObject::connect(m_btnRefresh,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(refresh()));
  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeItem()));
  QObject::connect(m_edFilterSearch,
                   SIGNAL(textEdited(const QString&)),
                   this,
                   SLOT(filterSearchChanged()));
  QObject::connect(m_edFilterSearch,
                   SIGNAL(enterSignal()),
                   this,
                   SLOT(filterSearchEnter()));
  QObject::connect(m_edFilterSearch,
                   SIGNAL(keyDownSignal()),
                   this,
                   SLOT(filterSearchEnter()));
  QObject::connect(m_cbContains,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(containsPressed()));
  QObject::connect(m_table->horizontalHeader(),
                   SIGNAL(sortIndicatorChanged(int,
                                               Qt::SortOrder)),
                   this,
                   SLOT(filterSearchChanged()));
  QObject::connect(m_table,
                   SIGNAL(enterKeyPressedSignal()),
                   this,
                   SLOT(selectItem()));
  QObject::connect(m_btnFilter,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(showFilter()));
  QObject::connect(m_btnFilterClear,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clearFilter()));

  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F),
                this,
                SLOT(focusFilterSearch()));

  if (m_table->horizontalHeader()->count() > 1)
    m_table->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);

  if (m_bSelectorMode)
  {
    m_btnOpen->hide();
    m_btnRefresh->hide();
    m_btnFilter->hide();
    m_btnRemove->setEnabled(false);
    m_btnRemove->hide();
    m_edFilterSearch->setFocus();
  }

  if (m_tableName != NOTE_SQL_TABLE_NAME)
  {
    m_btnFilter->hide();
    m_btnFilterClear->hide();
  }

  filterSearchChanged();
}

JDatabase::~JDatabase()
{
  if (m_currentItem != nullptr)
    delete m_currentItem;
  m_currentItem = nullptr;
}

void JDatabase::selectItem()
{
  QModelIndex idx = m_proxyModel->mapToSource(m_table->currentIndex());
  if (idx.isValid())
  {
    JTableModel* model = dynamic_cast<JTableModel*>(m_proxyModel->sourceModel());
    qlonglong id = model->record(idx.row()).value(SQL_COLID_NUMBER).toLongLong();
    selectItem(id);
  }
  else
  {
    if (m_currentItem != nullptr)
      delete m_currentItem;
    m_currentItem = nullptr;
  }
}

void JDatabase::selectItem(qlonglong id)
{
  if (m_currentItem != nullptr)
    delete m_currentItem;
  m_currentItem = nullptr;

  bool bSuccess = false;
  QString error;
  if (m_tableName == IMAGE_SQL_TABLE_NAME)
  {
    Image o;
    o.m_id = id;
    bSuccess = ImageSQL::select(o, error);
    m_currentItem = new Image(o);
  }
  else if (m_tableName == PERSON_SQL_TABLE_NAME)
  {
    Person o;
    o.m_id = id;
    bSuccess = PersonSQL::select(o, error);
    m_currentItem = new Person(o);
  }
  else if (m_tableName == CATEGORY_SQL_TABLE_NAME)
  {
    Category o;
    o.m_id = id;
    bSuccess = CategorySQL::select(o, error);
    m_currentItem = new Category(o);
  }
  else if (m_tableName == PRODUCT_SQL_TABLE_NAME)
  {
    Product o;
    o.m_id = id;
    bSuccess = ProductSQL::select(o, error);
    m_currentItem = new Product(o);
  }
  else if (m_tableName == NOTE_SQL_TABLE_NAME)
  {
    Note o;
    o.m_id = id;
    bSuccess = NoteSQL::select(o, error);
    m_currentItem = new Note(o);
  }
  else if (m_tableName == USER_SQL_TABLE_NAME)
  {
    User o;
    o.m_id = id;
    bSuccess = UserSQL::select(o, error);
    m_currentItem = new User(o);
  }
  else if (m_tableName == REMINDER_SQL_TABLE_NAME)
  {
    Reminder o;
    o.m_id = id;
    bSuccess = ReminderSQL::select(o, error);
    m_currentItem = new Reminder(o);
  }
  else if (m_tableName == SHOPPING_LIST_SQL_TABLE_NAME)
  {
    ShoppingList o;
    o.m_id = id;
    bSuccess = ShoppingListSQL::select(o, error);
    m_currentItem = new ShoppingList(o);
  }
  else if (m_tableName == RESERVATION_SQL_TABLE_NAME)
  {
    Reservation o;
    o.m_id = id;
    bSuccess = ReservationSQL::select(o, error);
    m_currentItem = new Reservation(o);
  }
  else
  {
    error = tr("Item ainda não implementado.");
  }

  if (bSuccess && m_currentItem != nullptr)
  {
    const JItem& jItem = *m_currentItem;
    emit itemSelectedSignal(jItem);
  }
  if (!bSuccess)
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("O seguinte erro ocorreu ao selecionar o item com ID "
                             "'%1':\n'%2'").arg(QString::number(id), error));
  }
}

void JDatabase::refresh()
{
  JTableModel* model = dynamic_cast<JTableModel*>(m_proxyModel->sourceModel());
  if (m_fixedFilter.isEmpty() && m_filter.isEmpty())
    model->select();
  else
  {
    QString str;
    if (!m_fixedFilter.isEmpty())
    {
      if (!m_filter.isEmpty())
        str = m_fixedFilter + " AND " + m_filter;
      else
        str = m_fixedFilter;
    }
    else
      str = m_filter;
    model->selectFilter(str);
  }

  m_proxyModel->invalidate();
  enableControls();
  if (m_bSelectorMode)
    m_edFilterSearch->setFocus();
}

void JDatabase::enableControls()
{
  bool bSelected = m_table->currentIndex().isValid();
  m_btnOpen->setEnabled(bSelected);
  m_btnRemove->setEnabled(bSelected);
}

void JDatabase::removeItem()
{
  QModelIndex idx = m_proxyModel->mapToSource(m_table->currentIndex());
  if (idx.isValid())
  {
    if (QMessageBox::question(this,
                              tr("Remover item"),
                              tr("Tem certeza que deseja remover o item selecionado?"),
                              QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok)
    {
      return;
    }
    JTableModel* model = dynamic_cast<JTableModel*>(m_proxyModel->sourceModel());
    qlonglong id = model->index(idx.row(), SQL_COLID_NUMBER).data(Qt::EditRole).toLongLong();
    bool bSuccess = false;
    QString error;
    if (m_tableName == IMAGE_SQL_TABLE_NAME)
      bSuccess = ImageSQL::remove(id, error);
    else if (m_tableName == PERSON_SQL_TABLE_NAME)
      bSuccess = PersonSQL::remove(id, error);
    else if (m_tableName == CATEGORY_SQL_TABLE_NAME)
      bSuccess = CategorySQL::remove(id, error);
    else if (m_tableName == PRODUCT_SQL_TABLE_NAME)
      bSuccess = ProductSQL::remove(id, error);
    else if (m_tableName == NOTE_SQL_TABLE_NAME)
      bSuccess = NoteSQL::remove(id, error);
    else if (m_tableName == USER_SQL_TABLE_NAME)
      bSuccess = UserSQL::remove(id, error);
    else if (m_tableName == REMINDER_SQL_TABLE_NAME)
      bSuccess = ReminderSQL::remove(id, error);
    else if (m_tableName == SHOPPING_LIST_SQL_TABLE_NAME)
      bSuccess = ShoppingListSQL::remove(id, error);
    else if (m_tableName == RESERVATION_SQL_TABLE_NAME)
      bSuccess = ReservationSQL::remove(id, error);
    else
      error = tr("Item ainda não implementado.");

    if (bSuccess)
    {
      emit itemRemovedSignal(id);
      m_proxyModel->invalidate();
    }
    else
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("O seguinte erro ocorreu ao remover o item com ID "
                               "'%1':\n'%2'").arg(QString::number(id), error));
    }
  }
}

void JDatabase::filterSearchChanged()
{
  int column = m_table->horizontalHeader()->sortIndicatorSection();
  m_proxyModel->setFilterKeyColumn(column);
  QString filter = m_edFilterSearch->text();
  if (!m_cbContains->isChecked() && !filter.isEmpty())
    filter.prepend("\\b");
  QRegExp regExp(filter, Qt::CaseInsensitive, QRegExp::RegExp);
  m_proxyModel->setFilterRegExp(regExp);

  if (filter.isEmpty())
  {
    QString columnName;
    if (column > 0)
      columnName = m_proxyModel->headerData(column, Qt::Horizontal).toString().toLower();
    columnName = tr("Procurar por: ") + columnName;
    m_edFilterSearch->setPlaceholderText(columnName);
  }

  enableControls();
}

void JDatabase::filterSearchEnter()
{
  m_table->setFocus();
  if (m_proxyModel->rowCount() != 0)
  {
    m_table->selectRow(0);
  }
  else
  {
    m_edFilterSearch->setFocus();
    m_edFilterSearch->selectAll();
  }
}

void JDatabase::clearFilterSearch()
{
  m_edFilterSearch->clear();
  filterSearchChanged();
}

void JDatabase::containsPressed()
{
  filterSearchChanged();
  m_edFilterSearch->setFocus();
}

void JDatabase::focusFilterSearch()
{
  m_edFilterSearch->selectAll();
  m_edFilterSearch->setFocus();
}

void JDatabase::setFixedFilter(const QString& fixedFilter)
{
  m_fixedFilter = fixedFilter;
  refresh();
}

QString JDatabase::getTableName() const
{
  return m_tableName;
}

JItem* JDatabase::getCurrentItem() const
{
  return m_currentItem;
}

bool JDatabase::save(const JItem& jItem)
{
  QString error;
  bool bSuccess = false;
  if (m_tableName == IMAGE_SQL_TABLE_NAME)
  {
    const Image& o = dynamic_cast<const Image&>(jItem);
    bSuccess = o.isValidId()
               ? ImageSQL::update(o, error)
               : ImageSQL::insert(o, error);
  }
  else if (m_tableName == PERSON_SQL_TABLE_NAME)
  {
    const Person& o = dynamic_cast<const Person&>(jItem);
    bSuccess = o.isValidId()
               ? PersonSQL::update(o, error)
               : PersonSQL::insert(o, error);
  }
  else if (m_tableName == CATEGORY_SQL_TABLE_NAME)
  {
    const Category& o = dynamic_cast<const Category&>(jItem);
    bSuccess = o.isValidId()
               ? CategorySQL::update(o, error)
               : CategorySQL::insert(o, error);
  }
  else if (m_tableName == PRODUCT_SQL_TABLE_NAME)
  {
    const Product& o = dynamic_cast<const Product&>(jItem);
    bSuccess = o.isValidId()
               ? ProductSQL::update(o, error)
               : ProductSQL::insert(o, error);
  }
  else if (m_tableName == NOTE_SQL_TABLE_NAME)
  {
    const Note& o = dynamic_cast<const Note&>(jItem);
    bSuccess = o.isValidId()
               ? NoteSQL::update(o, error)
               : NoteSQL::insert(o, error);
  }
  else if (m_tableName == USER_SQL_TABLE_NAME)
  {
    const User& o = dynamic_cast<const User&>(jItem);
    bSuccess = o.isValidId()
               ? UserSQL::update(o, error)
               : UserSQL::insert(o, error);
  }
  else if (m_tableName == REMINDER_SQL_TABLE_NAME)
  {
    const Reminder& o = dynamic_cast<const Reminder&>(jItem);
    bSuccess = o.isValidId()
               ? ReminderSQL::update(o, error)
               : ReminderSQL::insert(o, error);
  }
  else if (m_tableName == SHOPPING_LIST_SQL_TABLE_NAME)
  {
    const ShoppingList& o = dynamic_cast<const ShoppingList&>(jItem);
    bSuccess = o.isValidId()
               ? ShoppingListSQL::update(o, error)
               : ShoppingListSQL::insert(o, error);
  }
  else if (m_tableName == RESERVATION_SQL_TABLE_NAME)
  {
    const Reservation& o = dynamic_cast<const Reservation&>(jItem);
    bSuccess = o.isValidId()
               ? ReservationSQL::update(o, error)
               : ReservationSQL::insert(o, error);
  }
  else
    error = tr("Item ainda não implementado.");

  if (bSuccess)
    refresh();
  else
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao salvar o item.").arg(error),
                          QMessageBox::Ok);
  }
  return bSuccess;
}

void JDatabase::emitCurrentRowChangedSignal()
{
  emit currentRowChangedSignal(m_table->currentIndex().row());
}

void JDatabase::showFilter()
{
  if (m_tableName == NOTE_SQL_TABLE_NAME)
  {
    if (m_noteFilter == nullptr)
      m_noteFilter = new NoteFilterDlg(this);
    m_noteFilter->exec();
    m_filter = m_noteFilter->getFilter();
    refresh();
  }
}

void JDatabase::clearFilter()
{
  if (m_tableName == NOTE_SQL_TABLE_NAME)
  {
    m_noteFilter->clearFilter();
    m_filter = "";
    refresh();
  }
}

JDatabaseSelector::JDatabaseSelector(const QString& tableName,
                                     const QString& title,
                                     const QIcon& icon,
                                     QWidget* parent)
  : QDialog(parent)
  , m_database(nullptr)
{
  m_database = new JDatabase(tableName, true);
  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->addWidget(m_database);
  setLayout(hlayout0);

  resize(500, 400);
  setWindowTitle(title);
  setWindowIcon(icon);

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));
}

void JDatabaseSelector::itemSelected(const JItem& /*jItem*/)
{
  accept();
}

JDatabase* JDatabaseSelector::getDatabase() const
{
  return m_database;
}

void JDatabaseSelector::closeEvent(QCloseEvent * e)
{
  m_database->clearFilterSearch();
  m_database->refresh();
  QDialog::closeEvent(e);
}

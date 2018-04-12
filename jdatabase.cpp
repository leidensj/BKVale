#include "jdatabase.h"
#include "jlineedit.h"
#include "defines.h"
#include "databaseutils.h"
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
#include <QSqlQuery>

class JTableModel : public QSqlQueryModel
{
public:
  JTableModel(QObject *parent, QSqlDatabase db)
    : QSqlQueryModel(parent)
    , m_db(db)
  {

  }

  virtual QString getStrQuery() = 0;
  virtual void setQuery() = 0;
  virtual void setFilter(const QString& value, bool bContaining, int column) = 0;

protected:
  QSqlDatabase m_db;
};

class NoteTableModel : public JTableModel
{
public:
  NoteTableModel(QObject *parent, QSqlDatabase db)
    : JTableModel(parent, db)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL01 ","
                     NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL02 ","
                     NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL03 ","
                     "SUM("
                     NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL03
                     "*"
                     NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL04 ")"
                     " FROM "
                     NOTE_ITEMS_SQL_TABLE_NAME
                     " INNER JOIN "
                     NOTE_SQL_TABLE_NAME
                     " ON "
                     NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL00
                     "="
                     NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL01
                     " GROUP BY "
                     NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL01);
    return strQuery;
  }

  void setQuery()
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
    setHeaderData(0, Qt::Horizontal, tr("Número"));
    setHeaderData(1, Qt::Horizontal, tr("Data"));
    setHeaderData(2, Qt::Horizontal, tr("Fornecedor"));
    setHeaderData(3, Qt::Horizontal, tr("Total"));
  }

  void setFilter(const QString& /*filter*/, bool /*bContaining*/, int /*column*/)
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }

  QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const
  {
    QVariant value = QSqlQueryModel::data(idx, role);
    if (role == Qt::DisplayRole)
    {
      if (idx.column() == 1)
        value = QDate::fromString(value.toString(), Qt::ISODate).toString("dd/MM/yyyy");
      else if (idx.column() == 3)
        value = "R$ " + QString::number(value.toDouble(), 'f', 2);
    }
    return value;
  }
};

class ReminderTableModel : public JTableModel
{
public:
  ReminderTableModel(QObject *parent, QSqlDatabase db)
    : JTableModel(parent, db)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     REMINDER_SQL_COL01 ","
                     REMINDER_SQL_COL02 ","
                     REMINDER_SQL_COL03
                     " FROM "
                     REMINDER_SQL_TABLE_NAME);
    return strQuery;
  }

  void setQuery()
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }

  void setFilter(const QString& /*filter*/, bool /*bContaining*/, int /*column*/)
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
  {
    if (!index.isValid())
      return QModelIndex();

    QVariant value = QSqlQueryModel::data(index, role);
    if (role == Qt::DecorationRole)
    {
      if (index.column() == 2)
      {
        if (QSqlQueryModel::data(index, Qt::EditRole).toBool())
          value = QVariant::fromValue(QIcon(":/icons/res/favorite.png"));
        else
          value = "";
      }
    }
    else if (role == Qt::DisplayRole)
    {
      if (index.column() == 2)
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
  PersonTableModel(QObject *parent, QSqlDatabase db)
    : JTableModel(parent, db)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     PERSON_SQL_COL02 ","
                     PERSON_SQL_COL03
                     " FROM "
                     PERSON_SQL_TABLE_NAME);
    return strQuery;
  }

  void setQuery()
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }

  void setFilter(const QString& /*filter*/, bool /*bContaining*/, int /*column*/)
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }
};

class CategoryTableModel : public JTableModel
{
public:
  CategoryTableModel(QObject *parent, QSqlDatabase db)
    : JTableModel(parent, db)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     CATEGORY_SQL_COL02
                     " FROM "
                     CATEGORY_SQL_TABLE_NAME);
    return strQuery;
  }

  void setQuery()
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }

  void setFilter(const QString& /*filter*/, bool /*bContaining*/, int /*column*/)
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }
};

class ProductTableModel : public JTableModel
{
public:
  ProductTableModel(QObject *parent, QSqlDatabase db)
    : JTableModel(parent, db)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     PRODUCT_SQL_COL01 ","
                     PRODUCT_SQL_COL04
                     " FROM "
                     PRODUCT_SQL_TABLE_NAME);
    return strQuery;
  }

  void setQuery()
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }

  void setFilter(const QString& /*filter*/, bool /*bContaining*/, int /*column*/)
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }
};

class UserTableModel : public JTableModel
{
public:
  UserTableModel(QObject *parent, QSqlDatabase db)
    : JTableModel(parent, db)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     USER_SQL_COL01
                     " FROM "
                     USER_SQL_TABLE_NAME);
    return strQuery;
  }

  void setQuery()
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }

  void setFilter(const QString& /*filter*/, bool /*bContaining*/, int /*column*/)
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }
};

class ImageTableModel : public JTableModel
{
public:
  ImageTableModel(QObject *parent, QSqlDatabase db)
    : JTableModel(parent, db)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     IMAGE_SQL_COL01
                     " FROM "
                     IMAGE_SQL_TABLE_NAME);
    return strQuery;
  }

  void setQuery()
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }

  void setFilter(const QString& /*filter*/, bool /*bContaining*/, int /*column*/)
  {
    QSqlQueryModel::setQuery(getStrQuery(), m_db);
  }

  QVariant data(const QModelIndex &idx, int role) const
  {
    QVariant value = QSqlQueryModel::data(idx, role);
    if (role == Qt::DecorationRole)
    {
      if (idx.column() == 0)
      {
        //TODO
        /*QPixmap px(QSize(16, 16));
        px.loadFromData(QSqlTableModel::data(createIndex(idx.row(), (int)Image::Column::Image),
                                             Qt::EditRole).toByteArray());
        value = QVariant::fromValue(QIcon(px));*/
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

JDatabase::JDatabase(bool bSelectorMode,
                     QWidget *parent)
  : QFrame(parent)
  , m_bSelectorMode(bSelectorMode)
  , m_btnOpen(nullptr)
  , m_btnRefresh(nullptr)
  , m_btnRemove(nullptr)
  , m_btnFilter(nullptr)
  , m_edFilterSearch(nullptr)
  , m_btnContains(nullptr)
  , m_table(nullptr)
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

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->setAlignment(Qt::AlignLeft);
  hlayout0->addWidget(m_btnOpen);
  hlayout0->addWidget(m_btnRefresh);
  hlayout0->addWidget(m_btnRemove);
  hlayout0->addWidget(m_btnFilter);

  m_edFilterSearch = new JLineEdit(JValidatorType::All, true, false);
  m_edFilterSearch->setToolTip(tr("Procurar (Ctrl+F)"));
  m_edFilterSearch->setClearButtonEnabled(true);

  m_btnContains = new QPushButton();
  m_btnContains->setFlat(true);
  m_btnContains->setText("");
  m_btnContains->setIconSize(QSize(24, 24));
  m_btnContains->setIcon(QIcon(":/icons/res/center.png"));
  m_btnContains->setToolTip(tr("Contendo"));
  m_btnContains->setCheckable(true);

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->addWidget(m_edFilterSearch);
  hlayout1->addWidget(m_btnContains);

  m_table = new JTableView();
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setEditTriggers(QTableView::NoEditTriggers);
  m_table->setSortingEnabled(true);
  m_table->horizontalHeader()->setHighlightSections(false);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->setContentsMargins(0, 0, 0, 0);
  vlayout1->addLayout(hlayout0);
  vlayout1->addLayout(hlayout1);
  vlayout1->addWidget(m_table);
  setLayout(vlayout1);

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
  QObject::connect(m_btnContains,
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
  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F),
                this,
                SLOT(focusFilterSearch()));

  filterSearchChanged();
  if (m_bSelectorMode)
  {
    m_btnOpen->hide();
    m_btnRefresh->hide();
    m_btnFilter->hide();
    m_btnRemove->setEnabled(false);
    m_btnRemove->hide();
    m_edFilterSearch->setFocus();
  }
}

JDatabase::~JDatabase()
{

}

void JDatabase::setDatabase(QSqlDatabase db,
                            const QString& tableName)
{
  if (m_table->model() != nullptr)
    return;

  JTableModel* model = nullptr;
  if (tableName == IMAGE_SQL_TABLE_NAME)
    model = new ImageTableModel(this, db);
  else if (tableName == NOTE_SQL_TABLE_NAME)
    model = new NoteTableModel(this, db);
  else if (tableName == REMINDER_SQL_TABLE_NAME)
    model = new ReminderTableModel(this, db);
  else if (tableName == PERSON_SQL_TABLE_NAME)
    model = new PersonTableModel(this, db);
  else if (tableName == USER_SQL_TABLE_NAME)
    model = new UserTableModel(this, db);
  else if (tableName == PRODUCT_SQL_TABLE_NAME)
    model = new ProductTableModel(this, db);
  else if (tableName == CATEGORY_SQL_TABLE_NAME)
    model = new CategoryTableModel(this, db);
  else
    return;

  m_db = db;
  m_tableName = tableName;
  model->setQuery();
  m_table->setModel(model);

  QObject::connect(m_table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));
  refresh(false);
}

QSqlDatabase JDatabase::getDatabase() const
{
  return m_db;
}

void JDatabase::selectItem()
{
  int row = m_table->currentIndex().row();
  JTableModel* model = dynamic_cast<JTableModel*>(m_table->model());
  if (model != nullptr && row >= 0)
  {
    qlonglong id = model->index(row, ID_COLUMN).data(Qt::EditRole).toLongLong();
    selectItem(id);
  }
}

void JDatabase::selectItem(qlonglong id)
{
  bool bSuccess = false;
  QString error;
  if (m_tableName == IMAGE_SQL_TABLE_NAME)
  {
    Image o;
    o.m_id = id;
    bSuccess = ImageSQL::select(m_db, o, error);
    if (bSuccess)
      emit itemSelectedSignal(o);
  }
  else if (m_tableName == PERSON_SQL_TABLE_NAME)
  {
    Person o;
    o.m_id = id;
    bSuccess = PersonSQL::select(m_db, o, error);
    if (bSuccess)
      emit itemSelectedSignal(o);
  }
  else if (m_tableName == CATEGORY_SQL_TABLE_NAME)
  {
    Category o;
    o.m_id = id;
    bSuccess = CategorySQL::select(m_db, o, error);
    if (bSuccess)
      emit itemSelectedSignal(o);
  }
  else if (m_tableName == PRODUCT_SQL_TABLE_NAME)
  {
    Product o;
    o.m_id = id;
    bSuccess = ProductSQL::select(m_db, o, error);
    if (bSuccess)
      emit itemSelectedSignal(o);
  }
  else if (m_tableName == NOTE_SQL_TABLE_NAME)
  {
    Note o;
    o.m_id = id;
    bSuccess = NoteSQL::select(m_db, o, error);
    if (bSuccess)
      emit itemSelectedSignal(o);
  }
  else if (m_tableName == USER_SQL_TABLE_NAME)
  {
    User o;
    o.m_id = id;
    bSuccess = UserSQL::select(m_db, o, error);
    if (bSuccess)
      emit itemSelectedSignal(o);
  }
  else if (m_tableName == REMINDER_SQL_TABLE_NAME)
  {
    Reminder o;
    o.m_id = id;
    bSuccess = ReminderSQL::select(m_db, o, error);
    if (bSuccess)
      emit itemSelectedSignal(o);
  }
  else
  {
    error = tr("Item ainda não implementado.");
  }

  if (!bSuccess)
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("O seguinte erro ocorreu ao selecionar o item com ID "
                             "'%1':\n'%2'").arg(QString::number(id), error));
  }
}

void JDatabase::refresh(bool bSaveIdx)
{
  if (m_table->model())
  {
    QModelIndex idx;
    if (bSaveIdx)
      idx = m_table->currentIndex();

    JTableModel* model = dynamic_cast<JTableModel*>(m_table->model());
    model->setQuery();

    if (bSaveIdx && idx.isValid())
      m_table->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
  }
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
  int row = m_table->currentIndex().row();
  if (row >= 0 && m_table->model())
  {
    qlonglong id = m_table->model()->index(row, ID_COLUMN).data(Qt::EditRole).toLongLong();
    bool bSuccess = false;
    QString error;
    if (m_tableName == IMAGE_SQL_TABLE_NAME)
      bSuccess = ImageSQL::remove(m_db, id, error);
    else if (m_tableName == PERSON_SQL_TABLE_NAME)
      bSuccess = PersonSQL::remove(m_db, id, error);
    else if (m_tableName == CATEGORY_SQL_TABLE_NAME)
      bSuccess = CategorySQL::remove(m_db, id, error);
    else if (m_tableName == PRODUCT_SQL_TABLE_NAME)
      bSuccess = ProductSQL::remove(m_db, id, error);
    else if (m_tableName == NOTE_SQL_TABLE_NAME)
      bSuccess = NoteSQL::remove(m_db, id, error);
    else if (m_tableName == USER_SQL_TABLE_NAME)
      bSuccess = UserSQL::remove(m_db, id, error);
    else if (m_tableName == REMINDER_SQL_TABLE_NAME)
      bSuccess = ReminderSQL::remove(m_db, id, error);
    else
      error = tr("Item ainda não implementado.");

    if (bSuccess)
    {
      emit itemRemovedSignal(id);
      refresh();
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
  if (m_table->model() == nullptr)
    return;

  JTableModel* model = dynamic_cast<JTableModel*>(m_table->model());
  int column = m_table->horizontalHeader()->sortIndicatorSection();
  if (m_edFilterSearch->text().isEmpty())
  {
    m_edFilterSearch->setPlaceholderText(tr("Procurar..."));
    model->setFilter("", false, column);
  }
  else
  {
    model->setFilter(m_edFilterSearch->text(), m_btnContains->isChecked(), column);
  }
  enableControls();
}

void JDatabase::filterSearchEnter()
{
  m_table->setFocus();
  if (m_table->model() != nullptr &&
      m_table->model()->rowCount() != 0)
  {
    m_table->selectRow(0);
  }
  else
  {
    m_edFilterSearch->setFocus();
    m_edFilterSearch->selectAll();
  }
}

void JDatabase::containsPressed()
{
  filterSearchChanged();
  m_edFilterSearch->setFocus();
}

void JDatabase::focusFilterSearch()
{
  m_edFilterSearch->setFocus();
}

void JDatabase::setUserFilter(const QString& userFilter)
{
  m_userFilter = userFilter;
  filterSearchChanged();
}

QString JDatabase::getTableName() const
{
  return m_tableName;
}

bool JDatabase::save(const JItem& jItem)
{
  QString error;
  bool bSuccess = false;
  if (m_tableName == IMAGE_SQL_TABLE_NAME)
  {
    const Image& o = dynamic_cast<const Image&>(jItem);
    bSuccess = o.isValidId()
               ? ImageSQL::update(m_db, o, error)
               : ImageSQL::insert(m_db, o, error);
  }
  else if (m_tableName == PERSON_SQL_TABLE_NAME)
  {
    const Person& o = dynamic_cast<const Person&>(jItem);
    bSuccess = o.isValidId()
               ? PersonSQL::update(m_db, o, error)
               : PersonSQL::insert(m_db, o, error);
  }
  else if (m_tableName == CATEGORY_SQL_TABLE_NAME)
  {
    const Category& o = dynamic_cast<const Category&>(jItem);
    bSuccess = o.isValidId()
               ? CategorySQL::update(m_db, o, error)
               : CategorySQL::insert(m_db, o, error);
  }
  else if (m_tableName == PRODUCT_SQL_TABLE_NAME)
  {
    const Product& o = dynamic_cast<const Product&>(jItem);
    bSuccess = o.isValidId()
               ? ProductSQL::update(m_db, o, error)
               : ProductSQL::insert(m_db, o, error);
  }
  else if (m_tableName == NOTE_SQL_TABLE_NAME)
  {
    const Note& o = dynamic_cast<const Note&>(jItem);
    bSuccess = o.isValidId()
               ? NoteSQL::update(m_db, o, error)
               : NoteSQL::insert(m_db, o, error);
  }
  else if (m_tableName == USER_SQL_TABLE_NAME)
  {
    const User& o = dynamic_cast<const User&>(jItem);
    bSuccess = o.isValidId()
               ? UserSQL::update(m_db, o, error)
               : UserSQL::insert(m_db, o, error);
  }
  else if (m_tableName == REMINDER_SQL_TABLE_NAME)
  {
    const Reminder& o = dynamic_cast<const Reminder&>(jItem);
    bSuccess = o.isValidId()
               ? ReminderSQL::update(m_db, o, error)
               : ReminderSQL::insert(m_db, o, error);
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

JDatabaseSelector::JDatabaseSelector(const QString& title,
                                     const QIcon& icon,
                                     QWidget* parent)
  : QDialog(parent)
  , m_database(nullptr)
{
  m_database = new JDatabase(true);
  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->addWidget(m_database);
  setLayout(hlayout0);

  setWindowTitle(title);
  setWindowIcon(icon);

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));
}

void JDatabaseSelector::setDatabase(QSqlDatabase db,
                                    const QString& tableName)
{
  m_database->setDatabase(db, tableName);
}

void JDatabaseSelector::itemSelected(const JItem& jItem)
{
  if (m_database->getTableName() == IMAGE_SQL_TABLE_NAME)
    m_currentImage = dynamic_cast<const Image&>(jItem);
  else if (m_database->getTableName() == PERSON_SQL_TABLE_NAME)
    m_currentPerson = dynamic_cast<const Person&>(jItem);
  else if (m_database->getTableName() == CATEGORY_SQL_TABLE_NAME)
    m_currentCategory = dynamic_cast<const Category&>(jItem);
  else if (m_database->getTableName() == PRODUCT_SQL_TABLE_NAME)
    m_currentProduct = dynamic_cast<const Product&>(jItem);
  emit itemSelectedSignal(jItem);
  close();
}

void JDatabaseSelector::setUserFilter(const QString& userFilter)
{
  m_database->setUserFilter(userFilter);
}

Product JDatabaseSelector::getCurrentProduct() const
{
  return m_currentProduct;
}

Person JDatabaseSelector::getCurrentPerson() const
{
  return m_currentPerson;
}

Category JDatabaseSelector::getCurrentCategory() const
{
  return m_currentCategory;
}

Image JDatabaseSelector::getCurrentImage() const
{
  return m_currentImage;
}

QString JDatabaseSelector::getTableName() const
{
  return m_database->getTableName();
}

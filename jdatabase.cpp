#include "jdatabase.h"
#include "jlineedit.h"
#include "defines.h"
#include "databaseutils.h"
#include "imagetablemodel.h"
#include <QDate>
#include <QLayout>
#include <QSqlRecord>
#include <QPushButton>
#include <QHeaderView>
#include <QShortcut>
#include <QMessageBox>

class NoteTableModel : public QSqlTableModel
{
public:
  NoteTableModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
  {

  }

  QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const
  {
    QVariant value = QSqlTableModel::data(idx, role);
    if (role == Qt::DisplayRole)
    {
      if (idx.column() == 2)
        value = QDate::fromString(value.toString(), Qt::ISODate).toString("dd/MM/yyyy");
      else if (idx.column() == 3)
      {
        Person person;
        person.m_id = value.toLongLong();
        QString error;
        PersonSQL::select(database(), person, error);
        value = person.m_alias;
      }
      else if (idx.column() == 4)
        value = "R$ " + QString::number(value.toDouble(), 'f', 2);
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

  m_edFilterSearch = new JLineEdit(JValidatorType::All, false, false);
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
                            const QString& tableName,
                            const QVector<JTableColumn>& vColumns)
{
  if (m_table->model() != nullptr)
    return;

  QSqlTableModel* model = nullptr;
  if (m_tableName == IMAGE_SQL_TABLE_NAME)
    model = new ImageTableModel(this, db);
  else if (m_tableName == NOTE_SQL_TABLE_NAME)
    model = new NoteTableModel(this, db);
  else
    model = new QSqlTableModel(this, db);


  m_tableName = tableName;
  m_vColumns = vColumns;

  model->setTable(m_tableName);
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m_table->setModel(model);

  int count = m_table->horizontalHeader()->count();
  if (m_vColumns.size() == count)
  {
    for (int i = 0; i != m_vColumns.size(); ++i)
    {
      model->setHeaderData(i, Qt::Horizontal, m_vColumns.at(i).m_friendlyName);
      if (!m_vColumns.at(i).m_icon.isEmpty())
        model->setHeaderData(i,
                             Qt::Horizontal,
                             QVariant::fromValue(QIcon(m_vColumns.at(i).m_icon)),
                             Qt::DecorationRole);
    }

    for (int i = 0; i != m_vColumns.size(); ++i)
    {
      m_table->horizontalHeader()->setSectionResizeMode(i, (QHeaderView::ResizeMode)m_vColumns.at(i).m_resizeMode);
      if (m_vColumns.at(i).m_bHidden)
        m_table->hideColumn(i);
      if (m_vColumns.at(i).m_bSort)
        m_table->horizontalHeader()->setSortIndicator(i, Qt::SortOrder::AscendingOrder);
    }
  }

  QObject::connect(m_table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));
  refresh();
}

QSqlDatabase JDatabase::getDatabase() const
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    return model->database();
  }
  return QSqlDatabase();
}

void JDatabase::selectItem()
{
  int row = m_table->currentIndex().row();
  QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
  if (model != nullptr && row >= 0)
  {
    qlonglong id = model->index(row, ID_COLUMN).data(Qt::EditRole).toLongLong();
    selectItem(id);
  }
}

void JDatabase::selectItem(qlonglong id)
{
  QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
  if (model != nullptr)
  {
    bool bSuccess = false;
    QString error;
    if (m_tableName == IMAGE_SQL_TABLE_NAME)
    {
      Image o;
      o.m_id = id;
      bSuccess = ImageSQL::select(model->database(), o, error);
      if (bSuccess)
        emit itemSelectedSignal(o);
    }
    else if (m_tableName == PERSON_SQL_TABLE_NAME)
    {
      Person o;
      o.m_id = id;
      bSuccess = PersonSQL::select(model->database(), o, error);
      if (bSuccess)
        emit itemSelectedSignal(o);
    }
    else if (m_tableName == CATEGORY_SQL_TABLE_NAME)
    {
      Category o;
      o.m_id = id;
      bSuccess = CategorySQL::select(model->database(), o, error);
      if (bSuccess)
        emit itemSelectedSignal(o);
    }
    else if (m_tableName == PRODUCT_SQL_TABLE_NAME)
    {
      Product o;
      o.m_id = id;
      bSuccess = ProductSQL::select(model->database(), o, error);
      if (bSuccess)
        emit itemSelectedSignal(o);
    }
    else if (m_tableName == NOTE_SQL_TABLE_NAME)
    {
      Note o;
      o.m_id = id;
      bSuccess = NoteSQL::select(model->database(), o, error);
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
}

void JDatabase::refresh()
{
  if (m_table->model())
  {
    QModelIndex idx = m_table->currentIndex();
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    model->select();
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
  QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
  if (model != nullptr && row >= 0)
  {
    qlonglong id = model->index(row, ID_COLUMN).data(Qt::EditRole).toLongLong();
    bool bSuccess = false;
    QString error;
    if (m_tableName == IMAGE_SQL_TABLE_NAME)
      bSuccess = ImageSQL::remove(model->database(), id, error);
    else if (m_tableName == PERSON_SQL_TABLE_NAME)
      bSuccess = PersonSQL::remove(model->database(), id, error);
    else if (m_tableName == CATEGORY_SQL_TABLE_NAME)
      bSuccess = CategorySQL::remove(model->database(), id, error);
    else if (m_tableName == PRODUCT_SQL_TABLE_NAME)
      bSuccess = ProductSQL::remove(model->database(), id, error);
    else if (m_tableName == NOTE_SQL_TABLE_NAME)
      bSuccess = NoteSQL::remove(model->database(), id, error);
    else if (m_tableName == USER_SQL_TABLE_NAME)
      bSuccess = UserSQL::remove(model->database(), id, error);
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

  QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
  QString str = m_edFilterSearch->text();
  str = str.toUpper();
  m_edFilterSearch->setTextBlockingSignals(str);
  int column = m_table->horizontalHeader()->sortIndicatorSection();
  if (str.isEmpty())
  {
    m_edFilterSearch->setPlaceholderText(tr("Procurar pelo(a) ") +
                                         m_vColumns.at(column).m_friendlyName.toLower());
    model->setFilter("");
  }
  else
  {
    QString filter = m_vColumns.at(column).m_sqlName + " LIKE '";
    if (m_btnContains->isChecked())
        filter += "%";
    filter += str + "%'";
    model->setFilter(filter);
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

QString JDatabase::getTableName() const
{
  return m_tableName;
}

bool JDatabase::save(const JItem& jItem)
{
  QString error;
  bool bSuccess = false;
  QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
  if (model != nullptr)
  {
    if (m_tableName == IMAGE_SQL_TABLE_NAME)
    {
      const Image& o = dynamic_cast<const Image&>(jItem);
      bSuccess = o.isValidId()
                 ? ImageSQL::update(model->database(), o, error)
                 : ImageSQL::insert(model->database(), o, error);
    }
    else if (m_tableName == PERSON_SQL_TABLE_NAME)
    {
      const Person& o = dynamic_cast<const Person&>(jItem);
      bSuccess = o.isValidId()
                 ? PersonSQL::update(model->database(), o, error)
                 : PersonSQL::insert(model->database(), o, error);
    }
    else if (m_tableName == CATEGORY_SQL_TABLE_NAME)
    {
      const Category& o = dynamic_cast<const Category&>(jItem);
      bSuccess = o.isValidId()
                 ? CategorySQL::update(model->database(), o, error)
                 : CategorySQL::insert(model->database(), o, error);
    }
    else if (m_tableName == PRODUCT_SQL_TABLE_NAME)
    {
      const Product& o = dynamic_cast<const Product&>(jItem);
      bSuccess = o.isValidId()
                 ? ProductSQL::update(model->database(), o, error)
                 : ProductSQL::insert(model->database(), o, error);
    }
    else if (m_tableName == NOTE_SQL_TABLE_NAME)
    {
      const Note& o = dynamic_cast<const Note&>(jItem);
      bSuccess = o.isValidId()
                 ? NoteSQL::update(model->database(), o, error)
                 : NoteSQL::insert(model->database(), o, error);
    }
    else
      error = tr("Item ainda não implementado.");
  }

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
                                    const QString& tableName,
                                    const QVector<JTableColumn>& vColumns)
{
  m_database->setDatabase(db, tableName, vColumns);
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

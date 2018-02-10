#include "jdatabase.h"
#include "jlineedit.h"
#include <QDate>
#include <QLayout>
#include <QSqlRecord>
#include <QPushButton>

JTableView::JTableView(QWidget *parent)
  : QTableView(parent)
{

}

void JTableView::keyPressEvent(QKeyEvent* event)
{
  emit enterKeyPressedSignal();
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

  m_btnRefresh = new QPushButton();
  m_btnRefresh->setFlat(true);
  m_btnRefresh->setText("");
  m_btnRefresh->setIconSize(QSize(24, 24));
  m_btnRefresh->setIcon(QIcon(":/icons/res/refresh.png"));

  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/remove.png"));

  m_btnFilter = new QPushButton();
  m_btnFilter->setFlat(true);
  m_btnFilter->setText("");
  m_btnFilter->setIconSize(QSize(24, 24));
  m_btnFilter->setIcon(QIcon(":/icons/res/filter.png"));

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->setAlignment(Qt::AlignLeft);
  hlayout0->addWidget(m_btnOpen);
  hlayout0->addWidget(m_btnRefresh);
  hlayout0->addWidget(m_btnRemove);
  hlayout0->addWidget(m_btnFilter);

  m_edFilterSearch = new JLineEdit(false, false);
  m_edFilterSearch->setClearButtonEnabled(true);

  m_btnContains = new QPushButton();
  m_btnContains->setFlat(true);
  m_btnContains->setText("");
  m_btnContains->setIconSize(QSize(24, 24));
  m_btnContains->setIcon(QIcon(":/icons/res/center.png"));
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
                   SLOT(itemSelected(const QModelIndex&)));

  QObject::connect(m_btnOpen,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(itemSelected()));

  QObject::connect(m_btnRefresh,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(refresh()));

  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitItemRemoveSignal()));

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
                   SLOT(emitEnterKeyPressedSignal()));

  QObject::connect(m_table,
                   SIGNAL(enterKeyPressedSignal()),
                   this,
                   SLOT(itemSelected()));

  filterSearchChanged();
  if (m_bSelectorMode)
  {
    m_btnRemove->hide();
    m_edFilterSearch->setFocus();
  }
}

JDatabase::~JDatabase()
{

}

void JDatabase::set(QSqlTableModel* model,
                    const QString& tableName,
                    const QVector<SqlTableColumn>& sqlTableColumns)
{
  if (m_table->model() != nullptr || model == nullptr)
    return;

  m_tableName = tableName;
  m_columns = sqlTableColumns;

  model->setTable(m_tableName);
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m_table->setModel(model);

  int count = m_table->horizontalHeader()->count();
  if (m_columns.size() == count)
  {
    for (int i = 0; i != m_columns.size(); ++i)
      model->setHeaderData(i, Qt::Horizontal, m_columns.at(i).m_friendlyName);

    for (int i = 0; i != m_columns.size(); ++i)
    {
      m_table->horizontalHeader()->setSectionResizeMode(i, m_columns.at(i).m_resizeMode);
      if (m_columns.at(i).m_bHidden)
        m_table->hideColumn(i);
      if (m_columns.at(i).m_bSort)
        m_table->horizontalHeader()->setSortIndicator(i, Qt::SortOrder::AscendingOrder);
    }
  }

  QObject::connect(m_table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));
  refresh();
}

QSqlDatabase JDatabase::get() const
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    return model->database();
  }
  return QSqlDatabase();
}

void JDatabase::itemSelected()
{
  itemSelected(m_table->currentIndex());
}

void JDatabase::itemSelected(const QModelIndex& idx)
{
  if (m_table->model() != nullptr && idx.isValid())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    emit itemSelectedSignal(model->index(idx.row(), ID_COLUMN).data(Qt::EditRole).toInt());
  }
}

void JDatabase::refresh()
{
  if (m_table->model())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    model->select();
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

void JDatabase::emitItemRemoveSignal()
{
  if (m_table->model())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    if (m_table->currentIndex().isValid())
    {
      int id = model->index(m_table->currentIndex().row(),
                            ID_COLUMN).data(Qt::EditRole).toInt();
      emit itemRemoveSignal(id);
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
                                         m_columns.at(column).m_friendlyName.toLower());
    model->setFilter("");
  }
  else
  {
    QString filter = m_columns.at(column).m_sqlName + " LIKE '";
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

void JDatabase::emitEnterKeyPressedSignal()
{
  if (m_table->model())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    if (m_table->currentIndex().isValid())
    {
      int id = model->index(m_table->currentIndex().row(),
                            ID_COLUMN).data(Qt::EditRole).toInt();
      emit enterKeyPressedSignal(id);
    }
  }
}

JDatabaseSelector::JDatabaseSelector(const QString& title,
                                     const QIcon& icon,
                                     int invalidId,
                                     QWidget* parent)
  : QDialog(parent)
  , m_currentId(invalidId)
  , m_database(nullptr)
{
  m_database = new JDatabase(true);
  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->addWidget(m_database);
  setLayout(hlayout0);

  setWindowTitle(title);
  setWindowIcon(icon);

  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(int)),
                   this,
                   SLOT(itemSelected(int)));
}

void JDatabaseSelector::set(QSqlTableModel* model,
                            const QString& tableName,
                            const QVector<SqlTableColumn>& sqlTableColumns)
{
  m_database->set(model, tableName, sqlTableColumns);
}

void JDatabaseSelector::itemSelected(int id)
{
  m_currentId = id;
  close();
}

int JDatabaseSelector::getCurrentId() const
{
  return m_currentId;
}

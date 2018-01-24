#include "notedatabase.h"
#include "jlineedit.h"
#include <QDate>
#include <QLayout>
#include <QSqlRecord>
#include <QPushButton>
#include <QCheckBox>
#include <QTableView>
#include <QSqlTableModel>
#include <QHeaderView>

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
      if (idx.column() == (int)NoteTableIndex::Date)
        value = QDate::fromJulianDay(value.toLongLong()).toString("dd/MM/yyyy");
      else if (idx.column() == (int)NoteTableIndex::Total)
        value = "R$ " + QString::number(value.toDouble(), 'f', 2);
      if (idx.column() == (int)NoteTableIndex::Cash)
        value = "";
    }
    else if (role == Qt::DecorationRole)
    {
      if (idx.column() == (int)NoteTableIndex::Cash)
      {
        value = QSqlTableModel::data(idx, Qt::EditRole).toBool()
                ? QVariant::fromValue(QIcon(":/icons/res/check.png"))
                : "";
      }
    }
    return value;
  }
};


NoteDatabase::NoteDatabase(QWidget *parent)
  : QFrame(parent)
  , m_btnOpen(nullptr)
  , m_btnRefresh(nullptr)
  , m_btnRemove(nullptr)
  , m_btnFilter(nullptr)
  , m_edFilterSearch(nullptr)
  , m_cbContains(nullptr)
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

  m_edFilterSearch = new JLineEdit(false, false);

  m_cbContains = new QCheckBox();
  m_cbContains->setText(tr("Contendo"));

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_btnOpen);
  hlayout1->addWidget(m_btnRefresh);
  hlayout1->addWidget(m_btnRemove);
  hlayout1->addWidget(m_btnFilter);
  hlayout1->addWidget(m_edFilterSearch);
  hlayout1->addWidget(m_cbContains);

  m_table = new QTableView();
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setEditTriggers(QTableView::NoEditTriggers);
  m_table->setSortingEnabled(true);
  m_table->horizontalHeader()->setHighlightSections(false);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->setContentsMargins(0, 0, 0, 0);
  vlayout1->addLayout(hlayout1);
  vlayout1->addWidget(m_table);
  setLayout(vlayout1);

  QObject::connect(m_table,
                   SIGNAL(doubleClicked(const QModelIndex&)),
                   this,
                   SLOT(noteSelected(const QModelIndex&)));

  QObject::connect(m_btnOpen,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(noteSelected()));

  QObject::connect(m_btnRefresh,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(refresh()));

  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitNoteRemoveSignal()));

  QObject::connect(m_edFilterSearch,
                   SIGNAL(textEdited(const QString&)),
                   this,
                   SLOT(filterSearchChanged()));

  QObject::connect(m_edFilterSearch,
                   SIGNAL(enterSignal()),
                   this,
                   SLOT(filterSearchEnter()));

  QObject::connect(m_cbContains,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(containsPressed()));
}

void NoteDatabase::set(QSqlDatabase db,
                       const QString& tableName,
                       const QVector<SqlTableColumn>& sqlTableColumns)
{
  if (m_table->model() != nullptr)
    return;

  m_tableName = tableName;
  m_columns = sqlTableColumns;
  NoteTableModel* model = new NoteTableModel(this, db);
  model->setTable(m_tableName);
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  for (int i = 0; i != m_columns.size(); ++i)
  {
    model->setHeaderData(i, Qt::Horizontal, m_columns.at(i).m_friendlyName);
    if (m_columns.at(i).m_bHidden)
      m_table->hideColumn(i);
  }

  m_table->setModel(model);
  QObject::connect(m_table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));
  refresh();
}

QSqlDatabase NoteDatabase::get() const
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    return model->database();
  }
  return QSqlDatabase();
}

NoteDatabase::~NoteDatabase()
{

}

void NoteDatabase::noteSelected()
{
  noteSelected(m_table->currentIndex());
}

void NoteDatabase::noteSelected(const QModelIndex& idx)
{
  if (m_table->model() != nullptr && idx.isValid())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    emit noteSelectedSignal(model->index(idx.row(), (int)NoteTableIndex::ID).data(Qt::EditRole).toInt());
  }
}

void NoteDatabase::refresh()
{
  if (m_table->model())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    model->select();
  }
  enableControls();
}

void NoteDatabase::enableControls()
{
  bool bNoteSelected = m_table->currentIndex().isValid();
  m_btnOpen->setEnabled(bNoteSelected);
  m_btnRemove->setEnabled(bNoteSelected);
}

void NoteDatabase::emitNoteRemoveSignal()
{
  if (m_table->model())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    if (m_table->currentIndex().isValid())
    {
      int id = model->index(m_table->currentIndex().row(),
                            (int)NoteTableIndex::ID).data(Qt::EditRole).toInt();
      emit noteRemoveSignal(id);
    }
  }
}

void NoteDatabase::filterSearchChanged()
{
  if (m_table->model() == nullptr)
    return;

  QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
  QString str = m_edFilterSearch->text();
  str.toUpper();
  m_edFilterSearch->setText(str);
  int column = m_table->horizontalHeader()->sortIndicatorSection();
  if (str.isEmpty())
  {
    m_edFilterSearch->setPlaceholderText(tr("Procurar pelo(a) ") + m_columns.at(column).m_friendlyName);
    model->setFilter("");
  }
  else
  {
    QString filter = filter = m_columns.at(column).m_sqlName + " LIKE '";
    if (m_cbContains->isChecked())
        filter += "%";
    filter += str + "%'";
    model->setFilter(filter);
  }
}

void NoteDatabase::filterSearchEnter()
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

void NoteDatabase::containsPressed()
{
  filterSearchChanged();
  m_edFilterSearch->setFocus();
}

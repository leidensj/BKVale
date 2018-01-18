#include "notedatabase.h"
#include <QDate>
#include <QLayout>
#include <QSqlRecord>
#include <QPushButton>
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

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
  {
    {
      if (!index.isValid())
        return QModelIndex();

      QVariant value = QSqlTableModel::data(index, role);
      if (role == Qt::DisplayRole)
      {
        if (index.column() == (int)NoteTableIndex::Date)
          value = QDate::fromJulianDay(value.toLongLong()).toString("dd/MM/yyyy");
        else if (index.column() == (int)NoteTableIndex::Total)
          value = "R$ " + QString::number(value.toDouble(), 'f', 2);
      }

      return value;
    }
  }
};


NoteDatabase::NoteDatabase(QWidget *parent)
  : QFrame(parent)
  , m_btnOpen(nullptr)
  , m_btnRefresh(nullptr)
  , m_btnRemove(nullptr)
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

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_btnOpen);
  hlayout1->addWidget(m_btnRefresh);
  hlayout1->addWidget(m_btnRemove);

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
}

void NoteDatabase::set(QSqlDatabase db)
{
  if (m_table->model() != nullptr)
    return;

  NoteTableModel* model = new NoteTableModel(this, db);
  model->setTable("_PROMISSORYNOTES");
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setHeaderData((int)NoteTableIndex::ID, Qt::Horizontal, tr("ID"));
  model->setHeaderData((int)NoteTableIndex::Number, Qt::Horizontal, tr("Número"));
  model->setHeaderData((int)NoteTableIndex::Date, Qt::Horizontal, tr("Data"));
  model->setHeaderData((int)NoteTableIndex::Supplier, Qt::Horizontal, tr("Fornecedor"));
  model->setHeaderData((int)NoteTableIndex::Total, Qt::Horizontal, tr("Total"));
  m_table->setModel(model);
  m_table->hideColumn((int)NoteTableIndex::ID);
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

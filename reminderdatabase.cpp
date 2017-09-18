#include "reminderdatabase.h"
#include "databaseutils.h"
#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QHeaderView>
#include <QMessageBox>

namespace
{
QString friendlyColumnName(ReminderTableIndex idx)
{
  switch (idx)
  {
    case ReminderTableIndex::ID:
      return QObject::tr("ID");
    case ReminderTableIndex::Title:
      return QObject::tr("Título");
    case ReminderTableIndex::Message:
      return QObject::tr("Mensagem");
    case ReminderTableIndex::Favorite:
      return QObject::tr("Favorito");
    case ReminderTableIndex::Capitalization:
      return QObject::tr("Capital");
    case ReminderTableIndex::Size:
      return QObject::tr("Tamanho");
    default:
      return "";
  }
}
}

ReminderTableModel::ReminderTableModel(QObject *parent, QSqlDatabase db)
  : QSqlTableModel(parent, db)
{

}

QVariant ReminderTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QModelIndex();

  QVariant value = QSqlTableModel::data(index, role);
  if (role == Qt::DisplayRole)
  {
    if (index.column() == (int)ReminderTableIndex::Favorite)
      value = QVariant::fromValue(QIcon(":/icons/res/favorite.png"));
  }

  return value;
}

ReminderDatabase::ReminderDatabase(QWidget *parent)
  : QFrame(parent)
  , m_table(nullptr)
{
  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->setContentsMargins(0, 0, 0, 0);
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
  m_btnRemove->setIcon(QIcon(":/icons/res/trash.png"));
  m_btnFavorite = new QPushButton();
  m_btnFavorite->setFlat(true);
  m_btnFavorite->setText("");
  m_btnFavorite->setIconSize(QSize(24, 24));
  m_btnFavorite->setIcon(QIcon(":/icons/res/favorite.png"));
  m_btnFavorite->setCheckable(true);
  m_btnFavorite->setChecked(false);
  QHBoxLayout* buttons = new QHBoxLayout();
  buttons->setContentsMargins(0, 0, 0, 0);
  buttons->addWidget(m_btnOpen);
  buttons->addWidget(m_btnRefresh);
  buttons->addWidget(m_btnRemove);
  buttons->addWidget(m_btnFavorite);
  buttons->setAlignment(Qt::AlignLeft);

  m_table = new QTableView(this);
  m_table->setSelectionBehavior(QAbstractItemView::SelectItems);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);

  vlayout->addLayout(buttons);
  vlayout->addWidget(m_table);
  setLayout(vlayout);

  QObject::connect(m_table,
                   SIGNAL(doubleClicked(const QModelIndex&)),
                   this,
                   SLOT(emitSelectedSignal()));

  QObject::connect(m_btnOpen,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSelectedSignal()));

  enableControls();
}

void ReminderDatabase::setDatabase(QSqlDatabase db)
{
  if (m_table->model() != nullptr)
    return;

  QSqlTableModel* model = new QSqlTableModel(this, db);
  model->setTable("_REMINDERS");
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);

  model->setHeaderData((int)ReminderTableIndex::ID,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderTableIndex::ID));
  model->setHeaderData((int)ReminderTableIndex::Title,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderTableIndex::Title));
  model->setHeaderData((int)ReminderTableIndex::Message,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderTableIndex::Message));
  model->setHeaderData((int)ReminderTableIndex::Favorite,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderTableIndex::Favorite));
  model->setHeaderData((int)ReminderTableIndex::Capitalization,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderTableIndex::Capitalization));
  model->setHeaderData((int)ReminderTableIndex::Size,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderTableIndex::Size));

  m_table->setModel(model);
  m_table->hideColumn((int)ReminderTableIndex::ID);
  m_table->hideColumn((int)ReminderTableIndex::Message);
  m_table->hideColumn((int)ReminderTableIndex::Capitalization);
  m_table->hideColumn((int)ReminderTableIndex::Size);
  m_table->horizontalHeader()->setSortIndicator((int)ReminderTableIndex::Favorite,
                                                  Qt::SortOrder::AscendingOrder);
  m_table->horizontalHeader()->setSectionResizeMode((int)ReminderTableIndex::Title,
                                                      QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode((int)ReminderTableIndex::Favorite,
                                                      QHeaderView::ResizeToContents);

  QObject::connect(m_table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));

  QObject::connect(model,
                   SIGNAL(dataChanged(const QModelIndex&,
                                      const QModelIndex&,
                                      const QVector<int>&)),
                   this,
                   SLOT(enableControls()));
  enableControls();
}

void ReminderDatabase::enableControls()
{
  bool bSelected = m_table->currentIndex().isValid();
  m_btnOpen->setEnabled(bSelected);
  m_btnRemove->setEnabled(bSelected);
  m_btnFavorite->setEnabled(bSelected);
}

bool ReminderDatabase::insertOrUpdate(const Reminder& r, QString& error)
{
  error.clear();
  QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
  bool bSuccess = false;
  if (model != nullptr)
  {
    bSuccess = ReminderSQL::insertOrUpdate(model->database(), r, error);
    refresh();
  }
  enableControls();
  return bSuccess;
}

void ReminderDatabase::remove()
{
  if (m_table->currentIndex().isValid())
  {
    int row = m_table->currentIndex().row();
    if (m_table->model() != nullptr)
    {
      if (QMessageBox::question(this,
                                tr("Remover Lembrete"),
                                tr("Tem certeza que deseja remover o lembrete selecionado?"),
                                QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
      {
        QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
        model->removeRow(row);
        model->submitAll();
        refresh();
      }
    }
  }
  enableControls();
}

void ReminderDatabase::refresh()
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    model->select();
  }
  enableControls();
}

void ReminderDatabase::emitSelectedSignal()
{
  QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
  if (model != nullptr && m_table->currentIndex().isValid())
  {
    const int row = m_table->currentIndex().row();
    Reminder r;
    r.m_id = model->index(row, (int)ReminderTableIndex::ID).data(Qt::EditRole).toInt();
    r.m_title = model->index(row, (int)ReminderTableIndex::Title).data(Qt::EditRole).toString();
    r.m_message = model->index(row, (int)ReminderTableIndex::Message).data(Qt::EditRole).toString();
    r.m_bFavorite = model->index(row, (int)ReminderTableIndex::Favorite).data(Qt::EditRole).toBool();
    r.m_capitalization = (Reminder::Capitalization)model->index(row, (int)ReminderTableIndex::Capitalization).data(Qt::EditRole).toInt();
    r.m_size = (Reminder::Size)model->index(row, (int)ReminderTableIndex::Size).data(Qt::EditRole).toInt();
    emit selectedSignal(r);
  }
}

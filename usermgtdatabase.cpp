#include "usermgtdatabase.h"
#include "user.h"
#include <QLayout>
#include <QSqlRecord>
#include <QPushButton>
#include <QTableView>
#include <QSqlTableModel>
#include <QHeaderView>

class UserTableModel : public QSqlTableModel
{

public:
  UserTableModel(QObject *parent, QSqlDatabase db)
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
        //TODO
      }
      return value;
    }
  }
};


UserMgtDatabase::UserMgtDatabase(QWidget *parent)
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
                   SLOT(userSelected(const QModelIndex&)));

  QObject::connect(m_btnOpen,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(userSelected()));

  QObject::connect(m_btnRefresh,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(refresh()));

  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitUserRemoveSignal()));
}

void UserMgtDatabase::set(QSqlDatabase db)
{
  if (m_table->model() != nullptr)
    return;

  UserTableModel* model = new UserTableModel(this, db);
  model->setTable("_USERS");
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setHeaderData((int)UserTableIndex::ID, Qt::Horizontal, tr("ID"));
  model->setHeaderData((int)UserTableIndex::User, Qt::Horizontal, tr("Usuário"));
  model->setHeaderData((int)UserTableIndex::Password, Qt::Horizontal, tr("Senha"));
  model->setHeaderData((int)UserTableIndex::AccessNote, Qt::Horizontal, tr("Notas"));
  model->setHeaderData((int)UserTableIndex::AccessReminder, Qt::Horizontal, tr("Lembretes"));
  model->setHeaderData((int)UserTableIndex::AccessCalculator, Qt::Horizontal, tr("Calculadora"));
  model->setHeaderData((int)UserTableIndex::AccessShop, Qt::Horizontal, tr("Compras"));
  model->setHeaderData((int)UserTableIndex::AccessConsumption, Qt::Horizontal, tr("Consumo"));
  model->setHeaderData((int)UserTableIndex::AccessUser, Qt::Horizontal, tr("Usuários"));
  model->setHeaderData((int)UserTableIndex::AccessItem, Qt::Horizontal, tr("Produtos"));
  model->setHeaderData((int)UserTableIndex::AccessSettings, Qt::Horizontal, tr("Configurações"));
  m_table->setModel(model);
  m_table->hideColumn((int)UserTableIndex::ID);
  m_table->hideColumn((int)UserTableIndex::Password);
  m_table->hideColumn((int)UserTableIndex::AccessNote);
  m_table->hideColumn((int)UserTableIndex::AccessReminder);
  m_table->hideColumn((int)UserTableIndex::AccessCalculator);
  m_table->hideColumn((int)UserTableIndex::AccessCalculator);
  m_table->hideColumn((int)UserTableIndex::AccessShop);
  m_table->hideColumn((int)UserTableIndex::AccessConsumption);
  m_table->hideColumn((int)UserTableIndex::AccessUser);
  m_table->hideColumn((int)UserTableIndex::AccessItem);
  m_table->hideColumn((int)UserTableIndex::AccessSettings);

  QObject::connect(m_table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));
  refresh();
}

QSqlDatabase UserMgtDatabase::get() const
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    return model->database();
  }
  return QSqlDatabase();
}

UserMgtDatabase::~UserMgtDatabase()
{

}

void UserMgtDatabase::userSelected()
{
  userSelected(m_table->currentIndex());
}

void UserMgtDatabase::userSelected(const QModelIndex& idx)
{
  if (m_table->model() != nullptr && idx.isValid())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    emit userSelectedSignal(model->index(idx.row(), (int)UserTableIndex::ID).data(Qt::EditRole).toInt());
  }
}

void UserMgtDatabase::refresh()
{
  if (m_table->model())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    model->select();
  }
  enableControls();
}

void UserMgtDatabase::enableControls()
{
  bool bUserSelected = m_table->currentIndex().isValid();
  m_btnOpen->setEnabled(bUserSelected);
  m_btnRemove->setEnabled(bUserSelected);
}

void UserMgtDatabase::emitUserRemoveSignal()
{
  if (m_table->model())
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    if (m_table->currentIndex().isValid())
    {
      int id = model->index(m_table->currentIndex().row(),
                            (int)UserTableIndex::ID).data(Qt::EditRole).toInt();
      emit userRemoveSignal(id);
    }
  }
}


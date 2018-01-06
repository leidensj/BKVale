#include "consumptiondatabase.h"
#include <QDialog>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDate>
#include "databaseutils.h"

namespace
{
QString friendlyColumnName(ConsumptionTableIndex idx)
{
  switch (idx)
  {
    case ConsumptionTableIndex::ID:
      return QObject::tr("ID"); break;
    case ConsumptionTableIndex::Date:
      return QObject::tr("Data"); break;
    case ConsumptionTableIndex::ItemID:
      return QObject::tr("Item"); break;
    case ConsumptionTableIndex::Ammount:
      return QObject::tr("Quantidade"); break;
    case ConsumptionTableIndex::Price:
      return QObject::tr("Preço"); break;
    case ConsumptionTableIndex::Total:
      return QObject::tr("Total"); break;
    default:
      return "";
  }
}

void setColumnText(QSqlTableModel* model,
                   ConsumptionTableIndex idx)
{
  if (model != nullptr)
  {
    model->setHeaderData((int)idx,
                         Qt::Horizontal,
                         friendlyColumnName(idx));
  }
}
}

class ConsumptionTableModel : public QSqlTableModel
{

public:
  ConsumptionTableModel(QObject *parent, QSqlDatabase db)
   : QSqlTableModel(parent, db)
  {

  }

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
  {
    if (!index.isValid())
      return QModelIndex();

    QVariant value = QSqlTableModel::data(index, role);
    if (role == Qt::DisplayRole)
    {
      switch ((ConsumptionTableIndex)index.column())
      {
        case ConsumptionTableIndex::Date:
          value = QDate::fromJulianDay(value.toLongLong()).toString("dd/MM/yyyy");
          break;
        case ConsumptionTableIndex::Price:
          value = "R$ " + QString::number(value.toDouble(), 'f', 2);
          break;
        case ConsumptionTableIndex::Ammount:
        {
          Item item;
          item.m_id = record(index.row()).value("_ITEMID").toInt();
          QString error;
          ItemSQL::select(database(),
                          item,
                          error);
          value = QString::number(value.toDouble(), 'f', 3) + item.m_unity;
        }

          break;
        case ConsumptionTableIndex::ItemID:
        {
          Item item;
          item.m_id = value.toInt();
          QString error;
          bool bSuccess = ItemSQL::select(database(),
                                               item,
                                               error);
          value = bSuccess ? item.m_description : error;
        } break;
        case ConsumptionTableIndex::Total:
        {
          double total = record(index.row()).value("_PRICE").toDouble() *
                         record(index.row()).value("_AMMOUNT").toDouble();
          value = "R$ " + QString::number(total, 'f', 2);
        } break;
        default:
          break;
      }
    }

    return value;
  }
};

ConsumptionDatabase::ConsumptionDatabase(QWidget *parent)
  : QFrame(parent)
  , m_openFilter(nullptr)
  , m_refresh(nullptr)
  , m_remove(nullptr)
  , m_table(nullptr)
  , m_total(nullptr)
  , m_chart(nullptr)
{
  m_openFilter = new QPushButton();
  m_openFilter->setFlat(true);
  m_openFilter->setText("");
  m_openFilter->setIconSize(QSize(24, 24));
  m_openFilter->setIcon(QIcon(":/icons/res/filter.png"));

  m_refresh = new QPushButton();
  m_refresh->setFlat(true);
  m_refresh->setText("");
  m_refresh->setIconSize(QSize(24, 24));
  m_refresh->setIcon(QIcon(":/icons/res/refresh.png"));

  m_remove = new QPushButton();
  m_remove->setFlat(true);
  m_remove->setText("");
  m_remove->setIconSize(QSize(24, 24));
  m_remove->setIcon(QIcon(":/icons/res/remove.png"));

  m_total = new QLineEdit();
  m_total->setAlignment(Qt::AlignRight);
  m_total->setPlaceholderText("Consumo Total");
  m_total->setReadOnly(true);
  QPalette palette = m_total->palette();
  palette.setColor(QPalette::Text, Qt::red);
  m_total->setPalette(palette);
  QFont font = m_total->font();
  font.setBold(true);
  m_total->setFont(font);

  m_chart = new QPushButton();
  m_chart->setFlat(true);
  m_chart->setText("");
  m_chart->setIconSize(QSize(24, 24));
  m_chart->setIcon(QIcon(":/icons/res/chart.png"));
  m_chart->setDefault(true);

  QHBoxLayout* hlayout = new QHBoxLayout();
  hlayout->addWidget(m_openFilter);
  hlayout->addWidget(m_refresh);
  hlayout->addWidget(m_remove);
  hlayout->addStretch();
  hlayout->addWidget(m_total);
  hlayout->addWidget(m_chart);
  hlayout->setContentsMargins(0, 0, 0, 0);

  m_table = new QTableView();
  m_table->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
  m_table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
  m_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
  m_table->horizontalHeader()->setHighlightSections(false);
  m_table->setSortingEnabled(true);

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->setContentsMargins(0, 0, 0, 0);
  vlayout->addLayout(hlayout);
  vlayout->addWidget(m_table);

  setLayout(vlayout);

  QObject::connect(m_remove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(remove()));

  QObject::connect(m_openFilter,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitFilterSignal()));

  QObject::connect(m_refresh,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(refresh()));

  QObject::connect(m_chart,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitChartSignal()));
}

ConsumptionDatabase::~ConsumptionDatabase()
{

}

void ConsumptionDatabase::setDatabase(QSqlDatabase db)
{
  if (m_table->model() != nullptr)
    return;

  ConsumptionTableModel* model = new ConsumptionTableModel(this, db);
  model->setTable("_CONSUMPTION");
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);

  setColumnText(model, ConsumptionTableIndex::ID);
  setColumnText(model, ConsumptionTableIndex::Date);
  setColumnText(model, ConsumptionTableIndex::ItemID);
  setColumnText(model, ConsumptionTableIndex::Price);
  setColumnText(model, ConsumptionTableIndex::Ammount);
  setColumnText(model, ConsumptionTableIndex::Total);

  m_table->setModel(model);
  m_table->hideColumn((int)ConsumptionTableIndex::ID);
  m_table->horizontalHeader()->setSectionResizeMode((int)ConsumptionTableIndex::Date,
                                                    QHeaderView::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)ConsumptionTableIndex::ItemID,
                                                    QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode((int)ConsumptionTableIndex::Price,
                                                    QHeaderView::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)ConsumptionTableIndex::Ammount,
                                                    QHeaderView::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)ConsumptionTableIndex::Total,
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
  setFilter(Consumption::Filter());
  refresh();
}

void ConsumptionDatabase::enableControls()
{
  bool bSelected = m_table->currentIndex().isValid();
  m_remove->setEnabled(bSelected);
}

void ConsumptionDatabase::insert(const Consumption& consumption)
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    QSqlRecord rec = model->record();
    rec.setValue("_ITEMID", consumption.m_itemID);
    rec.setValue("_DATE", consumption.m_date);
    rec.setValue("_PRICE", consumption.m_price);
    rec.setValue("_AMMOUNT", consumption.m_ammount);
    rec.setValue("_TOTAL", consumption.m_total);
    model->insertRecord(-1, rec);
    model->submitAll();
    updateTotal();
  }
}

void ConsumptionDatabase::remove()
{
  if (m_table->currentIndex().isValid())
  {
    int row = m_table->currentIndex().row();
    if (m_table->model() != nullptr)
    {
      if (QMessageBox::question(this,
                                tr("Remover Consumo"),
                                tr("Tem certeza que deseja remover o consumo selecionado?"),
                                QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
      {
        QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
        model->removeRow(row);
        model->submitAll();
      }
    }
    updateTotal();
  }
  enableControls();
}

void ConsumptionDatabase::refresh()
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    model->select();
    updateTotal();
  }
  enableControls();
}

void ConsumptionDatabase::setFilter(const Consumption::Filter& filter)
{
  if (m_table->model() != nullptr)
  {
    m_filter = filter;
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    QString filter;
    if (m_filter.m_bDate)
    {
      filter = Consumption::columnName(ConsumptionTableIndex::Date) +
               " BETWEEN " +
               QString::number(m_filter.m_datei) +
               " AND " +
               QString::number(m_filter.m_datef);
    }
    model->setFilter(filter);
    updateTotal();
  }
}

void ConsumptionDatabase::emitChartSignal()
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    QVector<double> vSubTotal;
    QVector<qint64> vDate;
    QString error;
    ConsumptionSQL::selectSubTotal(model->database(),
                                   m_filter,
                                   vDate,
                                   vSubTotal,
                                   error);
    emit chartSignal(vDate, vSubTotal);
  }
}

void ConsumptionDatabase::emitFilterSignal()
{
  emit filterSignal();
}

void ConsumptionDatabase::updateTotal()
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    double total = ConsumptionSQL::getTotal(model->database(), m_filter);
    m_total->setText("R$ " + QString::number(total, 'f', 2));
  }
}

bool ConsumptionDatabase::isValid() const
{
  if (m_table->model() != nullptr)
    return m_table->model()->rowCount() != 0;
  return false;
}

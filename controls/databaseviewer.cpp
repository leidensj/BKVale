#include "databaseviewer.h"
#include "widgets/jlineedit.h"
#include "defines.h"
#include "pincodedialog.h"
#include "items/jitemhelper.h"
#include "models/jmodel.h"
#include "widgets/filegenerator.h"
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
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QFileDialog>

JEnterSignalTable::JEnterSignalTable(QWidget *parent)
  : QTableView(parent)
{

}

void JEnterSignalTable::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Enter ||
      event->key() == Qt::Key_Return)
  {
    emit enterKeyPressedSignal();
  }
  QTableView::keyPressEvent(event);
}

DatabaseViewer::DatabaseViewer(const QString& tableName,
                               Mode mode,
                               QWidget *parent)
  : QWidget(parent)
  , m_mode(mode)
  , m_tableName(tableName)
  , m_btnOpen(nullptr)
  , m_btnRefresh(nullptr)
  , m_btnRemove(nullptr)
  , m_btnCopy(nullptr)
  , m_btnCSV(nullptr)
  , m_ltButton(nullptr)
  , m_edSearch(nullptr)
  , m_cbContains(nullptr)
  , m_table(nullptr)
  , m_proxyModel(nullptr)
{
  m_btnOpen = new QPushButton;
  m_btnOpen->setFlat(true);
  m_btnOpen->setIconSize(QSize(24, 24));
  m_btnOpen->setIcon(QIcon(":/icons/res/open.png"));
  m_btnOpen->setToolTip(tr("Abrir (Ctrl+O)"));
  m_btnOpen->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));

  m_btnRemove = new QPushButton;
  m_btnRemove->setFlat(true);
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setToolTip(tr("Remover (Del)"));
  m_btnRemove->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::Key_Delete));

  m_btnRefresh = new QPushButton;
  m_btnRefresh->setFlat(true);
  m_btnRefresh->setIconSize(QSize(16, 16));
  m_btnRefresh->setIcon(QIcon(":/icons/res/refresh.png"));
  m_btnRefresh->setToolTip(tr("Atualizar (F5)"));
  m_btnRefresh->setShortcut(QKeySequence(Qt::Key_F5));

  m_btnCopy = new QPushButton;
  m_btnCopy->setFlat(true);
  m_btnCopy->setIconSize(QSize(24, 24));
  m_btnCopy->setToolTip(tr("Copiar (Ctrl+C)"));
  m_btnCopy->setIcon(QIcon(":/icons/res/copy.png"));
  m_btnCopy->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));

  m_btnCSV = new QPushButton;
  m_btnCSV->setFlat(true);
  m_btnCSV->setIconSize(QSize(24, 24));
  m_btnCSV->setToolTip(tr("Exportar para CSV"));
  m_btnCSV->setIcon(QIcon(":/icons/res/csv_export.png"));

  m_ltButton = new QHBoxLayout;
  m_ltButton->setContentsMargins(0, 0, 0, 0);
  m_ltButton->setAlignment(Qt::AlignLeft);
  m_ltButton->addWidget(m_btnOpen);
  m_ltButton->addWidget(m_btnRemove);
  m_ltButton->addWidget(m_btnCopy);
  m_ltButton->addWidget(m_btnCSV);

  m_edSearch = new JLineEdit(Text::Input::All, true);
  m_edSearch->setArrowsAndEnterAsTab(false);
  m_edSearch->setToolTip(tr("Procurar (Ctrl+F)"));
  m_edSearch->setClearButtonEnabled(true);

  m_cbContains = new QCheckBox;
  m_cbContains->setText(tr("Contendo"));
  m_cbContains->setIconSize(QSize(16, 16));
  m_cbContains->setIcon(QIcon(":/icons/res/center.png"));
  m_cbContains->setToolTip(tr("Procurar por palavras contendo o termo informado"));

  QHBoxLayout* hlayout1 = new QHBoxLayout;
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->addWidget(m_btnRefresh);
  hlayout1->addWidget(m_edSearch);
  hlayout1->addWidget(m_cbContains);

  m_table = new JEnterSignalTable;
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setSelectionMode(m_mode == Mode::Edit  ? QAbstractItemView::ExtendedSelection
                                                  : m_mode == Mode::MultiSelector ? QAbstractItemView::MultiSelection
                                                                                  : QAbstractItemView::SingleSelection);
  m_table->setEditTriggers(QTableView::NoEditTriggers);
  m_table->setSortingEnabled(true);
  m_table->horizontalHeader()->setHighlightSections(false);
  m_table->verticalHeader()->hide();

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->addLayout(m_ltButton);
  vlayout1->addLayout(hlayout1);
  vlayout1->addWidget(m_table);
  setLayout(vlayout1);

  JModel* model = JItemHelper::model(tableName, this);
  if (model == nullptr)
  {
    QMessageBox::critical(this, tr("Erro"), tr("Modelo não implementado"), QMessageBox::Ok);
    exit(EXIT_FAILURE);
  }

  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setSourceModel(model);
  m_proxyModel->setSortRole(Qt::DisplayRole);

  m_table->setModel(m_proxyModel);
  model->select(m_table->horizontalHeader());

  connect(m_table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(enableControls()));
  connect(m_table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(emitCurrentRowChangedSignal()));
  connect(m_btnRefresh, SIGNAL(clicked(bool)), this, SLOT(refresh()));
  connect(m_edSearch, SIGNAL(textEdited(const QString&)), this, SLOT(searchChanged()));
  connect(m_edSearch, SIGNAL(enterSignal()), this, SLOT(searchEnter()));
  connect(m_edSearch, SIGNAL(keyDownSignal()), this, SLOT(searchEnter()));
  connect(m_cbContains, SIGNAL(clicked(bool)), this, SLOT(containsPressed()));
  connect(m_table->horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(searchChanged()));
  connect(m_btnCSV, SIGNAL(clicked(bool)), this, SLOT(toCSV()));

  if (m_mode != Mode::ReadOnly)
  {
    connect(m_table, SIGNAL(enterKeyPressedSignal()), this, SLOT(emitItemsSelectedSignal()));
    connect(m_btnOpen, SIGNAL(clicked(bool)), this, SLOT(emitItemsSelectedSignal()));
    connect(m_table, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(emitItemsSelectedSignal()));
    connect(m_btnRemove, SIGNAL(clicked(bool)), this, SLOT(removeItems()));
    connect(m_btnCopy, SIGNAL(clicked(bool)), this, SLOT(emitCopySignal()));
  }

  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this, SLOT(focusSearch()));

  if (m_table->horizontalHeader()->count() > 1)
    m_table->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);

  if (m_mode != Mode::Edit)
  {
    m_btnOpen->hide();
    m_btnRemove->setEnabled(false);
    m_btnRemove->hide();
    m_btnCopy->setEnabled(false);
    m_btnCopy->hide();
    m_edSearch->setFocus();
    m_btnCSV->hide();
  }
  searchChanged();
}

DatabaseViewer::~DatabaseViewer()
{

}

void DatabaseViewer::setSortRole(int role)
{
  m_proxyModel->setSortRole(role);
}

Id DatabaseViewer::getFirstSelectedId() const
{
  QModelIndexList lst = m_table->selectionModel()->selectedRows();
  Id id;
  if (lst.size() != 0)
  {
    QModelIndex idx = m_proxyModel->mapToSource(lst.at(0));
    if (idx.isValid())
    {
      JModel* model = dynamic_cast<JModel*>(m_proxyModel->sourceModel());
      id = Id(model->index(idx.row(), SQL_COLID_NUMBER).data(Qt::EditRole).toLongLong());
    }
  }
  return id;
}

void DatabaseViewer::selectId(const Id& id)
{
  Ids ids;
  ids.push_back(id);
  selectIds(ids);
}

void DatabaseViewer::selectIds(const Ids& ids)
{
  m_table->clearSelection();
  for (auto id : ids)
  {
    QModelIndexList lst = m_proxyModel->match(m_proxyModel->index(0, 0), Qt::EditRole, id.get(), 1, Qt::MatchExactly);
    if (lst.size() != 0)
      m_table->selectRow(lst.at(0).row());
  }

  if (ids.size() != 0)
    m_table->setFocus();
}

Ids DatabaseViewer::getSelectedIds() const
{
  JModel* model = dynamic_cast<JModel*>(m_proxyModel->sourceModel());
  QModelIndexList lst = m_table->selectionModel()->selectedRows();
  Ids ids;
  for (auto i : lst)
  {
    QModelIndex idx = m_proxyModel->mapToSource(i);
    if (idx.isValid())
      ids.push_back(Id(model->index(idx.row(), SQL_COLID_NUMBER).data(Qt::EditRole).toLongLong()));
  }
  return ids;
}

void DatabaseViewer::refresh()
{
  JModel* model = dynamic_cast<JModel*>(m_proxyModel->sourceModel());
  if (m_fixedFilter.isEmpty() && m_dynamicFilter.isEmpty())
    model->select();
  else
  {
    QString str;
    if (!m_fixedFilter.isEmpty())
    {
      if (!m_dynamicFilter.isEmpty())
        str = m_fixedFilter + " AND " + m_dynamicFilter;
      else
        str = m_fixedFilter;
    }
    else
      str = m_dynamicFilter;
    model->selectFilter(str);
  }

  m_proxyModel->invalidate();
  enableControls();
  if (m_mode != Mode::Edit)
    m_edSearch->setFocus();

  emit refreshSignal();
}

void DatabaseViewer::enableControls()
{
  bool bSelected = m_table->currentIndex().isValid();
  m_btnOpen->setEnabled(bSelected);
  m_btnRemove->setEnabled(bSelected);
  m_btnCopy->setEnabled(bSelected);
  m_btnCSV->setEnabled(m_proxyModel->rowCount() != 0);
}

void DatabaseViewer::removeItems()
{
  Ids ids = getSelectedIds();
  JItemHelper::remove(ids, m_tableName, this);
  emit itemsRemovedSignal(ids);
  refresh();
}

void DatabaseViewer::searchChanged()
{
  int column = m_table->horizontalHeader()->sortIndicatorSection();
  m_proxyModel->setFilterKeyColumn(column);
  QString search = m_edSearch->text();
  if (!m_cbContains->isChecked() && !search.isEmpty())
    search.prepend("\\b");
  QRegularExpression regExp(search, QRegularExpression::CaseInsensitiveOption);
  m_proxyModel->setFilterRegularExpression(regExp);

  if (search.isEmpty())
  {
    QString columnName;
    if (column > 0)
      columnName = m_proxyModel->headerData(column, Qt::Horizontal).toString().toLower();
    columnName = tr("Procurar por: ") + columnName;
    m_edSearch->setPlaceholderText(columnName);
  }

  enableControls();
  emit refreshSignal();
}

void DatabaseViewer::searchEnter()
{
  m_table->setFocus();
  if (m_proxyModel->rowCount() != 0)
  {
    m_table->selectRow(0);
  }
  else
  {
    m_edSearch->setFocus();
    m_edSearch->selectAll();
  }
}

void DatabaseViewer::clearSearch()
{
  m_edSearch->clear();
  searchChanged();
}

void DatabaseViewer::containsPressed()
{
  searchChanged();
  m_edSearch->setFocus();
}

void DatabaseViewer::focusSearch()
{
  m_edSearch->selectAll();
  m_edSearch->setFocus();
}

void DatabaseViewer::setFixedFilter(const QString& fixedFilter)
{
  m_fixedFilter = fixedFilter;
  refresh();
}

void DatabaseViewer::setDynamicFilter(const QString& dynamicFilter)
{
  m_dynamicFilter = dynamicFilter;
  refresh();
}

QString DatabaseViewer::getTableName() const
{
  return m_tableName;
}

void DatabaseViewer::emitItemsSelectedSignal()
{
  emit itemsSelectedSignal();
}

void DatabaseViewer::emitCurrentRowChangedSignal()
{
  emit currentRowChangedSignal(m_table->currentIndex().row());
}

int DatabaseViewer::getRowCount() const
{
  return m_proxyModel->rowCount();
}

double DatabaseViewer::getSum(int column) const
{
  double sum = 0.0;
  for (int row = 0; row != m_proxyModel->rowCount(); ++row)
    sum += m_proxyModel->data(m_proxyModel->index(row, column), Qt::EditRole).toDouble();
  return sum;
}

QPushButton* DatabaseViewer::addButton(const QString& toolTip, const QIcon& icon, int shortcut)
{
  QPushButton* btn = new QPushButton;
  btn->setFlat(true);
  btn->setToolTip(toolTip);
  btn->setIconSize(QSize(24, 24));
  btn->setIcon(icon);
  if (shortcut)
    btn->setShortcut(QKeySequence(shortcut));
  m_ltButton->addWidget(btn);
  return btn;
}

void DatabaseViewer::toCSV()
{
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Salvar arquivo"),
                                                  JItemHelper::text(getTableName()) +
                                                  DateTime::server().toString("dd-MM-yyyy hh-mm-ss") +
                                                  ".csv",
                                                  tr("CSV (*.csv)"));

  if (fileName.isEmpty())
    return;

  QString csv, line;
  for (int j = 0; j != m_proxyModel->columnCount(); ++j)
    if (!m_table->horizontalHeader()->isSectionHidden(j))
      line += m_proxyModel->headerData(j, Qt::Horizontal).toString() + ";";
  if (!line.isEmpty())
  {
    line.chop(1);
    csv += line + "\n";
  }
  for (int i = 0; i != m_proxyModel->rowCount(); ++i)
  {
    line.clear();
    for (int j = 0; j != m_proxyModel->columnCount(); ++j)
      if (!m_table->horizontalHeader()->isSectionHidden(j))
        line += m_proxyModel->data(m_proxyModel->index(i, j), Qt::DisplayRole).toString() + ";";
    if (!line.isEmpty())
    {
      line.chop(1);
      line.replace("\n", "\t");
      csv += line + "\n";
    }
  }

  CsvGenerator* w = new CsvGenerator(fileName, csv, tr("Gerando arquivo csv:"), true);
  w->generate();
}

void DatabaseViewer::emitCopySignal()
{
  emit copySignal();
}

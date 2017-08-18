#include "itemwidget.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QKeyEvent>
#include <QShortcut>
#include <QLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QTableView>
#include <QPushButton>
#include <QDebug>

namespace
{
QString friendlyColumnName(ItemTableIndex idx)
{
  switch (idx)
  {
    case ItemTableIndex::ID:
      return QObject::tr("ID"); break;
    case ItemTableIndex::Description:
      return QObject::tr("Descrição"); break;
    case ItemTableIndex::Unity:
      return QObject::tr("Unidade"); break;
    case ItemTableIndex::Supplier:
      return QObject::tr("Fornecedor"); break;
    case ItemTableIndex::Price:
      return QObject::tr("Preço"); break;
    case ItemTableIndex::Details:
      return QObject::tr("Detalhes"); break;
    case ItemTableIndex::MidasCode:
      return QObject::tr("Código Midas"); break;
    case ItemTableIndex::Icon:
      return QObject::tr("Ícone"); break;
    default:
      return "";
  }
}

QString columnIcon(ItemTableIndex idx)
{
  switch (idx)
  {
    case ItemTableIndex::Description:
      return ":/icons/res/description.png";
    case ItemTableIndex::Unity:
      return ":/icons/res/unity.png";
    case ItemTableIndex::Supplier:
      return ":/icons/res/supplier.png";
    case ItemTableIndex::Price:
      return ":/icons/res/price.png";
    case ItemTableIndex::Details:
      return ":/icons/res/details.png";
    case ItemTableIndex::MidasCode:
    case ItemTableIndex::Icon:
    case ItemTableIndex::ID:
    default:
      return "";
  }
}

void setColumnText(QSqlTableModel* model,
                   ItemTableIndex idx)
{
  if (model != nullptr)
  {
    model->setHeaderData((int)idx,
                         Qt::Horizontal,
                         friendlyColumnName(idx));
  }
}

void setColumnIcon(QSqlTableModel* model,
                   ItemTableIndex idx)
{
  if (model != nullptr)
  {
    model->setHeaderData((int)idx,
                         Qt::Horizontal,
                         QVariant::fromValue(QIcon(columnIcon(idx))),
                         Qt::DecorationRole);
  }
}

QString filterPlaceholder(ItemTableIndex idx)
{
  QString column = friendlyColumnName(idx);
  if (!column.isEmpty())
    column[0].toLower();
  return QObject::tr("Procurar pelo(a) ") +
      column +
      "...";
}

QString buildFilter(QString text,
                    ItemTableIndex idx,
                    bool bContains)
{
  QString filter;
  if (!text.isEmpty())
  {
    QString column = Item::columnName(idx);
    filter = column + " LIKE '";
    if (bContains)
      filter += "%";
    filter += text + "%'";
  }
  return filter;
}
}

ItemTableView::ItemTableView(bool bEditMode, QWidget *parent)
  : QTableView(parent)
{
  setSelectionBehavior(QAbstractItemView::SelectItems);
  setSelectionMode(QAbstractItemView::SingleSelection);
  {
    QFont f = font();
    f.setCapitalization(QFont::AllUppercase);
    f.setPointSize(12);
    setFont(f);
  }
  {
    QFont f = horizontalHeader()->font();
    f.setCapitalization(QFont::Capitalize);
    f.setPointSize(12);
    horizontalHeader()->setFont(f);
  }

  if (!bEditMode)
  {
    setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    horizontalHeader()->setHighlightSections(false);
  }

  QObject::connect(this,
                   SIGNAL(doubleClicked(const QModelIndex&)),
                   this,
                   SIGNAL(enterKeyPressedSignal()));
}

void ItemTableView::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Enter ||
      event->key() == Qt::Key_Return)
    emit enterKeyPressedSignal();
  else
    QTableView::keyPressEvent(event);
}

FilterLineEdit::FilterLineEdit()
{
  QFont f = font();
  f.setPointSize(12);
  setFont(f);
  setClearButtonEnabled(true);

  QObject::connect(this,
                   SIGNAL(textEdited(const QString&)),
                   this,
                   SLOT(toUpper()));
}

void FilterLineEdit::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Down)
    emit downArrowPressedSignal();
  else if (event->key() == Qt::Key_Enter ||
           event->key() == Qt::Key_Return)
    emit enterKeyPressedSignal();
  else
    QLineEdit::keyPressEvent(event);
}

void FilterLineEdit::toUpper()
{
  setText(text().toUpper());
  emit changedSignal();
}

ItemWidget::ItemWidget(bool bEditMode, QWidget *parent) :
  QFrame(parent),
  m_filter(nullptr),
  m_contains(nullptr),
  m_create(nullptr),
  m_save(nullptr),
  m_discard(nullptr),
  m_refresh(nullptr),
  m_remove(nullptr),
  m_buttons(nullptr),
  m_bEditMode(bEditMode)
{
  QHBoxLayout* buttonLayout = new QHBoxLayout();
  buttonLayout->setContentsMargins(0, 0, 0, 0);
  m_create = new QPushButton();
  m_create->setFlat(true);
  m_create->setText("");
  m_create->setIconSize(QSize(24, 24));
  m_create->setIcon(QIcon(":/icons/res/newitem.png"));
  m_save = new QPushButton();
  m_save->setFlat(true);
  m_save->setText("");
  m_save->setIconSize(QSize(24, 24));
  m_save->setIcon(QIcon(":/icons/res/save.png"));
  m_discard = new QPushButton();
  m_discard->setFlat(true);
  m_discard->setText("");
  m_discard->setIconSize(QSize(24, 24));
  m_discard->setIcon(QIcon(":/icons/res/revert.png"));
  m_refresh = new QPushButton();
  m_refresh->setFlat(true);
  m_refresh->setText("");
  m_refresh->setIconSize(QSize(24, 24));
  m_refresh->setIcon(QIcon(":/icons/res/refresh.png"));
  m_remove = new QPushButton();
  m_remove->setFlat(true);
  m_remove->setText("");
  m_remove->setIconSize(QSize(24, 24));
  m_remove->setIcon(QIcon(":/icons/res/trash.png"));
  buttonLayout->addWidget(m_create);
  buttonLayout->addWidget(m_save);
  buttonLayout->addWidget(m_discard);
  buttonLayout->addWidget(m_refresh);
  buttonLayout->addWidget(m_remove);
  buttonLayout->setAlignment(Qt::AlignLeft);
  m_buttons = new QFrame();
  m_buttons->setLayout(buttonLayout);

  QHBoxLayout* filterLayout = new QHBoxLayout();
  filterLayout->setContentsMargins(0, 0, 0, 0);
  m_filter = new FilterLineEdit();
  m_contains = new QCheckBox();
  m_contains->setText(tr("Contendo"));
  filterLayout->addWidget(m_filter);
  filterLayout->addWidget(m_contains);

  m_table = new ItemTableView(bEditMode);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(m_buttons);
  layout->addLayout(filterLayout);
  layout->addWidget(m_table);
  setLayout(layout);

  if (!bEditMode)
  {
    m_buttons->setEnabled(false);
    m_buttons->setVisible(false);
  }

  QObject::connect(m_remove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeSelectedItem()));

  QObject::connect(m_refresh,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(refresh()));

  QObject::connect(m_save,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save(bool)));

  QObject::connect(m_discard,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(discard(bool)));

  QObject::connect(m_create,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));

  QObject::connect(m_filter,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(setFilter()));

  QObject::connect(m_filter,
                   SIGNAL(downArrowPressedSignal()),
                   this,
                   SLOT(filterDownArrowPressed()));

  QObject::connect(m_filter,
                   SIGNAL(enterKeyPressedSignal()),
                   this,
                   SLOT(filterEnterKeyPressed()));

  QObject::connect(m_contains,
                   SIGNAL(stateChanged(int)),
                   this,
                   SLOT(containsPressed()));

  QObject::connect(m_table->horizontalHeader(),
                   SIGNAL(sortIndicatorChanged(int,
                                               Qt::SortOrder)),
                   this,
                   SLOT(refresh()));

  QObject::connect(m_table,
                   SIGNAL(enterKeyPressedSignal()),
                   this,
                   SLOT(itemSelected()));

  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F),
                this,
                SLOT(focusFilter()));
}

ItemWidget::~ItemWidget()
{

}

void ItemWidget::setDatabase(QSqlDatabase db)
{
  if (m_table->model() != nullptr)
    return;

  QSqlTableModel* model = new QSqlTableModel(this, db);
  model->setTable("_ITEMS");
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);

  setColumnText(model, ItemTableIndex::ID);
  setColumnText(model, ItemTableIndex::Description);
  setColumnText(model, ItemTableIndex::Unity);
  setColumnText(model, ItemTableIndex::Supplier);
  setColumnText(model, ItemTableIndex::Price);
  setColumnText(model, ItemTableIndex::Details);
  setColumnText(model, ItemTableIndex::MidasCode);
  setColumnText(model, ItemTableIndex::Icon);

  //setColumnIcon(model, ItemTableIndex::Description);
  //setColumnIcon(model, ItemTableIndex::Unity);
  //setColumnIcon(model, ItemTableIndex::Supplier);
  //setColumnIcon(model, ItemTableIndex::Price);
  //setColumnIcon(model, ItemTableIndex::Details);

  m_table->setModel(model);
  m_table->hideColumn((int)ItemTableIndex::ID);
  m_table->hideColumn((int)ItemTableIndex::MidasCode);
  m_table->hideColumn((int)ItemTableIndex::Icon);
  m_table->horizontalHeader()->setSortIndicator((int)ItemTableIndex::Description,
                                                  Qt::SortOrder::AscendingOrder);
  m_table->horizontalHeader()->setSectionResizeMode((int)ItemTableIndex::Description,
                                                      QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode((int)ItemTableIndex::Unity,
                                                      QHeaderView::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)ItemTableIndex::Supplier,
                                                      QHeaderView::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)ItemTableIndex::Price,
                                                      QHeaderView::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)ItemTableIndex::Price,
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
  refresh();
  focusFilter();
}

void ItemWidget::refresh()
{
  if (m_table->model() != nullptr)
  {
    confirm();
    auto idx = currentSortIndicator();
    m_filter->clear();
    m_filter->setPlaceholderText(filterPlaceholder(idx));
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    model->setFilter("");
    model->select();
  }
  enableControls();
}

void ItemWidget::enableControls()
{
  bool bSelected = m_table->currentIndex().isValid();
  m_remove->setEnabled(bSelected);
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    m_save->setEnabled(model->isDirty());
    m_discard->setEnabled(model->isDirty());
  }
}

void ItemWidget::removeSelectedItem()
{
  if (m_table->currentIndex().isValid())
  {
    if (m_table->model() != nullptr)
    {
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
      model->removeRow(m_table->currentIndex().row());
    }
  }
  enableControls();
}

void ItemWidget::save(bool bSkipConfirmation)
{
  if (m_table->model() != nullptr)
  {
    int ret = !bSkipConfirmation ? QMessageBox::question(this,
                                                         tr("Salvar mudanças"),
                                                         tr("Tem certeza que deseja salvar as mudanças realizadas?"),
                                                         QMessageBox::Ok | QMessageBox::Cancel)
                                 : QMessageBox::Ok;
    if (ret == QMessageBox::Ok)
    {
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
      model->submitAll();
    }
  }
}

void ItemWidget::discard(bool bSkipConfirmation)
{
  if (m_table->model() != nullptr)
  {
    int ret = !bSkipConfirmation ? QMessageBox::question(this,
                                                        tr("Descartar mudanças"),
                                                        tr("Tem certeza que deseja descartar as mudanças realizadas?"),
                                                        QMessageBox::Ok | QMessageBox::Cancel)
                                : QMessageBox::Ok;
    if (ret == QMessageBox::Ok)
    {
      m_filter->clear();
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
      model->setFilter("");
      model->revertAll();
      refresh();
    }
  }
}

void ItemWidget::create()
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    model->insertRow(model->rowCount());
  }
  enableControls();
}

void ItemWidget::setFilter()
{
  if (m_table->model() != nullptr)
  {
    confirm();
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    auto idx = currentSortIndicator();
    if (m_filter->text().isEmpty())
      m_filter->setPlaceholderText(filterPlaceholder(idx));
    QString filter(buildFilter(m_filter->text(),
                               idx,
                               m_contains->isChecked()));
    model->setFilter(filter);
  }
}

void ItemWidget::containsPressed()
{
  setFilter();
  m_filter->setFocus();
}

void ItemWidget::focusFilter()
{
  m_filter->setFocus();
  m_filter->selectAll();
}

void ItemWidget::confirm()
{
  QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
  if (model->isDirty())
  {
    int ret = QMessageBox::question(this,
                                    tr("O cadastro de produtos foi modificado"),
                                    tr("Deseja salvar as mudaças?"),
                                    QMessageBox::Save | QMessageBox::Discard);
    switch(ret)
    {
      case QMessageBox::Save:
        save(true);
        break;
      case QMessageBox::Discard:
      default:
        discard(true);
    }
  }
}

ItemTableIndex ItemWidget::currentSortIndicator() const
{
  int idx = m_table->horizontalHeader()->sortIndicatorSection();
  return (ItemTableIndex)idx;
}

Item ItemWidget::item() const
{
  Item item;
  if (m_table->currentIndex().isValid())
  {
    if (m_table->model() != nullptr)
    {
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
      QSqlRecord rec = model->record(m_table->currentIndex().row());
      item.m_id = rec.value((int)ItemTableIndex::ID).toInt();
      item.m_description = rec.value((int)ItemTableIndex::Description).toString();
      item.m_unity = rec.value((int)ItemTableIndex::Unity).toString();
      item.m_supplier = rec.value((int)ItemTableIndex::Supplier).toString();
      item.m_price = rec.value((int)ItemTableIndex::Price).toDouble();
      item.m_details = rec.value((int)ItemTableIndex::ID).toString();
      item.m_midasCode = rec.value((int)ItemTableIndex::MidasCode).toString();
      item.m_icon = rec.value((int)ItemTableIndex::Icon).toInt();
    }
  }
  return item;
}

void ItemWidget::filterDownArrowPressed()
{
  m_table->setFocus();
  if (m_table->model() != nullptr)
  {
    if (m_table->model()->rowCount() != 0)
    {
      m_table->selectRow(0);
    }
  }
}

void ItemWidget::filterEnterKeyPressed()
{
  m_table->setFocus();
  if (m_table->model() != nullptr &&
      m_table->model()->rowCount() != 0)
  {
    m_table->selectRow(0);
  }
  else
  {
    focusFilter();
  }
}

void ItemWidget::itemSelected()
{
  if (m_table->currentIndex().isValid())
  {
    Item p = item();
    if (p.isValid())
    {
      emit itemSelectedSignal(p);
      if (parentWidget() != nullptr)
        parentWidget()->close();
    }
  }
}

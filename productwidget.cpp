#include "productwidget.h"
#include "ui_productwidget.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QKeyEvent>
#include <QShortcut>

namespace
{
QString friendlyColumnName(ProductTableIndex idx)
{
  switch (idx)
  {
    case ProductTableIndex::ID:
      return QObject::tr("ID"); break;
    case ProductTableIndex::Description:
      return QObject::tr("Descrição"); break;
    case ProductTableIndex::Unity:
      return QObject::tr("Unidade"); break;
    case ProductTableIndex::Supplier:
      return QObject::tr("Fornecedor"); break;
    case ProductTableIndex::Price:
      return QObject::tr("Preço"); break;
    case ProductTableIndex::Details:
      return QObject::tr("Detalhes"); break;
    case ProductTableIndex::MidasCode:
      return QObject::tr("Código Midas"); break;
    case ProductTableIndex::Icon:
      return QObject::tr("Ícone"); break;
    default:
      return "";
  }
}

QString columnIcon(ProductTableIndex idx)
{
  switch (idx)
  {
    case ProductTableIndex::Description:
      return ":/icons/res/description.png";
    case ProductTableIndex::Unity:
      return ":/icons/res/unity.png";
    case ProductTableIndex::Supplier:
      return ":/icons/res/supplier.png";
    case ProductTableIndex::Price:
      return ":/icons/res/price.png";
    case ProductTableIndex::Details:
      return ":/icons/res/details.png";
    case ProductTableIndex::MidasCode:
    case ProductTableIndex::Icon:
    case ProductTableIndex::ID:
    default:
      return "";
  }
}

void setColumnText(QSqlTableModel* model,
                   ProductTableIndex idx)
{
  if (model != nullptr)
  {
    model->setHeaderData((int)idx,
                         Qt::Horizontal,
                         friendlyColumnName(idx));
  }
}

void setColumnIcon(QSqlTableModel* model,
                   ProductTableIndex idx)
{
  if (model != nullptr)
  {
    model->setHeaderData((int)idx,
                         Qt::Horizontal,
                         QVariant::fromValue(QIcon(columnIcon(idx))),
                         Qt::DecorationRole);
  }
}

QString filterPlaceholder(ProductTableIndex idx)
{
  QString column = friendlyColumnName(idx);
  if (!column.isEmpty())
    column[0].toLower();
  return QObject::tr("Procurar pelo(a) ") +
      column +
      "...";
}

QString buildFilter(QString text,
                    ProductTableIndex idx,
                    bool bContains)
{
  QString filter;
  if (!text.isEmpty())
  {
    QString column = Product::columnName(idx);
    filter = column + " LIKE '";
    if (bContains)
      filter += "%";
    filter += text + "%'";
  }
  return filter;
}
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
  emit filterChangedSignal();
}

ProductWidget::ProductWidget(bool bEditMode, QWidget *parent) :
  QFrame(parent),
  ui(new Ui::ProductWidget),
  m_bEditMode(bEditMode)
{
  ui->setupUi(this);
  ui->table->setSelectionBehavior(QAbstractItemView::SelectItems);
  ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
  {
    QFont f = ui->table->font();
    f.setCapitalization(QFont::AllUppercase);
    ui->table->setFont(f);
  }
  {
    QFont f = ui->table->horizontalHeader()->font();
    f.setCapitalization(QFont::Capitalize);
    ui->table->horizontalHeader()->setFont(f);
  }

  ui->filterLayout->insertWidget(0, &m_filter);

  if (!bEditMode)
  {
    ui->cmdFrame->setEnabled(false);
    ui->cmdFrame->setVisible(false);
    ui->table->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    ui->table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->table->horizontalHeader()->setHighlightSections(false);
  }

  QObject::connect(ui->buttonRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeSelectedProduct()));

  QObject::connect(ui->buttonRefresh,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(refresh()));

  QObject::connect(ui->buttonSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save(bool)));

  QObject::connect(ui->buttonDiscard,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(discard(bool)));

  QObject::connect(ui->buttonCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));

  QObject::connect(&m_filter,
                   SIGNAL(filterChangedSignal()),
                   this,
                   SLOT(setFilter()));

  QObject::connect(&m_filter,
                   SIGNAL(downArrowPressedSignal()),
                   this,
                   SLOT(filterDownArrowPressed()));

  QObject::connect(&m_filter,
                   SIGNAL(enterKeyPressedSignal()),
                   this,
                   SLOT(filterEnterKeyPressed()));

  QObject::connect(ui->buttonContains,
                   SIGNAL(stateChanged(int)),
                   this,
                   SLOT(containsPressed()));

  QObject::connect(ui->table->horizontalHeader(),
                   SIGNAL(sortIndicatorChanged(int,
                                               Qt::SortOrder)),
                   this,
                   SLOT(refresh()));

  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F),
                this,
                SLOT(focusFilter()));
}

ProductWidget::~ProductWidget()
{
  delete ui;
}

void ProductWidget::setDatabase(QSqlDatabase db)
{
  if (ui->table->model() != nullptr)
    return;

  QSqlTableModel* model = new QSqlTableModel(this, db);
  model->setTable("_ITEMS");
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);

  setColumnText(model, ProductTableIndex::ID);
  setColumnText(model, ProductTableIndex::Description);
  setColumnText(model, ProductTableIndex::Unity);
  setColumnText(model, ProductTableIndex::Supplier);
  setColumnText(model, ProductTableIndex::Price);
  setColumnText(model, ProductTableIndex::Details);
  setColumnText(model, ProductTableIndex::MidasCode);
  setColumnText(model, ProductTableIndex::Icon);

  setColumnIcon(model, ProductTableIndex::Description);
  setColumnIcon(model, ProductTableIndex::Unity);
  setColumnIcon(model, ProductTableIndex::Supplier);
  setColumnIcon(model, ProductTableIndex::Price);
  setColumnIcon(model, ProductTableIndex::Details);

  ui->table->setModel(model);
  ui->table->hideColumn((int)ProductTableIndex::ID);
  ui->table->hideColumn((int)ProductTableIndex::MidasCode);
  ui->table->hideColumn((int)ProductTableIndex::Icon);
  ui->table->horizontalHeader()->setSortIndicator((int)ProductTableIndex::Description,
                                                  Qt::SortOrder::AscendingOrder);
  ui->table->horizontalHeader()->setSectionResizeMode((int)ProductTableIndex::Description,
                                                      QHeaderView::Stretch);
  ui->table->horizontalHeader()->setSectionResizeMode((int)ProductTableIndex::Unity,
                                                      QHeaderView::ResizeToContents);
  ui->table->horizontalHeader()->setSectionResizeMode((int)ProductTableIndex::Supplier,
                                                      QHeaderView::ResizeToContents);
  ui->table->horizontalHeader()->setSectionResizeMode((int)ProductTableIndex::Price,
                                                      QHeaderView::ResizeToContents);
  ui->table->horizontalHeader()->setSectionResizeMode((int)ProductTableIndex::Price,
                                                      QHeaderView::ResizeToContents);

  QObject::connect(ui->table->selectionModel(),
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

void ProductWidget::refresh()
{
  if (ui->table->model() != nullptr)
  {
    confirm();
    auto idx = currentSortIndicator();
    m_filter.clear();
    m_filter.setPlaceholderText(filterPlaceholder(idx));
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
    model->setFilter("");
    model->select();
  }
  enableControls();
}

void ProductWidget::enableControls()
{
  bool bSelected = ui->table->currentIndex().isValid();
  ui->buttonRemove->setEnabled(bSelected);
  if (ui->table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
    ui->buttonSave->setEnabled(model->isDirty());
    ui->buttonDiscard->setEnabled(model->isDirty());
  }
}

void ProductWidget::removeSelectedProduct()
{
  if (ui->table->currentIndex().isValid())
  {
    if (ui->table->model() != nullptr)
    {
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
      model->removeRow(ui->table->currentIndex().row());
    }
  }
  enableControls();
}

void ProductWidget::save(bool bSkipConfirmation)
{
  if (ui->table->model() != nullptr)
  {
    int ret = !bSkipConfirmation ? QMessageBox::question(this,
                                                         tr("Salvar mudanças"),
                                                         tr("Tem certeza que deseja salvar as mudanças realizadas?"),
                                                         QMessageBox::Ok | QMessageBox::Cancel)
                                 : QMessageBox::Ok;
    if (ret == QMessageBox::Ok)
    {
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
      model->submitAll();
      refresh();
    }
  }
}

void ProductWidget::discard(bool bSkipConfirmation)
{
  if (ui->table->model() != nullptr)
  {
    int ret = !bSkipConfirmation ? QMessageBox::question(this,
                                                        tr("Descartar mudanças"),
                                                        tr("Tem certeza que deseja descartar as mudanças realizadas?"),
                                                        QMessageBox::Ok | QMessageBox::Cancel)
                                : QMessageBox::Ok;
    if (ret == QMessageBox::Ok)
    {
      m_filter.clear();
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
      model->setFilter("");
      model->revertAll();
      refresh();
    }
  }
}

void ProductWidget::create()
{
  if (ui->table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
    model->insertRow(model->rowCount());
  }
  enableControls();
}

void ProductWidget::setFilter()
{
  if (ui->table->model() != nullptr)
  {
    confirm();
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
    auto idx = currentSortIndicator();
    if (m_filter.text().isEmpty())
      m_filter.setPlaceholderText(filterPlaceholder(idx));
    QString filter(buildFilter(m_filter.text(),
                               idx,
                               ui->buttonContains->isChecked()));
    model->setFilter(filter);
  }
}

void ProductWidget::containsPressed()
{
  setFilter();
  m_filter.setFocus();
}

void ProductWidget::focusFilter()
{
  m_filter.setFocus();
  m_filter.selectAll();
}

void ProductWidget::confirm()
{
  QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
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

ProductTableIndex ProductWidget::currentSortIndicator() const
{
  int idx = ui->table->horizontalHeader()->sortIndicatorSection();
  return (ProductTableIndex)idx;
}

Product ProductWidget::product() const
{
  Product product;
  if (ui->table->currentIndex().isValid())
  {
    if (ui->table->model() != nullptr)
    {
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
      QSqlRecord rec = model->record(ui->table->currentIndex().row());
      product.m_id = rec.value((int)ProductTableIndex::ID).toInt();
      product.m_description = rec.value((int)ProductTableIndex::Description).toString();
      product.m_unity = rec.value((int)ProductTableIndex::Unity).toString();
      product.m_supplier = rec.value((int)ProductTableIndex::Supplier).toString();
      product.m_price = rec.value((int)ProductTableIndex::Price).toDouble();
      product.m_details = rec.value((int)ProductTableIndex::ID).toString();
      product.m_midasCode = rec.value((int)ProductTableIndex::ID).toString();
      product.m_icon = rec.value((int)ProductTableIndex::Icon).toInt();
    }
  }
  return product;
}

void ProductWidget::filterDownArrowPressed()
{
  ui->table->setFocus();
  if (ui->table->model() != nullptr)
  {
    if (ui->table->model()->rowCount() != 0)
      ui->table->selectRow(0);
  }
}

void ProductWidget::filterEnterKeyPressed()
{
  ui->table->setFocus();
  if (!ui->table->currentIndex().isValid())
  {
    if (ui->table->model() != nullptr &&
        ui->table->model()->rowCount() != 0)
    {
      ui->table->selectRow(0);
      ui->table->setFocus();
    }
    else
    {
      focusFilter();
    }
  }
}

void ProductWidget::productSelected()
{
  if (ui->table->currentIndex().isValid())
  {
    Product p = product();
    if (p.isValid())
    {
      emit productSelectedSignal(p);
      if (parentWidget() != nullptr)
        parentWidget()->close();
    }
  }
}

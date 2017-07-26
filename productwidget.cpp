#include "productwidget.h"
#include "ui_productwidget.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include "QSqlRecord"

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

ProductWidget::ProductWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::ProductWidget)
{
  ui->setupUi(this);
  ui->table->setSelectionBehavior(QAbstractItemView::SelectItems);
  ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
  QFont f = ui->table->font();
  f.setCapitalization(QFont::AllUppercase);
  ui->table->setFont(f);

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

  QObject::connect(ui->editFilter,
                   SIGNAL(textEdited(const QString&)),
                   this,
                   SLOT(setFilter()));

  QObject::connect(ui->buttonContains,
                   SIGNAL(stateChanged(int)),
                   this,
                   SLOT(contains()));

  QObject::connect(ui->table->horizontalHeader(),
                   SIGNAL(sortIndicatorChanged(int,
                                               Qt::SortOrder)),
                   this,
                   SLOT(refresh()));
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
}

void ProductWidget::refresh()
{
  if (ui->table->model() != nullptr)
  {
    confirm();
    auto idx = currentSortIndicator();
    ui->editFilter->clear();
    ui->editFilter->setPlaceholderText(filterPlaceholder(idx));
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
      ui->editFilter->clear();
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
    if (ui->editFilter->text().isEmpty())
      ui->editFilter->setPlaceholderText(filterPlaceholder(idx));
    QString filter(buildFilter(ui->editFilter->text(),
                               idx,
                               ui->buttonContains->isChecked()));
    model->setFilter(filter);
  }
}

void ProductWidget::contains()
{
  setFilter();
  ui->editFilter->setFocus();
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

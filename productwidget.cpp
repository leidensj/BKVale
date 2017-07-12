#include "productwidget.h"
#include "ui_productwidget.h"
#include <QMessageBox>
#include <QSqlTableModel>

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
                   SLOT(save()));

  QObject::connect(ui->buttonRevert,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(revert()));

  QObject::connect(ui->buttonCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
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
  model->setHeaderData((int)ProductTableIndex::ID, Qt::Horizontal, tr("ID"));
  model->setHeaderData((int)ProductTableIndex::Description, Qt::Horizontal, tr("Descrição"));
  model->setHeaderData((int)ProductTableIndex::Description, Qt::Horizontal, QVariant::fromValue(QIcon(":/icons/res/description.png")), Qt::DecorationRole);
  model->setHeaderData((int)ProductTableIndex::Unity, Qt::Horizontal, tr("Unidade"));
  model->setHeaderData((int)ProductTableIndex::Unity, Qt::Horizontal, QVariant::fromValue(QIcon(":/icons/res/unity.png")), Qt::DecorationRole);
  model->setHeaderData((int)ProductTableIndex::Supplier, Qt::Horizontal, tr("Fornecedor"));
  model->setHeaderData((int)ProductTableIndex::Supplier, Qt::Horizontal, QVariant::fromValue(QIcon(":/icons/res/supplier.png")), Qt::DecorationRole);
  model->setHeaderData((int)ProductTableIndex::Price, Qt::Horizontal, tr("Preço"));
  model->setHeaderData((int)ProductTableIndex::Price, Qt::Horizontal, QVariant::fromValue(QIcon(":/icons/res/price.png")), Qt::DecorationRole);
  model->setHeaderData((int)ProductTableIndex::Details, Qt::Horizontal, tr("Detalhes"));
  model->setHeaderData((int)ProductTableIndex::Details, Qt::Horizontal, QVariant::fromValue(QIcon(":/icons/res/details.png")), Qt::DecorationRole);
  model->setHeaderData((int)ProductTableIndex::MidasCode, Qt::Horizontal, tr("Código Midas"));
  model->setHeaderData((int)ProductTableIndex::Icon, Qt::Horizontal, tr("Ícone"));

  ui->table->setModel(model);
  ui->table->hideColumn((int)ProductTableIndex::ID);
  ui->table->hideColumn((int)ProductTableIndex::MidasCode);
  ui->table->hideColumn((int)ProductTableIndex::Icon);

  QObject::connect(ui->table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));

  QObject::connect(ui->table->selectionModel(),
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
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
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
    ui->buttonRevert->setEnabled(model->isDirty());
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

void ProductWidget::save()
{
  if (ui->table->model() != nullptr)
  {
    if (QMessageBox::question(this,
                              tr("Salvar mudanças"),
                              tr("Tem certeza que deseja salvar as mudanças realizadas?"),
                              QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
    {
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
      model->submitAll();
      refresh();
    }
  }
}

void ProductWidget::revert()
{
  if (ui->table->model() != nullptr)
  {
    if (QMessageBox::question(this,
                              tr("Reverter mudanças"),
                              tr("Tem certeza que deseja reverter as mudanças realizadas?"),
                              QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
    {
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->table->model());
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

#include "productwidget.h"
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

  m_table = new QTableView();

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(m_buttons);
  layout->addLayout(filterLayout);
  layout->addWidget(m_table);
  setLayout(layout);

  m_table->setSelectionBehavior(QAbstractItemView::SelectItems);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  {
    QFont f = m_table->font();
    f.setCapitalization(QFont::AllUppercase);
    m_table->setFont(f);
  }
  {
    QFont f = m_table->horizontalHeader()->font();
    f.setCapitalization(QFont::Capitalize);
    m_table->horizontalHeader()->setFont(f);
  }

  if (!bEditMode)
  {
    m_buttons->setEnabled(false);
    m_buttons->setVisible(false);
    m_table->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    m_table->horizontalHeader()->setHighlightSections(false);
  }

  QObject::connect(m_remove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeSelectedProduct()));

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
                   SIGNAL(filterChangedSignal()),
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

  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F),
                this,
                SLOT(focusFilter()));
}

ProductWidget::~ProductWidget()
{

}

void ProductWidget::setDatabase(QSqlDatabase db)
{
  if (m_table->model() != nullptr)
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

  m_table->setModel(model);
  m_table->hideColumn((int)ProductTableIndex::ID);
  m_table->hideColumn((int)ProductTableIndex::MidasCode);
  m_table->hideColumn((int)ProductTableIndex::Icon);
  m_table->horizontalHeader()->setSortIndicator((int)ProductTableIndex::Description,
                                                  Qt::SortOrder::AscendingOrder);
  m_table->horizontalHeader()->setSectionResizeMode((int)ProductTableIndex::Description,
                                                      QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode((int)ProductTableIndex::Unity,
                                                      QHeaderView::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)ProductTableIndex::Supplier,
                                                      QHeaderView::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)ProductTableIndex::Price,
                                                      QHeaderView::ResizeToContents);
  m_table->horizontalHeader()->setSectionResizeMode((int)ProductTableIndex::Price,
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

void ProductWidget::refresh()
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

void ProductWidget::enableControls()
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

void ProductWidget::removeSelectedProduct()
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

void ProductWidget::save(bool bSkipConfirmation)
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

void ProductWidget::discard(bool bSkipConfirmation)
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

void ProductWidget::create()
{
  if (m_table->model() != nullptr)
  {
    QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
    model->insertRow(model->rowCount());
  }
  enableControls();
}

void ProductWidget::setFilter()
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

void ProductWidget::containsPressed()
{
  setFilter();
  m_filter->setFocus();
}

void ProductWidget::focusFilter()
{
  m_filter->setFocus();
  m_filter->selectAll();
}

void ProductWidget::confirm()
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

ProductTableIndex ProductWidget::currentSortIndicator() const
{
  int idx = m_table->horizontalHeader()->sortIndicatorSection();
  return (ProductTableIndex)idx;
}

Product ProductWidget::product() const
{
  Product product;
  if (m_table->currentIndex().isValid())
  {
    if (m_table->model() != nullptr)
    {
      QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(m_table->model());
      QSqlRecord rec = model->record(m_table->currentIndex().row());
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
  m_table->setFocus();
  if (m_table->model() != nullptr)
  {
    if (m_table->model()->rowCount() != 0)
      m_table->selectRow(0);
  }
}

void ProductWidget::filterEnterKeyPressed()
{
  m_table->setFocus();
  if (!m_table->currentIndex().isValid())
  {
    if (m_table->model() != nullptr &&
        m_table->model()->rowCount() != 0)
    {
      m_table->selectRow(0);
      m_table->setFocus();
    }
    else
    {
      focusFilter();
    }
  }
}

void ProductWidget::productSelected()
{
  if (m_table->currentIndex().isValid())
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

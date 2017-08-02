#include "consumptionwidget.h"
#include "ui_consumptionwidget.h"
#include "productwidget.h"
#include <QDialog>

ConsumptionWidget::ConsumptionWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::ConsumptionWidget)
{
  ui->setupUi(this);
  QObject::connect(ui->buttonSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(search()));
}

ConsumptionWidget::~ConsumptionWidget()
{
  delete ui;
}

void ConsumptionWidget::setDatabase(QSqlDatabase db)
{
  m_db = db;
}

void ConsumptionWidget::search()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout();
  dlg.setLayout(layout);
  ProductWidget* product = new ProductWidget(false);
  product->setDatabase(m_db);
  layout->addWidget(product);
  dlg.resize(width(), height());
  dlg.setWindowTitle(tr("Buscar Produto"));
  dlg.setWindowIcon(QIcon(":/icons/res/item.png"));
  dlg.setModal(true);
  dlg.exec();
}

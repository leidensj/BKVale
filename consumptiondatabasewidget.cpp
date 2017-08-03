#include "consumptiondatabasewidget.h"
#include "ui_consumptiondatabasewidget.h"
#include <QDialog>

ConsumptionDatabaseWidget::ConsumptionDatabaseWidget(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::ConsumptionDatabaseWidget)
{
  ui->setupUi(this);
  ui->headerLayout->insertWidget(0, &m_consumptionWidget);
  QObject::connect(ui->buttonCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
}

ConsumptionDatabaseWidget::~ConsumptionDatabaseWidget()
{
  delete ui;
}

void ConsumptionDatabaseWidget::setDatabase(QSqlDatabase db)
{
  m_db = db;
}

void ConsumptionDatabaseWidget::create()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout();
  dlg.setLayout(layout);
  ConsumptionWidget* wd = new ConsumptionWidget();
  wd->setDatabase(m_db);
  layout->addWidget(wd);
  dlg.setWindowTitle(tr("Novo Consumo"));
  dlg.setWindowIcon(QIcon(":/icons/res/file.png"));
  dlg.setModal(true);
  dlg.exec();
}

#include "consumptiondatabase.h"
#include "ui_consumptiondatabasewidget.h"
#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QLayout>

ConsumptionDatabase::ConsumptionDatabase(QWidget *parent)
  : QFrame(parent)
  , m_refresh(nullptr)
  , m_remove(nullptr)
  , m_table(nullptr)
{
  {
    m_refresh = new QPushButton();
    m_refresh->setFlat(true);
    m_refresh->setText("");
    m_refresh->setIconSize(QSize(24, 24));
    m_refresh->setIcon(QIcon(":/icons/res/refresh.png"));
  }

  {
    m_remove = new QPushButton();
    m_remove->setFlat(true);
    m_remove->setText("");
    m_remove->setIconSize(QSize(24, 24));
    m_remove->setIcon(QIcon(":/icons/res/trash.png"));
  }

  QHBoxLayout* hlayout = new QHBoxLayout();
  hlayout->addWidget(m_refresh);
  hlayout->addWidget(m_remove);
  hlayout->setAlignment(Qt::AlignLeft);
  hlayout->setContentsMargins(0, 0, 0, 0);

  m_table = new QTableView();
  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->addLayout(hlayout);
  vlayout->addWidget(m_table);

  setLayout(vlayout);
}

ConsumptionDatabase::~ConsumptionDatabase()
{

}

void ConsumptionDatabase::setDatabase(QSqlDatabase db)
{
  m_db = db;
}

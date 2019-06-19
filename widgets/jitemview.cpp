#include "jitemview.h"
#include "jdatabase.h"
#include "items/jitemex.h"
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QSplitter>

JItemView::JItemView(const QString& tableName, QWidget* parent)
  : QFrame(parent)
  , m_database(nullptr)
  , m_tab(nullptr)
  , m_tabDb(nullptr)
  , m_ltButton(nullptr)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_btnSearch(nullptr)
  , m_dlgDb(nullptr)
{
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
  m_btnCreate->setToolTip(tr("Novo (Ctrl+N)"));

  m_btnSave = new QPushButton;
  m_btnSave->setFlat(true);
  m_btnSave->setText("");
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
  m_btnSave->setToolTip(tr("Salvar (Ctrl+S)"));

  m_btnSearch = new QPushButton;
  m_btnSearch->setFlat(true);
  m_btnSearch->setText("");
  m_btnSearch->setIconSize(QSize(24, 24));
  m_btnSearch->setIcon(QIcon(":/icons/res/search.png"));
  m_btnSearch->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
  m_btnSearch->setToolTip(tr("Pesquisar (Ctrl+F)"));

  m_ltButton = new QHBoxLayout;
  m_ltButton->setContentsMargins(0, 0, 0, 0);
  m_ltButton->addWidget(m_btnCreate);
  m_ltButton->addWidget(m_btnSave);
  m_ltButton->addWidget(m_btnSearch);
  m_ltButton->setAlignment(Qt::AlignLeft);

  m_tab = new QTabWidget;

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(0, 0, 9, 0);
  mainLayout->setAlignment(Qt::AlignTop);
  mainLayout->addLayout(m_ltButton);
  mainLayout->addWidget(m_tab);

  setLayout(mainLayout);

  m_database = new JDatabase(tableName);
  m_dlgDb = new QDialog(this);

  QVBoxLayout* ltDb = new QVBoxLayout;
  ltDb->setContentsMargins(0, 0, 0, 0);
  ltDb->addWidget(m_database);
  QFrame* frDb = new QFrame;
  frDb->setLayout(ltDb);

  m_tabDb = new QTabWidget;
  m_tabDb->addTab(frDb, QIcon(":/icons/res/search.png"), tr("Pesquisar"));

  QHBoxLayout *ltDlg = new QHBoxLayout;
  ltDlg->addWidget(m_tabDb);
  m_dlgDb->setLayout(ltDlg);
  m_dlgDb->setWindowFlags(Qt::Window);
  m_dlgDb->setWindowTitle(JItemEx::text(tableName));
  m_dlgDb->setWindowIcon(QIcon(JItemEx::icon(tableName)));
  m_dlgDb->setModal(true);

  connect(m_btnCreate, SIGNAL(clicked(bool)), this, SLOT(create()));
  connect(m_btnSave, SIGNAL(clicked(bool)), this, SLOT(save()));
  connect(m_database, SIGNAL(itemSelectedSignal(const JItemSQL&)), this, SLOT(selectItem(const JItemSQL&)));
  connect(m_database, SIGNAL(itemSelectedSignal(const JItemSQL&)), m_dlgDb, SLOT(accept()));
  connect(m_database, SIGNAL(itemsRemovedSignal(const QVector<Id>&)), this, SLOT(itemsRemoved(const QVector<Id>&)));
  connect(m_btnSearch, SIGNAL(clicked(bool)), m_dlgDb, SLOT(exec()));

  setMinimumWidth(600);
}

void JItemView::selectItem(const JItemSQL& o)
{
  setItem(o);
  QString strIcon = o.m_id.isValid()
                    ? ":/icons/res/saveas.png"
                    : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(strIcon));
  if (m_tab->count() > 0)
    m_tab->setCurrentIndex(0);
}

void JItemView::itemsRemoved(const QVector<Id>& ids)
{
  if (ids.contains(getId()))
    create();
}

void JItemView::save()
{
  if (m_database->save(getItem()))
    create();
}

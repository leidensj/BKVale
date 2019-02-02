#include "jitemview.h"
#include "jdatabase.h"
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QSplitter>

JItemView::JItemView(const QString& tableName, QWidget* parent)
  : QFrame(parent)
  , m_tab(nullptr)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_btnSearch(nullptr)
{
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

  m_btnSave = new QPushButton;
  m_btnSave->setFlat(true);
  m_btnSave->setText("");
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

  m_btnSearch = new QPushButton;
  m_btnSearch->setFlat(true);
  m_btnSearch->setText("");
  m_btnSearch->setIconSize(QSize(24, 24));
  m_btnSearch->setIcon(QIcon(":/icons/res/search.png"));
  m_btnSearch->setCheckable(true);

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSave);
  buttonlayout->addWidget(m_btnSearch);
  buttonlayout->setAlignment(Qt::AlignLeft);

  m_tab = new QTabWidget;

  QVBoxLayout* viewlayout = new QVBoxLayout;
  viewlayout->setContentsMargins(0, 0, 9, 0);
  viewlayout->setAlignment(Qt::AlignTop);
  viewlayout->addLayout(buttonlayout);
  viewlayout->addWidget(m_tab);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewlayout);

  m_database = new JDatabase(tableName);
  m_database->layout()->setContentsMargins(9, 0, 0, 0);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(viewFrame);
  splitter->addWidget(m_database);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  connect(m_btnCreate, SIGNAL(clicked(bool)), this, SLOT(create()));
  connect(m_btnSave, SIGNAL(clicked(bool)), this, SLOT(save()));
  connect(m_database, SIGNAL(itemSelectedSignal(const JItem&)), this, SLOT(selectItem(const JItem&)));
  connect(m_database, SIGNAL(itemsRemovedSignal(const QVector<Id>&)), this, SLOT(itemsRemoved(const QVector<Id>&)));
  connect(m_btnSearch, SIGNAL(clicked(bool)), m_database, SLOT(setVisible(bool)));

  setMinimumWidth(600);
  m_database->hide();
}

void JItemView::selectItem(const JItem& o)
{
  setItem(o);
  QString strIcon = o.m_id.isValid()
                    ? ":/icons/res/saveas.png"
                    : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(strIcon));
  m_currentId = o.m_id;
}

void JItemView::itemsRemoved(const QVector<Id>& ids)
{
  if (ids.contains(m_currentId))
    create();
}

void JItemView::save()
{
  if (m_database->save(getItem()))
    create();
}

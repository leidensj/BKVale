#include "jitemview.h"
#include "controls/databaseviewer.h"
#include "items/jitemex.h"
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QMenu>
#include <QDialog>
#include <QDesktopWidget>
#include <QApplication>
#include "items/activeuser.h"

#define VIEW_BUTTON "VIEW_BUTTON"

JItemView::JItemView(const QString& tableName, QWidget* parent)
  : QWidget(parent)
  , m_viewer(nullptr)
  , m_tab(nullptr)
  , m_tabDb(nullptr)
  , m_ltButton(nullptr)
  , m_btnClear(nullptr)
  , m_btnSave(nullptr)
  , m_btnSearch(nullptr)
  , m_btnMore(nullptr)
  , m_dlgDb(nullptr)
  , m_wFocus(nullptr)
{
  m_btnClear = new QPushButton;
  m_btnClear->setFlat(true);
  m_btnClear->setIconSize(QSize(24, 24));
  m_btnClear->setIcon(QIcon(":/icons/res/file.png"));
  m_btnClear->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
  m_btnClear->setToolTip(tr("Novo (Ctrl+N)"));

  m_btnSave = new QPushButton;
  m_btnSave->setFlat(true);
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
  m_btnSave->setToolTip(tr("Salvar (Ctrl+S)"));

  m_btnSearch = new QPushButton;
  m_btnSearch->setFlat(true);
  m_btnSearch->setIconSize(QSize(24, 24));
  m_btnSearch->setIcon(QIcon(":/icons/res/search.png"));
  m_btnSearch->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
  m_btnSearch->setToolTip(tr("Pesquisar (Ctrl+F)"));

  m_ltButton = new QHBoxLayout;
  m_ltButton->setContentsMargins(0, 0, 0, 0);
  m_ltButton->addWidget(m_btnClear);
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

  m_viewer = new DatabaseViewer(tableName, DatabaseViewer::Mode::Edit);
  m_dlgDb = new QDialog(this);

  QVBoxLayout* ltDb = new QVBoxLayout;
  ltDb->setContentsMargins(0, 0, 0, 0);
  ltDb->addWidget(m_viewer);
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
  QRect rect = QApplication::desktop()->availableGeometry(this);
  m_dlgDb->resize(rect.width() * 0.7, rect.height() * 0.7);

  connect(m_btnClear, SIGNAL(clicked(bool)), this, SLOT(clear()));
  connect(m_btnSave, SIGNAL(clicked(bool)), this, SLOT(save()));
  connect(m_viewer, SIGNAL(itemsSelectedSignal()), this, SLOT(setItem()));
  connect(m_viewer, SIGNAL(itemsSelectedSignal()), m_dlgDb, SLOT(accept()));
  connect(m_viewer, SIGNAL(itemsRemovedSignal(const Ids&)), this, SLOT(itemsRemoved(const Ids&)));
  connect(m_btnSearch, SIGNAL(clicked(bool)), m_dlgDb, SLOT(exec()));

  setMinimumWidth(600);
}

JItemView::~JItemView()
{
  if (m_btnMore != nullptr)
  {
    if (m_btnMore->menu() != nullptr)
    {
      delete m_btnMore->menu();
      m_btnMore->setMenu(nullptr);
    }
  }
}

void JItemView::setItem()
{
  auto p = JItemEx::create(m_viewer->getTableName());
  if (p != nullptr)
  {
    clear();
    p->m_id = m_viewer->getFirstSelectedId();
    if (p->m_id.isValid())
      JItemEx::select(*p, this);
    m_id = p->m_id;
    setItem(*p);
    QString strIcon = p->m_id.isValid()
                      ? ":/icons/res/saveas.png"
                      : ":/icons/res/save.png";
    m_btnSave->setIcon(QIcon(strIcon));
    delete p;
  }
}

void JItemView::itemsRemoved(const Ids& ids)
{
  if (ids.contains(getId()))
    clear();
}

bool JItemView::save(Id& id)
{
  id.clear();
  JItemSQL* p = JItemEx::create(m_viewer->getTableName());
  bool bSuccess = false;
  if (p != nullptr)
  {
    getItem(*p);
    bSuccess = JItemEx::save(*p, m_viewer->getTableName(), this);
    if (bSuccess)
    {
      clear();
      id = p->m_id;
    }
    delete p;
  }
  return bSuccess;
}

void JItemView::save()
{
  Id id;
  save(id);
}

Id JItemView::getId() const
{
  return m_id;
}

void JItemView::setFocusWidgetOnClear(QWidget* w)
{
  m_wFocus = w;
}

void JItemView::clear()
{
  JItemSQL* p = JItemEx::create(m_viewer->getTableName());
  if (p != nullptr)
  {
    m_id = p->m_id;
    setItem(*p);
    m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
    if (m_tab->count() > 0)
      m_tab->setCurrentIndex(0);
    if (m_wFocus != nullptr)
      m_wFocus->setFocus();
    delete p;
  }
}

void JItemView::addViewButton(const QString& tableName)
{
  QString error;
  ActiveUser au;
  if (au.SQL_select_current_user(error))
  {
    if (au.getUser().hasPermission(tableName))
    {
      if (m_btnMore == nullptr)
      {
        m_btnMore = new QPushButton;
        m_btnMore->setFlat(true);
        m_btnMore->setText("");
        m_btnMore->setIconSize(QSize(24, 24));
        m_btnMore->setIcon(QIcon(":/icons/res/more.png"));
        m_btnMore->setToolTip(tr("Mais"));

        QFrame *line = new QFrame;
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        m_ltButton->addWidget(line);
        m_ltButton->addWidget(m_btnMore);
      }

      if (m_btnMore->menu() == nullptr)
        m_btnMore->setMenu(new QMenu);
      QAction* act = m_btnMore->menu()->addAction(QIcon(JItemEx::icon(tableName)),
                                                  tr("Gerenciar ") + JItemEx::text(tableName),
                                                  this,
                                                  SLOT(viewButtonClicked()));
      act->setProperty(VIEW_BUTTON, tableName);
    }
  }
}

void JItemView::viewButtonClicked()
{
  QString tableName = sender()->property(VIEW_BUTTON).toString();
  QDialog dlg(this);
  JItemView* view = JItemEx::view(tableName);
  if (view != nullptr)
  {
    QHBoxLayout *lt = new QHBoxLayout;
    dlg.setLayout(lt);
    lt->addWidget(view);
    dlg.setWindowFlags(Qt::Window);
    dlg.setWindowTitle(tr("Gerenciar ") + JItemEx::text(tableName));
    dlg.setWindowIcon(QIcon(JItemEx::icon(tableName)));
    dlg.setModal(true);
    dlg.exec();
  }
}

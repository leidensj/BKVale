#include "jitemview.h"
#include "controls/databaseviewer.h"
#include "items/jitemhelper.h"
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QMenu>
#include <QDialog>
#include <QScreen>
#include <QToolButton>
#include <QApplication>
#include "items/login.h"

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
  , m_btnPrint(nullptr)
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

  m_btnPrint = new QPushButton;
  m_btnPrint->setFlat(true);
  m_btnPrint->setText("");
  m_btnPrint->setIconSize(QSize(24, 24));
  m_btnPrint->setIcon(QIcon(":/icons/res/printer.png"));
  m_btnPrint->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
  m_btnPrint->setToolTip(tr("Imprimir (Ctrl+P)"));
  m_btnPrint->setCheckable(true);
  m_btnPrint->setChecked(true);
  m_btnPrint->setEnabled(false);
  m_btnPrint->hide();

  m_ltButton = new QHBoxLayout;
  m_ltButton->setContentsMargins(0, 0, 0, 0);
  m_ltButton->addWidget(m_btnClear);
  m_ltButton->addWidget(m_btnSave);
  m_ltButton->addWidget(m_btnSearch);
  m_ltButton->addWidget(m_btnPrint);
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
  m_dlgDb->setWindowTitle(JItemHelper::text(tableName));
  m_dlgDb->setWindowIcon(QIcon(JItemHelper::icon(tableName)));
  m_dlgDb->setModal(true);

  QRect  rect = QGuiApplication::primaryScreen()->geometry();
  m_dlgDb->resize(rect.width() * 0.7, rect.height() * 0.7);

  connect(m_btnClear, SIGNAL(clicked(bool)), this, SLOT(clear()));
  connect(m_btnSave, SIGNAL(clicked(bool)), this, SLOT(save()));
  connect(m_viewer, SIGNAL(itemsSelectedSignal()), this, SLOT(setItem()));
  connect(m_viewer, SIGNAL(itemsSelectedSignal()), m_dlgDb, SLOT(accept()));
  connect(m_viewer, SIGNAL(itemsRemovedSignal(const Ids&)), this, SLOT(itemsRemoved(const Ids&)));
  connect(m_btnSearch, SIGNAL(clicked(bool)), m_dlgDb, SLOT(exec()));
  connect(m_viewer, SIGNAL(copySignal()), this, SLOT(setCopyItem()));

  setMinimumWidth(600);
}

JItemView::~JItemView()
{

}

void JItemView::setItem()
{
  auto p = JItemHelper::create(m_viewer->getTableName());
  if (p != nullptr)
  {
    clear();
    p->m_id = m_viewer->getFirstSelectedId();
    if (p->m_id.isValid())
      JItemHelper::select(*p, this);
    m_id = p->m_id;
    setItem(*p);
    QString strIcon = p->m_id.isValid()
                      ? ":/icons/res/saveas.png"
                      : ":/icons/res/save.png";
    m_btnSave->setIcon(QIcon(strIcon));
    delete p;
  }
}

void JItemView::setItem(const Id& id)
{
  auto p = JItemHelper::create(m_viewer->getTableName());
  if (p != nullptr)
  {
    clear();
    p->m_id = id;
    if (p->m_id.isValid())
      JItemHelper::select(*p, this);
    m_id = p->m_id;
    setItem(*p);
    QString strIcon = p->m_id.isValid()
                      ? ":/icons/res/saveas.png"
                      : ":/icons/res/save.png";
    m_btnSave->setIcon(QIcon(strIcon));
    delete p;
  }
}

void JItemView::setCopyItem()
{
  auto p = JItemHelper::create(m_viewer->getTableName());
  if (p != nullptr)
  {
    clear();
    p->m_id = m_viewer->getFirstSelectedId();
    if (p->m_id.isValid())
      JItemHelper::select(*p, this);
    p->m_id.clear();
    m_id.clear();
    p->name();
    setItem(*p);
    QString strIcon = ":/icons/res/save.png";
    m_btnSave->setIcon(QIcon(strIcon));
    delete p;
    m_dlgDb->accept();
  }
}

void JItemView::itemsRemoved(const Ids& ids)
{
  if (ids.contains(getId()))
    clear();
}

void JItemView::save()
{
  JItemSQL* p = JItemHelper::create(m_viewer->getTableName());
  bool bSuccess = false;
  if (p != nullptr)
  {
    getItem(*p);
    bSuccess = JItemHelper::save(*p, this);
    if (bSuccess)
      clear();
    delete p;
  }
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
  JItemSQL* p = JItemHelper::create(m_viewer->getTableName());
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
  Login login(true);
  if (login.getUser().hasPermission(tableName))
  {
    bool bFirstButton = m_btnMore == nullptr;
    if (bFirstButton)
    {
      m_btnMore = new QToolButton;
      m_btnMore->setIconSize(QSize(24, 24));
      m_btnMore->setAutoRaise(true);
      m_btnMore->setPopupMode(QToolButton::MenuButtonPopup);
      QFrame *line = new QFrame;
      line->setFrameShape(QFrame::VLine);
      line->setFrameShadow(QFrame::Sunken);
      m_ltButton->addWidget(line);
      m_ltButton->addWidget(m_btnMore);
    }

    QAction* act = new QAction(QIcon(JItemHelper::icon(tableName)), JItemHelper::text(tableName), m_btnMore);
    act->setToolTip(tr("Gerenciar ") + JItemHelper::text(tableName));
    act->setProperty(VIEW_BUTTON, tableName);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(viewButtonClicked()));
    m_btnMore->addAction(act);
    if (bFirstButton)
      m_btnMore->setDefaultAction(act);
  }
}

void JItemView::viewButtonClicked()
{
  QString tableName = sender()->property(VIEW_BUTTON).toString();
  JItemView* view = JItemHelper::view(tableName);
  if (view != nullptr)
  {
    JItemViewDialog dlg(view, this);
    dlg.exec();
  }
}

QString JItemView::getTableName() const
{
  return m_viewer != nullptr ? m_viewer->getTableName() : "";
}

JItemViewDialog::JItemViewDialog(JItemView* view, QWidget* parent)
  : QDialog(parent)
{
  if (view != nullptr)
  {
    QHBoxLayout *lt = new QHBoxLayout;
    setLayout(lt);
    lt->addWidget(view);
    setWindowFlags(Qt::Window);
    setWindowTitle(tr("Gerenciar ") + JItemHelper::text(view->getTableName()));
    setWindowIcon(QIcon(JItemHelper::icon(view->getTableName())));
    setModal(true);
  }
}

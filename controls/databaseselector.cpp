#include "databaseselector.h"
#include "items/jitemex.h"
#include <QLayout>
#include <QLabel>

DatabaseSelector::DatabaseSelector(const QString& tableName,
                                   bool bMultiSelector,
                                   QWidget* parent)
  : QDialog(parent)
  , m_viewer(nullptr)
{
  m_viewer = new DatabaseViewer(tableName, bMultiSelector
                                ? DatabaseViewer::Mode::MultiSelector
                                : DatabaseViewer::Mode::SingleSelector);
  m_viewer->layout()->setContentsMargins(0, 0, 0, 0);
  QVBoxLayout* vlayout0 = new QVBoxLayout;
  if (bMultiSelector)
    vlayout0->addWidget(new QLabel(tr("Selecione os itens desejados e pressione Enter para confirmar.")));
  vlayout0->addWidget(m_viewer);
  setLayout(vlayout0);

  resize(640, 480);

  QString title = "Selecionar " + JItemEx::text(tableName);
  QString icon = JItemEx::icon(tableName);

  setWindowTitle(title);
  if (!icon.isEmpty())
    setWindowIcon(QIcon(icon));

  QObject::connect(m_viewer, SIGNAL(itemsSelectedSignal(const QVector<JItemSQL*>&)), this, SLOT(itemsSelected(const QVector<JItemSQL*>&)));
}

void DatabaseSelector::itemsSelected(const QVector<JItemSQL*>& /*v*/)
{
  accept();
}

DatabaseViewer* DatabaseSelector::getViewer() const
{
  return m_viewer;
}

void DatabaseSelector::closeEvent(QCloseEvent * e)
{
  m_viewer->clearSearch();
  m_viewer->refresh();
  QDialog::closeEvent(e);
}

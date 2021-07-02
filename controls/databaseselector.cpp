#include "databaseselector.h"
#include "items/jitemhelper.h"
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
  QVBoxLayout* lt = new QVBoxLayout;
  if (bMultiSelector)
    lt->addWidget(new QLabel(tr("Selecione os itens desejados e pressione Enter para confirmar.")));
  lt->addWidget(m_viewer);
  setLayout(lt);
  resize(640, 480);

  QString title = tr("Selecionar ") + JItemHelper::text(tableName);
  QString icon = JItemHelper::icon(tableName);

  setWindowTitle(title);
  if (!icon.isEmpty())
    setWindowIcon(QIcon(icon));

  QObject::connect(m_viewer, SIGNAL(itemsSelectedSignal()), this, SLOT(accept()));
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

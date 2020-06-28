#include "findwidget.h"
#include "jlineedit.h"
#include <QPushButton>
#include <QLayout>

FindWidget::FindWidget(QTextEdit* doc, QWidget* parent)
 : QWidget(parent)
 , m_doc(doc)
 , m_edFind(nullptr)
 , m_btnDown(nullptr)
 , m_btnUp(nullptr)
 , m_btnCaseSensitive(nullptr)
{
  m_edFind = new JLineEdit(Text::Input::All, false);
  m_edFind->setClearButtonEnabled(true);
  m_edFind->setArrowsAndEnterAsTab(false);
  m_edFind->setPlaceholderText(tr("Procurar..."));

  m_btnDown = new QPushButton;
  m_btnDown->setFlat(true);
  m_btnDown->setIconSize(QSize(16, 16));
  m_btnDown->setIcon(QIcon(":/icons/res/arrowdropdown.png"));
  m_btnDown->setToolTip(tr("Próximo"));

  m_btnUp = new QPushButton;
  m_btnUp->setFlat(true);
  m_btnUp->setIconSize(QSize(16, 16));
  m_btnUp->setIcon(QIcon(":/icons/res/arrowdropup.png"));
  m_btnUp->setToolTip(tr("Anterior"));

  m_btnCaseSensitive = new QPushButton;
  m_btnCaseSensitive->setFlat(true);
  m_btnCaseSensitive->setIconSize(QSize(16, 16));
  m_btnCaseSensitive->setIcon(QIcon(":/icons/res/uppercase.png"));
  m_btnCaseSensitive->setCheckable(true);
  m_btnCaseSensitive->setToolTip(tr("Diferenciar maiúsuclas e minúsculas"));

  QHBoxLayout* lt = new QHBoxLayout;
  lt->setContentsMargins(0, 0, 0, 0);
  lt->addWidget(m_edFind);
  lt->addWidget(m_btnUp);
  lt->addWidget(m_btnDown);
  lt->addWidget(m_btnCaseSensitive);
  setLayout(lt);

  if (m_doc != nullptr)
    m_doc->setStyleSheet("selection-color: black; selection-background-color: yellow;");

  connect(m_btnDown, SIGNAL(clicked(bool)), this, SLOT(find()));
  connect(m_btnUp, SIGNAL(clicked(bool)), this, SLOT(find()));
  connect(m_btnCaseSensitive, SIGNAL(clicked(bool)), this, SLOT(find()));
  connect(m_edFind, SIGNAL(textChanged(const QString&)), this, SLOT(updateControls()));
  connect(m_edFind, SIGNAL(enterSignal()), m_btnDown, SLOT(animateClick()));
  updateControls();
}

void FindWidget::find()
{
  if (!m_edFind->text().isEmpty() && m_doc != nullptr)
  {
    QTextDocument::FindFlags o;
    o.setFlag(QTextDocument::FindBackward, sender() == m_btnUp);
    o.setFlag(QTextDocument::FindCaseSensitively, m_btnCaseSensitive->isChecked());
    o.setFlag(QTextDocument::FindWholeWords, false);
    if (!m_doc->find(m_edFind->text(), o))
    {

    }
  }
}

void FindWidget::updateControls()
{
  const bool bIsEmpty = m_edFind->text().isEmpty();
  m_btnDown->setEnabled(!bIsEmpty);
  m_btnUp->setEnabled(!bIsEmpty);
}

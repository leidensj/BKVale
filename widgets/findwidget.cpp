#include "findwidget.h"
#include "jlineedit.h"
#include <QPushButton>
#include <QLayout>

FindWidget::FindWidget(QWidget* parent)
 : QWidget(parent)
 , m_edFind(nullptr)
 , m_btnDown(nullptr)
 , m_btnUp(nullptr)
 , m_btnCaseSensitive(nullptr)
{
  m_edFind = new JLineEdit(Text::Input::All, false);

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
  m_btnCaseSensitive->setToolTip(tr("Case Sensitive"));

  QHBoxLayout* lt = new QHBoxLayout;
  lt->setContentsMargins(0, 0, 0, 0);
  lt->addWidget(m_edFind);
  lt->addWidget(m_btnUp);
  lt->addWidget(m_btnDown);
  lt->addWidget(m_btnCaseSensitive);
  setLayout(lt);

  connect(m_btnDown, SIGNAL(clicked(bool)), this, SLOT(emitFindSignal()));
  connect(m_btnUp, SIGNAL(clicked(bool)), this, SLOT(emitFindSignal()));
  connect(m_btnCaseSensitive, SIGNAL(clicked(bool)), this, SLOT(emitFindSignal()));
}

void FindWidget::emitFindSignal()
{
  QTextDocument::FindFlags o;
  o.setFlag(QTextDocument::FindBackward, sender() == m_btnUp);
  o.setFlag(QTextDocument::FindCaseSensitively, m_btnCaseSensitive->isChecked());
  o.setFlag(QTextDocument::FindWholeWords, false);
  emit findSignal(m_edFind->text(), o);
}

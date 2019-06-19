#include "jaddremovebuttons.h"
#include <QPushButton>
#include <QLayout>

JAddRemoveButtons::JAddRemoveButtons(QWidget* parent)
  : QWidget(parent)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_lt(nullptr)
{
  m_lt = new QHBoxLayout;
  m_lt->setContentsMargins(0, 0, 0, 0);
  m_lt->setAlignment(Qt::AlignLeft);

  m_btnAdd = new QPushButton;
  m_btnAdd->setFlat(true);
  m_btnAdd->setText("");
  m_btnAdd->setIconSize(QSize(24, 24));
  m_btnAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAdd->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));
  m_btnAdd->setToolTip(tr("Adicionar (Alt++)"));

  m_btnRemove = new QPushButton;
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Minus));
  m_btnRemove->setToolTip(tr("Remover (Alt+-)"));

  m_lt->addWidget(m_btnAdd);
  m_lt->addWidget(m_btnRemove);
  setLayout(m_lt);
  m_btnRemove->setEnabled(false);
}

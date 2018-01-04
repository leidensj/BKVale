#include "usermgtwidget.h"
#include "usermgtview.h"
#include "usermgtdatabase.h"
#include <QLayout>

UserMgtWidget::UserMgtWidget(QWidget* parent)
  : QFrame(parent)
{
  m_view = new UserMgtView();
  m_view->layout()->setAlignment(Qt::AlignTop);
  m_view->setContentsMargins(9, 0, 0, 0);
  m_database = new UserMgtDatabase();
  m_database->setContentsMargins(0, 0, 0, 0);

  QHBoxLayout* h1 = new QHBoxLayout();
  h1->addWidget(m_database);
  h1->addWidget(m_view);
  h1->setContentsMargins(0, 0, 0, 0);
  setLayout(h1);
}

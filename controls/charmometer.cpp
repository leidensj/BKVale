#include "charmometer.h"
#include <QLayout>
#include <QPushButton>
#include "items/jitemhelper.h"

Charmometer::Charmometer(QWidget* parent)
 : QDialog(parent)
 , m_btnPrint(nullptr)
 , m_btnNew(nullptr)
{
  setWindowTitle(JItemHelper::text(Functionality::Idx::Charmometer));
  setWindowIcon(QIcon(JItemHelper::icon(Functionality::Idx::Charmometer)));

  m_btnNew = new QPushButton;
  m_btnNew->setIconSize(QSize(24,24));
  m_btnNew->setIcon(QIcon(":/icons/res/file.png"));
  m_btnNew->setFlat(true);
  m_btnNew->setToolTip(tr("Novo"));
  m_btnPrint = new QPushButton;
  m_btnPrint->setIconSize(QSize(24,24));
  m_btnPrint->setIcon(QIcon(":/icons/res/printer.png"));
  m_btnPrint->setFlat(true);
  m_btnPrint->setToolTip(tr("Imprimir"));

  auto vl = new QVBoxLayout;
  vl->addWidget(m_btnNew);
  vl->addWidget(m_btnPrint);
  vl->setAlignment(Qt::AlignLeft);

  setLayout(vl);


  QPalette pal = palette();
  pal.setColor(QPalette::Window, Qt::white);
  setAutoFillBackground(true);
  setPalette(pal);
  //setStyleSheet("background-color: white;");
  setWindowFlag(Qt::FramelessWindowHint);
  setWindowFlag(Qt::WindowStaysOnTopHint);
  showFullScreen();
  setModal(true);
  exec();
}

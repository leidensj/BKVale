#include "shoppinglistview.h"
#include <QPushButton>
#include <QCheckBox>
#include <QPlainTextEdit>
#include "jdatabase.h"
#include "jdatabasepicker.h"

namespace {
QPushButton* monthButtonFactory(int n)
{
  QPushButton* btn = new QPushButton;
  if (n >= 0 && n < 31)
  {
    btn = new QPushButton;
    btn->setFlat(true);
    btn->setIconSize(QSize(16, 16));
    btn->setCheckable(true);
    QString strIcon = ":/icons/res/cal" + QString::number(n + 1) + ".png";
    btn->setIcon(QIcon(strIcon));
  }
  return btn;
}

QPushButton* weekButtonFactory(int n)
{
  QPushButton* btn = new QPushButton;
  if (n > 0 && n <= 7)
  {
    btn = new QPushButton;
    btn->setFlat(true);
    btn->setIconSize(QSize(16, 16));
    btn->setCheckable(true);
    QString strIcon = ":/icons/res/calw" + QString::number(n) + ".png";
    btn->setIcon(QIcon(strIcon));
  }
  return btn;
}
}

ShoppingListView::ShoppingListView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_database(nullptr)
  , m_supplierPicker(nullptr)
  , m_teDescription(nullptr)
  , m_cbSupplierCalls(nullptr)
  , m_cbCallSupplier(nullptr)
  , m_cbWhatsapp(nullptr)
  , m_cbVisit(nullptr)
{
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
  m_btnCreate->setToolTip(tr("Novo (Ctrl+N)"));

  m_btnSave = new QPushButton;
  m_btnSave->setFlat(true);
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
  m_btnSave->setToolTip(tr("Salvar (Ctrl+S)"));

  m_supplierPicker = new JDatabasePicker(tr("Fornecedor"), QIcon(":/icons/res/supplier.png"), true, false, false);
  m_teDescription = new QPlainTextEdit;
  m_cbSupplierCalls = new QCheckBox;
  m_cbSupplierCalls->setIcon(QIcon(":/icons/res/phone.png"));
  m_cbCallSupplier = new QCheckBox;
  m_cbCallSupplier->setIcon(QIcon(":/icons/res/phoneback.png"));
  m_cbWhatsapp = new QCheckBox;
  m_cbWhatsapp->setIcon(QIcon(":/icons/res/whatsapp.png"));
  m_cbVisit = new QCheckBox;
  m_cbVisit->setIcon(QIcon(":/icons/res/visit.png"));

  for (int i = 0; i != 31; ++i)
    m_vbtnMonth.push_back(monthButtonFactory(i));

  for (int i = 0; i != 7; ++i)
    m_vbtnWeek.push_back(weekButtonFactory(i));

}

#include "jtablewidgetitem.h"
#include "jdatabase.h"
#include "tinyexpr.h"
#include "product.h"
#include "productbarcode.h"
#include "packageeditor.h"
#include <QObject>
#include <QPixmap>

DoubleTableWidgetItem::DoubleTableWidgetItem(JItem::DataType type, Color color, bool bCheckable)
  : m_type(type)
  , m_color(color)
  , m_bCheckable(bCheckable)
{
  if (m_bCheckable)
    setFlags(Qt::NoItemFlags |
             Qt::ItemIsSelectable |
             Qt::ItemIsEnabled |
             Qt::ItemIsEditable |
             Qt::ItemIsUserCheckable);
}

void DoubleTableWidgetItem::setValue(double val)
{
  setData(Qt::UserRole, val);
  setText(JItem::st_str(val, m_type));

  if (m_bCheckable)
  {
    if (checkState() == Qt::Unchecked)
      setFlags(flags() & ~Qt::ItemIsEditable);
    else
      setFlags(flags() | Qt::ItemIsEditable);
  }

  switch (m_color)
  {
    case Color::Background:
      setBackgroundColor(val == 0.0 ? QColor(255, 200, 200) : QColor(Qt::white));
      break;
    case Color::Foreground:
      setTextColor(QColor(val >= 0 ? Qt::red : Qt::darkGreen));
      break;
    case Color::None:
    default:
      break;
  }
}

double DoubleTableWidgetItem::getValue()
{
  return data(Qt::UserRole).toDouble();
}

bool DoubleTableWidgetItem::evaluate(const QString& exp)
{
  auto stdExp = exp.toStdString();
  int error = 0;
  double val = te_interp(stdExp.c_str(), &error);
  if (!error)
    setValue(val);
  return !error;
}

void DoubleTableWidgetItem::evaluate()
{
  if (!evaluate(text()))
    setValue(getValue());
}

PackageTableWidgetItem::PackageTableWidgetItem()
  : QTableWidgetItem()
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags |
           Qt::ItemIsSelectable |
           Qt::ItemIsEnabled);
}

JItemTableWidgetItem::JItemTableWidgetItem()
  : QTableWidgetItem()
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags |
           Qt::ItemIsSelectable |
           Qt::ItemIsEnabled);
}

void ProductTableWidgetItem::setItem(const JItem& o)
{
  m_product = dynamic_cast<const Product&>(o);
  setText(m_product.m_name);

  QPixmap pixmap(QSize(16, 16));
  pixmap.loadFromData(m_product.m_image.m_image);
  setIcon(QIcon(pixmap));
}

const JItem& ProductTableWidgetItem::getItem() const
{
  return m_product;
}

void ProductTableWidgetItem::selectItem(const QString& fixedFilter)
{
  JDatabaseSelector dlg(PRODUCT_SQL_TABLE_NAME,
                        QObject::tr("Selecionar Produto"),
                        QIcon(":/icons/res/item.png"));
  dlg.getDatabase()->setFixedFilter(fixedFilter);
  if (dlg.exec())
  {
    Product* p = static_cast<Product*>(dlg.getDatabase()->getCurrentItem());
    if (p != nullptr && p->isValidId())
      setItem(*p);
  }
}

void ProductTableWidgetItem::selectItemByBarcode(const QString& fixedFilter)
{
  JDatabaseSelector dlg(PRODUCT_BARCODE_SQL_TABLE_NAME,
                        QObject::tr("Selecionar Produto"),
                        QIcon(":/icons/res/item.png"));
  dlg.getDatabase()->setFixedFilter(fixedFilter);
  if (dlg.exec())
  {
    ProductBarcode* p = static_cast<ProductBarcode*>(dlg.getDatabase()->getCurrentItem());
    if (p != nullptr && p->isValidId())
      setItem(p->m_product);
  }
}

void PackageTableWidgetItem::setItem(const Package& o, const QString& productUnity)
{
  m_package = o;
  setText(m_package.strFormattedUnity(productUnity));
}

const Package& PackageTableWidgetItem::getItem() const
{
  return m_package;
}

void PackageTableWidgetItem::selectItem(const QString& productUnity)
{
  PackageEditor dlg(getItem(), productUnity);
  if (dlg.exec())
    setItem(dlg.getPackage(), productUnity);
}


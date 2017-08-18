#include "consumptionwidget.h"
#include "itemwidget.h"
#include <QDialog>
#include <QLayout>
#include <QDateEdit>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QKeyEvent>

ConsumptionDoubleSpinBox::ConsumptionDoubleSpinBox(QWidget *parent)
  : QDoubleSpinBox(parent)
{

}

void ConsumptionDoubleSpinBox::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Enter ||
      event->key() == Qt::Key_Return)
  {
    QKeyEvent modEvent(event->type(),
                       Qt::Key_Tab,
                       event->modifiers(),
                       event->nativeScanCode(),
                       event->nativeVirtualKey(),
                       event->nativeModifiers(),
                       event->text(),
                       event->isAutoRepeat(),
                       event->count());
    QDoubleSpinBox::keyPressEvent(&modEvent);
  }
  else
  {
    QDoubleSpinBox::keyPressEvent(event);
  }
}

ConsumptionWidget::ConsumptionWidget(QWidget *parent)
  : QFrame(parent)
  , m_date(nullptr)
  , m_weekDay(nullptr)
  , m_search(nullptr)
  , m_item(nullptr)
  , m_priceIcon(nullptr)
  , m_price(nullptr)
  , m_ammountIcon(nullptr)
  , m_ammount(nullptr)
{
  m_date = new QDateEdit();

  {
    m_weekDay = new QLabel();
    QPalette palette= m_weekDay->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    m_weekDay->setPalette(palette);
  }

  {
    m_search = new QPushButton();
    m_search->setFlat(true);
    m_search->setText("");
    m_search->setIconSize(QSize(24, 24));
    m_search->setIcon(QIcon(":/icons/res/search.png"));
  }

  {
    m_item = new QLineEdit();
    m_item->setEnabled(false);
  }

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->addWidget(m_date);
  hlayout1->addWidget(m_weekDay);
  hlayout1->addWidget(m_search);
  hlayout1->addWidget(m_item);
  hlayout1->setContentsMargins(0, 0, 0, 0);

  {
    m_priceIcon = new QLabel();
    m_priceIcon->setText("");
    m_priceIcon->setPixmap(QPixmap(":/icons/res/price.png"));
    m_priceIcon->setMinimumSize(24, 24);
    m_priceIcon->setMaximumSize(24,24);
    m_priceIcon->setScaledContents(true);
  }

  {
    m_price = new ConsumptionDoubleSpinBox();
    m_price->setSpecialValueText(tr("Preço"));
    m_price->setMinimum(0.0);
    m_price->setMaximum(9999.99);
    m_price->setSizePolicy(QSizePolicy::Policy::Expanding,
                           QSizePolicy::Policy::Fixed);
  }

  {
    m_ammountIcon = new QLabel();
    m_ammountIcon->setText("");
    m_ammountIcon->setPixmap(QPixmap(":/icons/res/ammount.png"));
    m_ammountIcon->setMinimumSize(24, 24);
    m_ammountIcon->setMaximumSize(24,24);
    m_ammountIcon->setScaledContents(true);
  }

  {
    m_ammount = new ConsumptionDoubleSpinBox();
    m_ammount->setSpecialValueText(tr("Quantidade"));
    m_ammount->setMinimum(0.0);
    m_ammount->setMaximum(9999.99);
    m_ammount->setSizePolicy(QSizePolicy::Policy::Expanding,
                           QSizePolicy::Policy::Fixed);
  }

  {
    m_save = new QPushButton();
    m_save->setFlat(true);
    m_save->setText("");
    m_save->setIconSize(QSize(24, 24));
    m_save->setIcon(QIcon(":/icons/res/save.png"));
  }

  QHBoxLayout* hlayout2 = new QHBoxLayout();
  hlayout2->addWidget(m_priceIcon);
  hlayout2->addWidget(m_price);
  hlayout2->addWidget(m_ammountIcon);
  hlayout2->addWidget(m_ammount);
  hlayout2->addWidget(m_save);
  hlayout2->setContentsMargins(0, 0, 0, 0);

  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->addLayout(hlayout1);
  vlayout->addLayout(hlayout2);
  setLayout(vlayout);

  setFrameShape(QFrame::Shape::StyledPanel);
  setFrameShadow(QFrame::Shadow::Plain);

  QObject::connect(m_search,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(search()));

  QObject::connect(m_date,
                   SIGNAL(dateChanged(const QDate&)),
                   this,
                   SLOT(dateChanged(const QDate&)));

  m_date->setDate(QDate::currentDate());
  enableControls();
}

ConsumptionWidget::~ConsumptionWidget()
{

}

void ConsumptionWidget::setDatabase(QSqlDatabase db)
{
  m_db = db;
}

void ConsumptionWidget::search()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout();
  dlg.setLayout(layout);
  ItemWidget* itemWidget = new ItemWidget(false);
  itemWidget->setDatabase(m_db);
  layout->addWidget(itemWidget);
  dlg.resize(540, 280);
  dlg.setWindowTitle(tr("Buscar Produto"));
  dlg.setWindowIcon(QIcon(":/icons/res/item.png"));
  dlg.setModal(true);
  itemWidget->focusFilter();

  QObject::connect(itemWidget,
                   SIGNAL(itemSelectedSignal(const Item&)),
                   this,
                   SLOT(itemSelected(const Item&)));

  dlg.exec();
}

void ConsumptionWidget::dateChanged(const QDate& date)
{
  m_weekDay->setText(date.toString("dddd").toUpper());
}

void ConsumptionWidget::enableControls()
{
  bool bItemSelected = !m_item->text().isEmpty();
  m_ammount->setEnabled(bItemSelected);
  m_price->setEnabled(bItemSelected);
  m_save->setEnabled(bItemSelected);
}

void ConsumptionWidget::itemSelected(const Item& item)
{
  if (item.isValid())
  {
    m_item->setText(item.m_description);
    m_ammount->setSuffix(item.m_unity);
    m_price->setSuffix("/" +item.m_unity);
    m_price->setValue(item.m_price);
    m_ammount->setEnabled(true);
    m_price->setEnabled(true);
    m_save->setEnabled(true);
  }
}
Consumption ConsumptionWidget::consumption() const
{

}

void ConsumptionWidget::save()
{

}

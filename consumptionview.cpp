#include "consumptionview.h"
#include "itemwidget.h"
#include <QDialog>
#include <QLayout>
#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include <QKeyEvent>

EnterPushButton::EnterPushButton(QWidget *parent)
  : QPushButton(parent)
{

}

void EnterPushButton::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Enter ||
      event->key() == Qt::Key_Return)
  {
    animateClick();
  }
  else
  {
    QPushButton::keyPressEvent(event);
  }
}

EnterDoubleSpinBox::EnterDoubleSpinBox(QWidget *parent)
  : QDoubleSpinBox(parent)
{

}

void EnterDoubleSpinBox::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Enter ||
      event->key() == Qt::Key_Return)
  {
    if (value() < maximum() &&
        value() > minimum())
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
  }
  else
  {
    QDoubleSpinBox::keyPressEvent(event);
  }
}

ConsumptionView::ConsumptionView(QWidget *parent)
  : QFrame(parent)
  , m_currentItemID(INVALID_ITEM_ID)
  , m_date(nullptr)
  , m_weekDay(nullptr)
  , m_search(nullptr)
  , m_item(nullptr)
  , m_price(nullptr)
  , m_ammount(nullptr)
  , m_save(nullptr)
{
  m_date = new QDateEdit();
  m_date->setCalendarPopup(true);
  m_date->setDisplayFormat("dd/MM/yyyy");

  m_weekDay = new QLabel();

  m_search = new EnterPushButton();
  m_search->setFlat(true);
  m_search->setText("");
  m_search->setIconSize(QSize(24, 24));
  m_search->setIcon(QIcon(":/icons/res/binoculars.png"));
  m_search->setDefault(true);

  m_item = new QLineEdit();
  m_item->setEnabled(false);

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->addWidget(m_date);
  hlayout1->addWidget(m_weekDay);
  hlayout1->addWidget(m_search);
  hlayout1->addWidget(m_item);
  hlayout1->setContentsMargins(0, 0, 0, 0);


  m_price = new EnterDoubleSpinBox();
  m_price->setMinimum(0.0);
  m_price->setMaximum(9999.99);
  m_price->setSpecialValueText(tr("Preço"));
  m_price->setSizePolicy(QSizePolicy::Policy::Expanding,
                         QSizePolicy::Policy::Fixed);

  m_ammount = new EnterDoubleSpinBox();
  m_ammount->setMinimum(0.0);
  m_ammount->setMaximum(9999.99);
  m_ammount->setSpecialValueText(tr("Quantidade"));
  m_ammount->setSizePolicy(QSizePolicy::Policy::Expanding,
                           QSizePolicy::Policy::Fixed);

  m_save = new EnterPushButton();
  m_save->setFlat(true);
  m_save->setText("");
  m_save->setIconSize(QSize(24, 24));
  m_save->setIcon(QIcon(":/icons/res/save.png"));
  m_save->setDefault(true);

  QHBoxLayout* hlayout2 = new QHBoxLayout();
  hlayout2->addWidget(m_price);
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

  QObject::connect(m_save,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save()));

  QObject::connect(m_price,
                   SIGNAL(valueChanged(double)),
                   this,
                   SLOT(enableControls()));

  QObject::connect(m_ammount,
                   SIGNAL(valueChanged(double)),
                   this,
                   SLOT(enableControls()));

  m_date->setDate(QDate::currentDate());
  clear();
  enableControls();
}

ConsumptionView::~ConsumptionView()
{

}

void ConsumptionView::setDatabase(QSqlDatabase db)
{
  m_db = db;
}

void ConsumptionView::search()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout();
  dlg.setLayout(layout);
  ItemWidget* w = new ItemWidget();
  w->setDatabase(m_db);
  layout->addWidget(w);
  dlg.setWindowTitle(tr("Buscar Produto"));
  dlg.setWindowIcon(QIcon(":/icons/res/item.png"));
  dlg.setModal(true);

  dlg.exec();
}

void ConsumptionView::dateChanged(const QDate& date)
{
  m_weekDay->setText(date.toString("dddd"));
}

void ConsumptionView::enableControls()
{
  m_ammount->setEnabled(m_currentItemID != INVALID_ITEM_ID);
  m_price->setEnabled(m_currentItemID != INVALID_ITEM_ID);
  m_save->setEnabled(m_currentItemID != INVALID_ITEM_ID &&
                     m_price->value() &&
                     m_ammount->value());
}

void ConsumptionView::itemSelected(const Item& item)
{
  if (item.isValidId())
  {
    m_item->setText(item.m_name);
    m_currentItemID = item.m_id;
    m_ammount->setSuffix(item.m_unity);
    m_price->setSuffix("/" +item.m_unity);
    m_price->setValue(0.0);
    enableControls();
    m_price->setFocus();
    m_price->selectAll();
  }
}

Consumption ConsumptionView::consumption() const
{
  Consumption c;
  c.m_date = m_date->date().toJulianDay();
  c.m_ammount = m_ammount->value();
  c.m_price = m_price->value();
  c.m_total = m_ammount->value() * m_price->value();
  c.m_itemID = m_currentItemID;
  return c;
}

void ConsumptionView::save()
{
  emit insertSignal(consumption());
  clear();
  enableControls();
  m_search->setFocus();
}

void ConsumptionView::clear()
{
  m_currentItemID = INVALID_ITEM_ID;
  m_item->setText("");
  m_price->setValue(0.0);
  m_ammount->setValue(0.0);
}

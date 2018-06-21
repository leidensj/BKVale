#include "reservationview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "jdatabase.h"
#include "jspinbox.h"
#include <QDateTimeEdit>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QMessageBox>
#include <QDockWidget>
#include <QPlainTextEdit>

ReservationView::ReservationView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_btnCreate(nullptr)
  , m_btnSearch(nullptr)
  , m_snNumber(nullptr)
  , m_edName(nullptr)
  , m_edPhone(nullptr)
  , m_edLocation(nullptr)
  , m_dateTimeEdit(nullptr)
  , m_snAmmount(nullptr)
  , m_teObservation(nullptr)
  , m_dock(nullptr)
  , m_database(nullptr)
{
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

  m_btnSearch = new QPushButton;
  m_btnSearch->setFlat(true);
  m_btnSearch->setText("");
  m_btnSearch->setIconSize(QSize(24, 24));
  m_btnSearch->setIcon(QIcon(":/icons/res/search.png"));
  m_btnSearch->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));

  m_snNumber = new JSpinBox(true);
  m_snNumber->setReadOnly(true);
  m_snNumber->setButtonSymbols(QSpinBox::ButtonSymbols::NoButtons);
  m_snNumber->setMaximum(99999999);
  m_snNumber->setMinimum(0);
  m_snNumber->setSpecialValueText(tr("S/N"));
  {
    QFont font = m_snNumber->font();
    font.setBold(true);
    m_snNumber->setFont(font);
    QPalette palette = m_snNumber->palette();
    palette.setColor(QPalette::ColorRole::Text, Qt::red);
    m_snNumber->setPalette(palette);
  }

  m_edName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
  m_edPhone = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
  m_edLocation = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
  m_dateTimeEdit = new QDateTimeEdit;
  m_dateTimeEdit->setCalendarPopup(true);
  m_dateTimeEdit->setDisplayFormat("dd/MM/yyyy HH:mm");
  m_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
  m_dateTimeEdit->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
  m_snAmmount = new JSpinBox(true);
  m_snAmmount->setSuffix(tr(" Pessoas"));
  m_dock = new QDockWidget;
  m_database = new JDatabase(RESERVATION_SQL_TABLE_NAME);
  m_teObservation = new QPlainTextEdit;

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSearch);
  buttonlayout->setAlignment(Qt::AlignLeft);

  QFormLayout* informationlayout = new QFormLayout;
  informationlayout->addRow(tr("Número:"), m_snNumber);
  informationlayout->addRow(tr("Nome:"), m_edName);
  informationlayout->addRow(tr("Telefone:"), m_edPhone);
  informationlayout->addRow(tr("Local:"), m_edLocation);
  informationlayout->addRow(tr("Horário:"), m_dateTimeEdit);
  informationlayout->addRow(tr("Quantidade:"), m_snAmmount);
  informationlayout->addRow(tr("Observações:"), m_teObservation);

  QFrame* informationFrame = new QFrame();
  informationFrame->setFrameShape(QFrame::Shape::StyledPanel);
  informationFrame->setFrameShadow(QFrame::Shadow::Plain);
  informationFrame->setLayout(informationlayout);

  QVBoxLayout* reservationlayout = new QVBoxLayout;
  reservationlayout->setContentsMargins(9, 0, 0, 0);
  reservationlayout->addLayout(buttonlayout);
  reservationlayout->addWidget(informationFrame);
  reservationlayout->setAlignment(Qt::AlignTop);

  QFrame* reservationFrame = new QFrame;
  reservationFrame->setLayout(reservationlayout);

  m_dock = new QDockWidget;
  m_dock->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  m_dock->setFeatures(0);
  m_dock->setFeatures(QDockWidget::DockWidgetClosable);
  m_dock->setWindowTitle(tr("Pesquisar"));
  m_dock->setWidget(m_database);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_dock);
  splitter->addWidget(reservationFrame);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));
  QObject::connect(m_database,
                   SIGNAL(itemRemovedSignal(qlonglong)),
                   this,
                   SLOT(itemRemoved(qlonglong)));
  QObject::connect(m_btnSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(showSearch()));
  m_dock->close();
}

void ReservationView::itemSelected(const JItem& jItem)
{
  const Reservation& res = dynamic_cast<const Reservation&>(jItem);
  if (res.isValidId())
    setReservation(res);
}

void ReservationView::itemRemoved(qlonglong id)
{
  if (m_currentId == id)
    create();
}

Reservation ReservationView::save()
{
  Reservation res = getReservation();
  if (m_database->save(res))
    create();
  return res;
}

void ReservationView::create()
{
  m_currentId = INVALID_ID;
  setReservation(Reservation());
  m_edName->setFocus();
}

Reservation ReservationView::getReservation() const
{
  Reservation res;
  res.m_id = m_currentId;
  res.m_number = m_snNumber->value();
  res.m_name = m_edName->text();
  res.m_phone = m_edPhone->text();
  res.m_location = m_edLocation->text();
  res.m_dateTime = m_dateTimeEdit->dateTime().toString(Qt::DateFormat::ISODate);
  res.m_ammount = m_snAmmount->value();
  res.m_observation = m_teObservation->toPlainText();
  return res;
}

void ReservationView::setReservation(const Reservation &res)
{
  m_currentId = res.m_id;
  m_snNumber->setValue(res.m_number);
  m_edName->setText(res.m_name);
  m_edPhone->setText(res.m_phone);
  m_edLocation->setText(res.m_location);
  m_dateTimeEdit->setDateTime(QDateTime::fromString(res.m_dateTime, Qt::DateFormat::ISODate));
  m_snAmmount->setValue(res.m_ammount);
  m_teObservation->setPlainText(res.m_observation);
}

void ReservationView::showSearch()
{
  if (m_dock->isVisible())
    m_dock->close();
  else
    m_dock->show();
}

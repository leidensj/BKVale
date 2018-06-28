#include "notefilterdlg.h"
#include "jdatabasepicker.h"
#include <QDateEdit>
#include <QCheckBox>
#include <QLayout>

NoteFilterDlg::NoteFilterDlg(QWidget* parent)
  : QDialog(parent)
  , m_cbDate(nullptr)
  , m_dtBegin(nullptr)
  , m_dtEnd(nullptr)
  , m_supplierPicker(nullptr)
  , m_productPicker(nullptr)
{
  m_cbDate = new QCheckBox;
  m_dtBegin = new QDateEdit;
  m_dtBegin->setCalendarPopup(true);
  m_dtEnd = new QDateEdit;
  m_dtEnd->setCalendarPopup(true);
  m_supplierPicker = new JDatabasePicker(PERSON_SQL_TABLE_NAME,
                                         tr("Fornecedor"),
                                         QIcon(":/icons/res/supplier.png"),
                                         true, false, true);
  m_productPicker = new JDatabasePicker(PRODUCT_SQL_TABLE_NAME,
                                        tr("Produto"),
                                        QIcon(":/icons/res/item.png"),
                                        true, false, true);

  QHBoxLayout* dateLayout = new QHBoxLayout;
  dateLayout->setContentsMargins(0, 0, 0, 0);
  dateLayout->setAlignment(Qt::AlignLeft);
  dateLayout->addWidget(m_cbDate);
  dateLayout->addWidget(m_dtBegin);
  dateLayout->addWidget(m_dtEnd);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->setAlignment(Qt::AlignTop);
  mainLayout->addLayout(dateLayout);
  mainLayout->addWidget(m_supplierPicker);
  mainLayout->addWidget(m_productPicker);
  setLayout(mainLayout);

  QObject::connect(m_cbDate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));

  setWindowFlags(Qt::Window);
  setWindowTitle(tr("Filtrar Notas"));
  setWindowIcon(QIcon(":/icons/res/filter.png"));
  setModal(true);

  clearFilter();
}

QString NoteFilterDlg::getFilter() const
{
  QString strFilter;
  if (m_cbDate->isChecked())
    strFilter += " " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL02 " BETWEEN " +
                 m_dtBegin->date().toString(Qt::ISODate) + " AND " +
                 m_dtEnd->date().toString(Qt::ISODate) + " ";

  QVector<qlonglong> vSupplier = m_supplierPicker->getIds();
  QVector<qlonglong> vProduct = m_productPicker->getIds();

  if (!vSupplier.isEmpty())
  {
    if (!strFilter.isEmpty())
      strFilter += " AND ";
    strFilter += " " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL03 " IN (";
    for (int i = 0; i != vSupplier.size(); ++i)
      strFilter += QString::number(vSupplier.at(i)) + ",";
    strFilter.chop(1);
    strFilter += ") ";
  }

  if (!vProduct.isEmpty())
  {
    if (!strFilter.isEmpty())
      strFilter += " AND ";
    strFilter += " " NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL02 " = ANY ("
                 "SELECT " NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL02 " FROM "
                 NOTE_ITEMS_SQL_TABLE_NAME " WHERE "
                 NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL02 " IN(";
    for (int i = 0; i != vProduct.size(); ++i)
      strFilter += QString::number(vProduct.at(i)) + ",";
    strFilter.chop(1);
    strFilter += "))";
  }

  return strFilter;
}

void NoteFilterDlg::clearFilter()
{
  m_cbDate->setChecked(false);
  m_dtBegin->setDate(QDate::currentDate());
  m_dtEnd->setDate(QDate::currentDate());
  m_supplierPicker->clearAll();
  m_productPicker->clearAll();
  updateControls();
}

void NoteFilterDlg::updateControls()
{
  m_dtBegin->setEnabled(m_cbDate->isChecked());
  m_dtEnd->setEnabled(m_cbDate->isChecked());
}

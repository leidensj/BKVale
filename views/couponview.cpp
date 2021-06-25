#include "couponview.h"
#include "printer.h"
#include "widgets/jlineedit.h"
#include "widgets/jdatepicker.h"
#include "controls/databasepicker.h"
#include "items/jitemhelper.h"
#include "tables/coupontable.h"
#include "filters/couponfilter.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>
#include <QRadioButton>
#include <QInputDialog>
#include <QSettings>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include <QProgressDialog>

#define SETTINGS_COUPON_STORE_ID "coupon/storeid"

CouponConfirmation::CouponConfirmation(const QVector<Coupon>& coupons, QWidget* parent)
  : QDialog(parent)
{
  QPlainTextEdit* teCodes = new QPlainTextEdit;
  QDialogButtonBox* btns = new QDialogButtonBox(QDialogButtonBox::Yes | QDialogButtonBox::No);
  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addWidget(new QLabel(tr("Deseja imprimir os %1 códigos gerados?").arg(coupons.size())));
  ltMain->addWidget(teCodes);
  ltMain->addWidget(btns);
  setLayout(ltMain);
  for (const Coupon& o : coupons)
    teCodes->appendPlainText(o.m_code);
  connect(btns, SIGNAL(accepted()), this, SLOT(accept()));
  connect(btns, SIGNAL(rejected()), this, SLOT(reject()));
}

CouponView::CouponView(QWidget* parent)
  : JItemView(COUPON_SQL_TABLE_NAME, parent)
  , m_lblRedeemed(nullptr)
  , m_edCode(nullptr)
  , m_cbExpiration(nullptr)
  , m_dtExpiration(nullptr)
  , m_rdoPercentage(nullptr)
  , m_rdoValue(nullptr)
  , m_rdoProduct(nullptr)
  , m_edPercentage(nullptr)
  , m_edValue(nullptr)
  , m_storePicker(nullptr)
  , m_btnAddRemove(nullptr)
  , m_table(nullptr)
  , m_lblCount(nullptr)
  , m_filter(nullptr)
{
  m_storePicker = new DatabasePicker(STORE_SQL_TABLE_NAME);

  m_edCode = new JLineEdit(Text::Input::Alpha, true);
  m_edCode->setPlaceholderText(tr("Gerar código automaticamente"));

  m_lblRedeemed = new QLabel;

  m_cbExpiration = new QCheckBox("Expira em:");
  m_dtExpiration = new JDatePicker;

  m_rdoPercentage = new QRadioButton(tr("Porcentagem:"));
  m_edPercentage = new JExpLineEdit(Data::Type::Percentage);
  m_edPercentage->setMaximum(100.0);
  m_edPercentage->setMinimum(0.0);

  m_rdoValue = new QRadioButton(tr("Valor:"));
  m_edValue = new JExpLineEdit(Data::Type::Money);
  m_edValue->setMinimum(0.0);

  m_rdoProduct = new QRadioButton(tr("Produto:"));
  m_btnAddRemove = new JAddRemoveButtons;
  m_table = new CouponTable(m_btnAddRemove);
  QWidget* product = new QWidget;
  QVBoxLayout* ltProduct = new QVBoxLayout;
  ltProduct->setAlignment(Qt::AlignTop);
  ltProduct->setContentsMargins(0, 0, 0, 0);
  ltProduct->addWidget(m_btnAddRemove);
  ltProduct->addWidget(m_table);
  product->setLayout(ltProduct);

  QFormLayout* ltMain = new QFormLayout;
  ltMain->addRow(JItemHelper::text(STORE_SQL_TABLE_NAME) + ":", m_storePicker);
  ltMain->addRow(tr("Código:"), m_edCode);
  ltMain->addRow(m_rdoPercentage, m_edPercentage);
  ltMain->addRow(m_rdoValue, m_edValue);
  ltMain->addRow(m_rdoProduct, product);
  ltMain->addRow(m_cbExpiration, m_dtExpiration);
  ltMain->setAlignment(Qt::AlignTop);
  ltMain->addRow(tr(""), m_lblRedeemed);

  m_filter = new CouponFilter;
  m_tabDb->addTab(m_filter, QIcon(":/icons/res/filter.png"), tr("Filtro"));

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltMain);
  m_tab->addTab(tabframe, QIcon(":/icons/res/coupon.png"), tr("Cupom"));

  m_lblCount = new QLabel;
  m_viewer->layout()->addWidget(m_lblCount);

  connect(m_cbExpiration, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoPercentage, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoValue, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoProduct, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_rdoPercentage, SIGNAL(clicked(bool)), m_edPercentage, SLOT(setFocus()));
  connect(m_rdoValue, SIGNAL(clicked(bool)), m_edValue, SLOT(setFocus()));
  connect(m_rdoProduct, SIGNAL(clicked(bool)), m_btnAddRemove->m_btnAdd, SLOT(setFocus()));
  connect(m_viewer, &DatabaseViewer::refreshSignal, m_lblCount, [=](){ m_lblCount->setText(tr("Número de cupons: %1").arg(m_viewer->getRowCount())); });
  connect(m_filter, SIGNAL(filterChangedSignal(const QString&)), m_viewer, SLOT(setDynamicFilter(const QString&)));

  setFocusWidgetOnClear(m_edPercentage);
  m_viewer->refresh();
  clear();
}

void CouponView::getItem(JItemSQL& o) const
{
  Coupon& _o = dynamic_cast<Coupon&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_code = m_edCode->text();
  if (m_rdoPercentage->isChecked())
    _o.m_type = Coupon::Type::Percentage;
  else if (m_rdoValue->isChecked())
    _o.m_type = Coupon::Type::Value;
  else if (m_rdoProduct->isChecked())
    _o.m_type = Coupon::Type::Product;
  _o.m_dtCreation = DateTime::server();
  _o.m_bExpires = m_cbExpiration->isChecked();
  _o.m_dtExpiration = m_dtExpiration->getDate();
  _o.m_percentage = m_edPercentage->value();
  _o.m_value = m_edValue->value();
  _o.m_store.m_id = m_storePicker->getFirstId();
  m_table->get(_o.m_products);
}

void CouponView::setItem(const JItemSQL& o)
{
  const Coupon& _o = static_cast<const Coupon&>(o);
  m_lblRedeemed->setText(tr("Código resgatado no dia %1").arg(_o.m_dtRedeemed.toString("dd/MM/yyyy hh:mm:ss")));
  m_lblRedeemed->setVisible(_o.m_bRedeemed);
  m_edCode->setReadOnly(_o.m_id.isValid());
  m_edCode->setText(_o.m_code);
  m_rdoPercentage->setChecked(_o.m_type == Coupon::Type::Percentage);
  m_rdoValue->setChecked(_o.m_type == Coupon::Type::Value);
  m_rdoProduct->setChecked(_o.m_type == Coupon::Type::Product);
  m_cbExpiration->setChecked(_o.m_bExpires);
  m_dtExpiration->setDate(_o.m_dtExpiration);
  m_edPercentage->setValue(_o.m_percentage);
  m_edValue->setValue(_o.m_value);
  m_tab->setTabEnabled(0, !_o.m_bRedeemed);
  m_btnSave->setEnabled(!_o.m_bRedeemed);
  m_storePicker->addItem(_o.m_store);
  m_table->set(_o.m_products, true);
  if (!_o.m_id.isValid() && !_o.m_store.m_id.isValid())
  {
    QSettings settings(SETTINGS_COMPANY_NAME, SETTINGS_APP_NAME);
    Store store(Id(settings.value(SETTINGS_COUPON_STORE_ID).toLongLong()));
    QString error;
    if (store.SQL_select(error))
      m_storePicker->addItem(store);
  }
  updateControls();
}

void CouponView::updateControls()
{
  m_dtExpiration->setEnabled(m_cbExpiration->isChecked());
  m_edPercentage->setEnabled(m_rdoPercentage->isChecked());
  m_edValue->setEnabled(m_rdoValue->isChecked());
  if (!m_rdoPercentage->isChecked())
    m_edPercentage->setValue(0.0);
  if (!m_rdoValue->isChecked())
    m_edValue->setValue(0.0);
  m_table->setEnabled(m_rdoProduct->isChecked());
  m_btnAddRemove->setEnabled(m_rdoProduct->isChecked());
}

bool CouponView::save(Id& id)
{
  Coupon c;
  if (!JItemHelper::authenticateSave(c))
    return false;

  id.clear();
  bool ok = true;
  int n = 1;
  if (!m_id.isValid())
    n = QInputDialog::getInt(this, tr("Gerar Cupons"), tr("Informe o número de cupons a serem gerados:"), 0, 1, 9999, 1, &ok);
  if (ok)
  {
    QVector<Coupon> coupons;
    Coupon o;
    getItem(o);
    for (int i = 0; i != n; ++i)
    {
      if (!m_id.isValid())
      {
        o.m_code.clear();
        o.m_code = m_edCode->text().isEmpty() ? Coupon::st_newCode() : m_edCode->text() + (n == 1 ? "" : Data::strInt(i + 1));
      }
      coupons.append(o);
    }
    st_saveMultiple(coupons, this);
    clear();
    CouponConfirmation dlg(coupons, this);
    if (dlg.exec())
      for (int i = 0; i != coupons.size(); ++i)
        JItemHelper::print(coupons.at(i), nullptr, this);
  }

  return true;
}

bool CouponView::st_saveMultiple(QVector<Coupon>& v, QWidget* parent)
{
  QString error;

  if (!JItemSQL::SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  const int count = v.count();
  QProgressDialog bar(tr("Gerando Cupons"), tr("Cancelar"), 0, count, parent);
  bar.setWindowModality(Qt::WindowModal);
  for (int i = 0; i != v.size(); ++i)
  {
    bar.setValue(i);
    bool ok = v.at(i).SQL_insert_update_proc(query);
    if (!ok)
      v.remove(i--);
    else
      v[i].m_store.SQL_select_proc(query, error);
    if (bar.wasCanceled())
    {
      v.remove(i + 1, v.size() - i - 1);
      break;
    }
  }
  bar.setValue(count);

  return JItemSQL::SQL_finish(db, query, true, error);
}

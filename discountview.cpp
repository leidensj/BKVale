#include "discountview.h"
#include "widgets/jlineedit.h"
#include "widgets/jdoublespinbox.h"
#include "widgets/jdatabase.h"
#include "widgets/jspinbox.h"
#include "discounttablewidget.h"
#include "databaseutils.h"
#include "printutils.h"
#include <QGroupBox>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QMessageBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QDateEdit>
#include <QPlainTextEdit>
#include <QDockWidget>
#include <QInputDialog>
#include <QFormLayout>
#include <QLabel>

DiscountView::DiscountView(QWidget* parent)
  : QFrame(parent)
  , m_btnCreate(nullptr)
  , m_btnSearch(nullptr)
  , m_btnRedeem(nullptr)
  , m_lblCode(nullptr)
  , m_edCode(nullptr)
  , m_cbExpires(nullptr)
  , m_dtExp(nullptr)
  , m_rdValue(nullptr)
  , m_rdPercentage(nullptr)
  , m_rdProduct(nullptr)
  , m_spnValue(nullptr)
  , m_spnPercentage(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_table(nullptr)
  , m_database(nullptr)
  , m_lblRedeemed(nullptr)
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

  m_btnRedeem = new QPushButton;
  m_btnRedeem->setFlat(true);
  m_btnRedeem->setText("");
  m_btnRedeem->setIconSize(QSize(24, 24));
  m_btnRedeem->setIcon(QIcon(":/icons/res/redeem.png"));
  m_btnRedeem->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));

  m_lblCode = new QLabel;
  m_lblCode->setText(tr("Código:"));
  m_edCode = new JLineEdit(JLineEdit::Input::Alphanumeric, JLineEdit::st_defaultFlags2);
  m_edCode->setPlaceholderText(tr("*"));
  m_edCode->setMaxLength(DISCOUNT_CODE_MAX_LENGTH);
  m_edCode->setReadOnly(true);

  m_cbExpires = new QCheckBox;
  m_cbExpires->setText(tr("Expira em:"));

  m_dtExp = new QDateEdit;
  m_dtExp->setCalendarPopup(true);
  m_dtExp->setDate(QDate::currentDate());

  m_rdValue = new QRadioButton;
  m_rdValue->setText(tr("Valor:"));

  m_rdPercentage = new QRadioButton;
  m_rdPercentage->setText(tr("Porcentagem:"));

  m_rdProduct = new QRadioButton;
  m_rdProduct->setText(tr("Produto:"));

  m_spnValue = new JDoubleSpinBox;
  m_spnValue->setMinimum(0.0);
  m_spnValue->setMaximum(99999999.0);
  m_spnValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

  m_spnPercentage = new JDoubleSpinBox;
  m_spnPercentage->setMinimum(0.0);
  m_spnPercentage->setMaximum(100.0);
  m_spnPercentage->setSuffix("%");

  m_btnAdd = new QPushButton;
  m_btnAdd->setFlat(true);
  m_btnAdd->setText("");
  m_btnAdd->setIconSize(QSize(24, 24));
  m_btnAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAdd->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));
  m_btnAdd->setToolTip(tr("Adicionar item (Alt++)"));

  m_btnRemove = new QPushButton;
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Minus));
  m_btnRemove->setToolTip(tr("Remover item (Alt+-)"));

  m_table = new DiscountTableWidget;

  m_lblRedeemed = new QLabel;
  m_lblRedeemed->setText(tr("Desconto já resgatado."));
  {
    QFont font = m_lblRedeemed->font();
    font.setBold(true);
    m_lblRedeemed->setFont(font);
    QPalette palette = m_lblRedeemed->palette();
    palette.setColor(QPalette::ColorRole::Foreground, Qt::red);
    m_lblRedeemed->setPalette(palette);
  }

  m_teDescription = new QPlainTextEdit;

  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->setContentsMargins(0, 0, 0, 0);
  buttonLayout->addWidget(m_btnCreate);
  buttonLayout->addWidget(m_btnSearch);
  buttonLayout->addWidget(m_btnRedeem);
  buttonLayout->setAlignment(Qt::AlignLeft);

  QFormLayout* formLayout = new QFormLayout;
  formLayout->setContentsMargins(0, 0, 0, 0);
  formLayout->addRow(m_lblCode, m_edCode);
  formLayout->addRow(m_cbExpires, m_dtExp);
  formLayout->addRow(m_rdValue, m_spnValue);
  formLayout->addRow(m_rdPercentage, m_spnPercentage);

  QHBoxLayout* productButtonsLayout = new QHBoxLayout;
  productButtonsLayout->setContentsMargins(0, 0, 0, 0);
  productButtonsLayout->setAlignment(Qt::AlignLeft);
  productButtonsLayout->addWidget(m_btnAdd);
  productButtonsLayout->addWidget(m_btnRemove);

  QVBoxLayout* productLayout = new QVBoxLayout;
  productLayout->setContentsMargins(0, 0, 0, 0);
  productLayout->setAlignment(Qt::AlignTop);
  productLayout->addLayout(productButtonsLayout);
  productLayout->addWidget(m_table);

  QFrame* productFrame = new QFrame;
  productFrame->setLayout(productLayout);

  formLayout->addRow(m_rdProduct, productFrame);
  formLayout->addRow(tr("Descrição:"), m_teDescription);

  QVBoxLayout* viewLayout = new QVBoxLayout;
  viewLayout->setAlignment(Qt::AlignTop);
  viewLayout->addLayout(buttonLayout);
  viewLayout->addWidget(m_lblRedeemed);
  viewLayout->addLayout(formLayout);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewLayout);

  m_database = new JDatabase(DISCOUNT_SQL_TABLE_NAME);

  m_dock = new QDockWidget;
  m_dock->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  m_dock->setFeatures(0);
  m_dock->setFeatures(QDockWidget::DockWidgetClosable);
  m_dock->setWindowTitle(tr("Pesquisar"));
  m_dock->setWidget(m_database);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_dock);
  splitter->addWidget(viewFrame);

  QVBoxLayout* mainLayout = new QVBoxLayout();
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  m_dock->hide();

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItemSQL&)),
                   this,
                   SLOT(itemSelected(const JItemSQL&)));
  QObject::connect(m_database,
                   SIGNAL(itemsRemovedSignal(const QVector<Id>&)),
                   this,
                   SLOT(itemsRemoved(const QVector<Id>&)));
  QObject::connect(m_rdValue,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_rdPercentage,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_rdProduct,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbExpires,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_btnAdd,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(searchProduct()));
  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeProduct()));
  QObject::connect(m_table,
                   SIGNAL(productSignal(const Product&)),
                   this,
                   SLOT(searchProduct()));
  QObject::connect(m_btnSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(showSearch()));
  QObject::connect(m_btnRedeem,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(redeem()));
  create();
}

void DiscountView::itemSelected(const JItemSQL& jItem)
{
  const Discount& o = dynamic_cast<const Discount&>(jItem);
  if (o.m_id.isValid())
    setDiscount(o);
}

void DiscountView::itemsRemoved(const QVector<Id>& ids)
{
  if (ids.contains(m_currentId))
    create();
}

void DiscountView::create()
{
  setDiscount(Discount());
}

Discount DiscountView::getDiscount() const
{
  Discount o;
  o.m_id = m_currentId;
  o.m_code = m_edCode->text();
  o.m_bExpires = m_cbExpires->isChecked();
  o.m_dtExp = m_dtExp->date();
  o.m_type = m_rdPercentage->isChecked()
             ? Discount::Type::Percentage
             : m_rdValue->isChecked()
             ? Discount::Type::Value
             : m_rdProduct->isChecked()
             ? Discount::Type::Product
             : Discount::Type::None;
  o.m_value = m_spnValue->value();
  o.m_percentage = m_spnPercentage->value();
  o.m_items = m_table->getDiscountItems();
  o.m_bRedeemed = m_lblRedeemed->isVisible();
  o.m_description = m_teDescription->toPlainText();
  return o;
}

void DiscountView::setDiscount(const Discount &o)
{
  m_currentId = o.m_id;
  m_lblRedeemed->setVisible(o.m_bRedeemed);
  m_edCode->setText(o.m_code);
  m_cbExpires->setChecked(o.m_bExpires);
  m_dtExp->setDate(o.m_dtExp);
  m_rdValue->setChecked(o.m_type == Discount::Type::Value);
  m_rdPercentage->setChecked(o.m_type == Discount::Type::Percentage);
  m_rdProduct->setChecked(o.m_type == Discount::Type::Product);
  m_spnValue->setValue(o.m_value);
  m_spnPercentage->setValue(o.m_percentage);
  m_table->setDiscountItems(o.m_items);
  m_teDescription->setPlainText(o.m_description);
  updateControls();
}

void DiscountView::updateControls()
{
  m_lblCode->setVisible(!m_edCode->text().isEmpty());
  m_edCode->setVisible(!m_edCode->text().isEmpty());
  const bool bRedeemed = m_lblRedeemed->isVisible();
  m_edCode->setEnabled(!bRedeemed);
  m_cbExpires->setEnabled(!bRedeemed);
  m_rdValue->setEnabled(!bRedeemed);
  m_rdPercentage->setEnabled(!bRedeemed);
  m_rdProduct->setEnabled(!bRedeemed);
  m_teDescription->setEnabled(!bRedeemed);

  m_spnValue->setEnabled(!bRedeemed && m_rdValue->isChecked());
  m_spnPercentage->setEnabled(!bRedeemed && m_rdPercentage->isChecked());
  m_btnAdd->setEnabled(!bRedeemed && m_rdProduct->isChecked());
  m_btnRemove->setEnabled(!bRedeemed && m_rdProduct->isChecked());
  m_table->setEnabled(!bRedeemed && m_rdProduct->isChecked());
  m_dtExp->setEnabled(!bRedeemed && m_cbExpires->isChecked());
  if (sender() == m_rdValue)
  {
    m_spnValue->setFocus();
    m_spnValue->selectAll();
  }
  if (sender() == m_rdPercentage)
  {
    m_spnPercentage->setFocus();
    m_spnPercentage->selectAll();
  }
}

void DiscountView::setProduct(const Product& product, bool bNewProduct)
{
  if (bNewProduct)
  {
    DiscountItem o;
    o.m_product = product;
    m_table->addDiscountItem(o);
    m_table->setFocus();
  }
  else
  {
    m_table->setProduct(product);
  }
  updateControls();
}

void DiscountView::searchProduct()
{
  JDatabaseSelector dlg(PRODUCT_SQL_TABLE_NAME);
  dlg.getDatabase()->setFixedFilter(PRODUCT_FILTER_SELL);
  dlg.exec();
  Product* p = static_cast<Product*>(dlg.getDatabase()->getCurrentItem());
  if (p != nullptr && p->m_id.isValid())
    setProduct(*p, m_btnAdd == sender());
}

void DiscountView::removeProduct()
{
  m_table->removeCurrentItem();
  updateControls();
}

void DiscountView::showSearch()
{
  if (m_dock->isVisible())
    m_dock->close();
  else
    m_dock->show();
}

void DiscountView::redeem()
{
  bool ok = false;
  QString code = QInputDialog::getText(this,
                                       tr("Resgatar Código"),
                                       tr("Informe o código de desconto:"),
                                       QLineEdit::Normal,
                                       "", &ok);

  if (!ok || code.isEmpty())
    return;

  bool redeemed = false;
  Discount o;
  QString error;
  bool bSuccess = o.SQL_redeem(code, redeemed, error);
  if (bSuccess)
  {
    if (redeemed)
    {
      bSuccess = false;
      //TODO permitir usuarios resgatarem codigos resgatados
      error = tr("Código já resgatado.");
    }
    else if (o.m_bExpires && BaitaSQL::getDate(true) > o.m_dtExp)
    {
      bSuccess = false;
      error = tr("Código expirado.");
    }
  }

  if (!bSuccess)
    QMessageBox::critical(this, tr("Erro ao resgatar código"), error, QMessageBox::Ok);
  else
    emit redeemSignal(DiscountPrinter::buildRedeem(o));
}

Discount DiscountView::save()
{
  Discount o = getDiscount();
  bool bSuccess = m_database->save(o);
  if (bSuccess)
  {
    QString error;
    if (o.SQL_select(error))
      create();
    else
      o.clear();
  }
  else
    o.clear();
  updateControls();
  return o;
}

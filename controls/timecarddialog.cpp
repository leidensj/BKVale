#include "timecarddialog.h"
#include <QDate>
#include "items/jitem.h"
#include "items/store.h"
#include "items/employee.h"
#include "controls/databasepicker.h"
#include "widgets/jspinbox.h"
#include "widgets/filegenerator.h"
#include <QDateEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QIcon>
#include <QFileDialog>
#include <QLocale>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>
#include "tables/dayofftable.h"

TimeCardDialog::TimeCardDialog(QWidget* parent)
 : QDialog(parent)
 , m_storePicker(nullptr)
 , m_date(nullptr)
 , m_buttons(nullptr)
 , m_spnExtraPages(nullptr)
 , m_cbOpenFile(nullptr)
 , m_dayOffTable(nullptr)
 , m_btnCSV(nullptr)
 , m_btnComplete(nullptr)
 , m_btnSwap(nullptr)
 , m_lblMessage(nullptr)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(tr("Livro Ponto"));
  setWindowIcon(QIcon(":/icons/res/timecard.png"));
  m_storePicker = new DatabasePicker(STORE_SQL_TABLE_NAME);
  m_date = new QDateEdit(QDate::currentDate());
  m_date->setDisplayFormat("MMMM yyyy");

  m_buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

  m_spnExtraPages = new JSpinBox;
  m_spnExtraPages->setMinimum(0);
  m_spnExtraPages->setMaximum(9999);

  m_cbOpenFile = new QCheckBox;
  m_cbOpenFile->setText(tr("Abrir arquivo automaticamente ao finalizar"));
  m_cbOpenFile->setCheckState(Qt::Checked);

  connect(m_buttons, SIGNAL(accepted()), this, SLOT(saveAndAccept()));
  connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));

  QFormLayout* formLayout = new QFormLayout;
  formLayout->addRow(tr("Loja:"), m_storePicker);
  formLayout->addRow(tr("Data:"), m_date);
  formLayout->addRow(tr("Páginas extras:"), m_spnExtraPages);

  m_btnCSV = new QPushButton;
  m_btnCSV->setFlat(true);
  m_btnCSV->setIconSize(QSize(24, 24));
  m_btnCSV->setToolTip(tr("Exportar para CSV"));
  m_btnCSV->setIcon(QIcon(":/icons/res/csv_export.png"));

  m_btnComplete = new QPushButton;
  m_btnComplete->setFlat(true);
  m_btnComplete->setIconSize(QSize(24, 24));
  m_btnComplete->setToolTip(tr("Gerar folgas"));
  m_btnComplete->setIcon(QIcon(":/icons/res/shuffle.png"));

  m_btnSwap = new QPushButton;
  m_btnSwap->setFlat(true);
  m_btnSwap->setIconSize(QSize(24, 24));
  m_btnSwap->setToolTip(tr("Trocar folgas"));
  m_btnSwap->setIcon(QIcon(":/icons/res/swap.png"));

  m_lblMessage = new QLabel;

  QHBoxLayout* ltTable = new QHBoxLayout;
  ltTable->setAlignment(Qt::AlignLeft);
  ltTable->addWidget(m_btnCSV);
  ltTable->addWidget(m_btnComplete);
  ltTable->addWidget(m_btnSwap);
  ltTable->addWidget(m_lblMessage);

  m_dayOffTable = new DayOffTable;

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(formLayout);
  ltMain->addWidget(m_cbOpenFile);
  ltMain->addLayout(ltTable);
  ltMain->addWidget(m_dayOffTable);
  ltMain->addWidget(m_buttons);
  setLayout(ltMain);

  connect(m_storePicker, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_date, SIGNAL(dateChanged(const QDate&)), this, SLOT(updateControls()));
  connect(m_btnCSV, SIGNAL(clicked(bool)), this, SLOT(saveDayOff()));
  connect(m_btnComplete, SIGNAL(clicked(bool)), this, SLOT(shuffle()));
  connect(m_btnSwap, SIGNAL(clicked(bool)), m_dayOffTable, SLOT(shuffleCurrentRow()));
  connect(m_dayOffTable, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)), this, SLOT(updateMessageAndSwapButton()));
  connect(m_dayOffTable, SIGNAL(changedSignal(bool)), this, SLOT(updateMessageAndSwapButton()));
  updateControls();
}

void TimeCardDialog::shuffle()
{
  int ret = QMessageBox::question(this, tr("Gerar folgas"), tr("Deseja gerar as folgas automaticamente?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No);
  if (ret == QMessageBox::Yes)
    m_dayOffTable->shuffle();
}

void TimeCardDialog::saveDayOff()
{
  Store o(m_storePicker->getFirstId());
  QString error;
  // TODO
  if (!o.m_id.isValid() || !o.SQL_select(error))
    return;

  QDate dt = m_date->date().addDays(-1 * m_date->date().day() + 1);
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Salvar folgas"),
                                                  "/desktop/folgas_" +
                                                  dt.toString("yyyy") + "_" +
                                                  dt.toString("MM") +
                                                  "_" +
                                                  o.m_form.strAliasName().replace(" ", "") +
                                                  ".csv",
                                                  tr("CSV (*.csv)"));
  if (fileName.isEmpty())
    return;

  QString csv = m_dayOffTable->toCSV();

  CsvGenerator* w = new CsvGenerator(fileName, csv, tr("Gerando arquivo csv:"), true);
  w->generate();
}

void TimeCardDialog::updateControls()
{
  QPushButton* pt = m_buttons->button(QDialogButtonBox::Save);
  if (pt != nullptr)
    pt->setEnabled(m_storePicker->getFirstId().isValid());
  m_btnCSV->setEnabled(m_storePicker->getFirstId().isValid());
  m_btnComplete->setEnabled(m_storePicker->getFirstId().isValid());

  m_lblMessage->setText("");
  Store o(m_storePicker->getFirstId());
  QString error;
  if (!o.m_id.isValid() || !o.SQL_select(error))
  {
    m_dayOffTable->clearAll();
    return;
  }
  m_dayOffTable->setStore(o, m_date->date());
}

void TimeCardDialog::saveAndAccept()
{
  Store o(m_storePicker->getFirstId());
  QString error;
  // TODO
  if (!o.m_id.isValid() || !o.SQL_select(error))
    return;

  QDate dt(m_date->date());
  QString html;
  QDate idt = dt.addDays(-1 * dt.day() + 1);
  QDate fdt = idt.addDays(idt.daysInMonth() - 1);

  /* 1 - Nome loja
   * 2 - Data inicial "dd/MM/yyyy"
   * 3 - Data final "dd/MM/yyyy"
   * 4 - Razão social
   * 5 - Endereço
   * 6 - CEP
   * 7 - Cidade / UF */

  // TODO: solução temporaria
  Address address;
  if (!o.m_form.m_vAddress.isEmpty())
    address = o.m_form.m_vAddress.at(0);

  html = QString(
      "<html>"
        "<body>"
          "<table align=\"center\" width=\"100%\" height=\"100%\" style=\"page-break-after:always;\">"
            "<tr><td align=\"center\" style=\"padding:80px;font-size:48pt;\"><font face=\"verdana\">%1</font></td></tr>"
            "<tr><td align=\"center\" style=\"font-size:36pt;\">Livro Ponto</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:36pt;\">%2</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:36pt;\">A</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:36pt;\">%3</td></tr>"
            "<tr><td align=\"center\" style=\"padding-top:100px;font-size:18pt;\">%4</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:14pt;\">%5</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:14pt;\">%6</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:14pt;\">%7</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:14pt;\">%8</td></tr>"
          "</table>").arg(o.m_form.strAliasName(),
                          idt.toString("dd/MM/yyyy"),
                          fdt.toString("dd/MM/yyyy"),
                          o.m_form.m_name,
                          o.m_form.m_CPF_CNPJ.isEmpty() ? "" : "CNPJ: " + o.m_form.m_CPF_CNPJ,
                          address.m_street.isEmpty() ? "" : address.m_street + ", " + QString::number(address.m_number),
                          address.m_cep.isEmpty() ? "" : "CEP: " + address.m_cep,
                          address.m_city.isEmpty() ? "" : address.m_city + " " + address.getBRState().m_abv);

  // página em branco
  html += "<p style=\"page-break-after: always;\">&nbsp;</p>";

  QLocale br(QLocale::Portuguese, QLocale::Brazil);

  QVector<QVector<bool>> dayOff;
  m_dayOffTable->getDayOff(dayOff);
  for (int i = 0; i != o.m_vEmployee.size() + m_spnExtraPages->value(); ++i)
  {
    // 1 - Nome funcionário
    // 2 - Horário
    // 3 - MÊS
    // 4 - ANO
    QString title("REGISTRO PONTO");
    QString page(QString::number(i + 1));
    while (title.length() < (42 - page.length()))
      title += " ";
    title += page;
    html += QString(
      "<pre style=\"font-size:16pt;\" align=\"left\"><b>" + title + "</b></pre>"
      "<p>Nome: %1 Horário: %2</p>"
      "<table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: gray;\">"
        "<tr><th colspan=\"2\">%3 %4</th>"
          "<th colspan=\"3\">ENTRADA</th>"
          "<th colspan=\"3\">SAIDA</th>"
        "</tr>"
        "<tr>"
          "<th>Dia</th>"
          "<th>Semana</th>"
          "<th>Assinatura</th>"
          "<th colspan=\"2\">Hora</th>"
          "<th>Assinatura</th>"
          "<th colspan=\"2\">Hora</th>"
        "</tr>").arg(i >= o.m_vEmployee.size()
                     ? "_____________________________________"
                     : o.m_vEmployee.at(i).m_form.m_name,
                     i >= o.m_vEmployee.size()
                     ? "______________________________________"
                     : o.m_vEmployee.at(i).strHours(),
                     br.toString(idt, "MMMM").toUpper(),
                     idt.toString("yyyy"));
    dt = idt;
    const int daysInMonth = dt.daysInMonth();
    for (int j = 0; j != daysInMonth; ++j)
    {
      // - Data dd
      // - Data dddd
      html += QString(
        "<tr>"
         "<td width=\"5%\">%1</td>"
           "<td width=\"15%\" bgcolor=\"%2\">%3</td>"
           "<td width=\"30%\" align=\"center\">%4</td>"
           "<td width=\"5%\" align=\"center\">%5</td>"
           "<td width=\"5%\" align=\"center\">%6</td>"
           "<td width=\"30%\" align=\"center\">%7</td>"
           "<td width=\"5%\" align=\"center\">%8</td>"
           "<td width=\"5%\" align=\"center\">%9</td>"
         "</tr>").arg(dt.toString("dd"),
                      dt.dayOfWeek() == 7 ? "gray" : "white",
                      br.toString(dt, "dddd"),
                      i < dayOff.size() ? (dayOff.at(i).at(j) ? "FOLGA" : "") : "",
                      i < dayOff.size() ? (dayOff.at(i).at(j) ? "-" : "") : "",
                      i < dayOff.size() ? (dayOff.at(i).at(j) ? "-" : "") : "",
                      i < dayOff.size() ? (dayOff.at(i).at(j) ? "FOLGA" : "") : "",
                      i < dayOff.size() ? (dayOff.at(i).at(j) ? "-" : "") : "",
                      i < dayOff.size() ? (dayOff.at(i).at(j) ? "-" : "") : "");
      dt = dt.addDays(1);
    }

    html += QString(
      "</table>"
      "<br>"
      "<table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: gray;%1\">"
      "<tr>"
      "<td width=\"40%\">TOTAL DE HORAS NORMAIS</td>"
      "<td width=\"15%\">(H.N.)</td>"
      "<td width=\"45%\"></td>"
      "</tr>"
      "<tr>"
      "<td width=\"40%\">TOTAL DE HORAS FALTAS</td>"
      "<td width=\"15%\">(H.F.)</td>"
      "<td width=\"45%\"></td>"
      "</tr>"
      "<tr>"
      "<td width=\"40%\">HORAS NOTURNAS</td>"
      "<td width=\"15%\">(H.Not.)</td>"
      "<td width=\"45%\"></td>"
      "</tr>"
      "<tr>"
      "<td width=\"40%\">HORAS ADICIONAIS NOTURNAS</td>"
      "<td width=\"15%\">(H.Ad.Not.)</td>"
      "<td width=\"45%\"></td>"
      "</tr>"
      "<tr>"
      "<td width=\"40%\">DESCANSO SEMANAL REMUNERADO</td>"
      "<td width=\"15%\">(D.R.S)</td>"
      "<td width=\"45%\"></td>"
      "</tr>"
      "<tr>"
      "<td width=\"40%\">TOTAL DE HORAS EXTRAS 50%</td>"
      "<td width=\"15%\">(H.E.)</td>"
      "<td width=\"45%\"></td>"
      "</tr>"
      "</tr>"
      "</table>").arg(i == (o.m_vEmployee.size() + m_spnExtraPages->value() - 1) ? "" : "page-break-after:always;");
  }

  html +=
    "</body>"
    "</html>";

  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Salvar livro ponto"),
                                                  "/desktop/livro_" +
                                                  idt.toString("yyyy") + "_" +
                                                  idt.toString("MM") +
                                                  "_" +
                                                  o.m_form.strAliasName().replace(" ", "") +
                                                  ".pdf",
                                                  tr("PDF (*.pdf)"));

  if (fileName.isEmpty())
    return;

  accept();
  PdfGenerator* w = new PdfGenerator(fileName, html, tr("Gerando livro ponto:"), m_cbOpenFile->isChecked(), false);
  w->generate();
}

void TimeCardDialog::updateMessageAndSwapButton()
{
  m_btnSwap->setEnabled(m_dayOffTable->isValidCurrentRow());
  m_lblMessage->setText(m_dayOffTable->message());
}

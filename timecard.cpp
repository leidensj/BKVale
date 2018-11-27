#include "timecard.h"
#include <QTextDocument>
#include <QPrinter>
#include <QDate>
#include "jitem.h"
#include "store.h"
#include "jdatabasepicker.h"
#include <QDateEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QIcon>
#include <QFileDialog>
#include <QLocale>
#include <QPushButton>
#include "jdatabase.h"
#include <QDesktopServices>
#include <QCheckBox>
#include <QProgressDialog>

TimeCard::TimeCard(QWidget* parent)
 : QDialog(parent)
 , m_storePicker(nullptr)
 , m_date(nullptr)
 , m_buttons(nullptr)
 , m_cbOpenFile(nullptr)
{
  setWindowTitle(tr("Livro Ponto"));
  setWindowIcon(QIcon(":/icons/res/timecard.png"));
  m_storePicker = new JDatabasePicker(STORE_SQL_TABLE_NAME,
                                      tr("Loja"),
                                      QIcon(":/icons/res/store.png"));
  m_date = new QDateEdit(QDate::currentDate());
  m_date->setDisplayFormat("MMMM yyyy");

  m_buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

  m_cbOpenFile = new QCheckBox;
  m_cbOpenFile->setText(tr("Abrir arquivo automaticamente ao finalizar."));
  m_cbOpenFile->setCheckState(Qt::Checked);

  connect(m_buttons, SIGNAL(accepted()), this, SLOT(saveAndAccept()));
  connect(m_buttons, SIGNAL(rejected()), this, SLOT(reject()));

  QFormLayout* formLayout = new QFormLayout;
  formLayout->addRow(tr("Loja:"), m_storePicker);
  formLayout->addRow(tr("Data:"), m_date);

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(formLayout);
  ltMain->addWidget(m_cbOpenFile);
  ltMain->addWidget(m_buttons);
  setLayout(ltMain);

  connect(m_storePicker, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  updateControls();
}

void TimeCard::updateControls()
{
  QPushButton* pt = m_buttons->button(QDialogButtonBox::Save);
  if (pt != nullptr)
    pt->setEnabled(m_storePicker->getId().isValid());
}

void TimeCard::saveAndAccept()
{

  auto pt = static_cast<Store*>(m_storePicker->getDatabase()->getCurrentItem());
  Store o;
  if (pt != nullptr)
    o = *pt;

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
  html = QString(
      "<html>"
        "<body>"
          "<table align=\"center\" width=\"100%\" height=\"100%\" style=\"page-break-after:always;\">"
            "<tr><td align=\"center\" style=\"padding:80px;font-size:48pt;\"><font face=\"verdana\">%1</font></td></tr>"
            "<tr><td align=\"center\" style=\"font-size:36pt;\">Livro Ponto</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:36pt;\">%2</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:36pt;\">A</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:36pt;\">%3</td></tr>"
            "<tr><td align=\"center\" style=\"padding-top:80px;font-size:18pt;\">%4</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:14pt;\">%5</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:14pt;\">%6</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:14pt;\">%7</td></tr>"
            "<tr><td align=\"center\" style=\"font-size:14pt;\">%8</td></tr>"
          "</table>").arg(o.m_name,
                          idt.toString("dd/MM/yyyy"),
                          fdt.toString("dd/MM/yyyy"),
                          o.m_person.m_name,
                          o.m_person.m_vAddress.isEmpty() ? "" : o.m_person.m_vAddress.at(0).getFormattedAddress2(),
                          o.m_person.m_vAddress.isEmpty() ? "" : "CEP: " + o.m_person.m_vAddress.at(0).m_cep,
                          o.m_person.m_vAddress.isEmpty() ? "" : o.m_person.m_vAddress.at(0).getFormattedAddress3(),
                          "CNPJ: " + o.m_person.m_CPF_CNPJ);

  QLocale br(QLocale::Portuguese, QLocale::Brazil);

  for (int i = 0; i != o.m_vEmployee.size(); ++i)
  {
    /* 1 - Página
     * 2 - Nome funcionário
     * 3 - Horário
     * 4 - MÊS
     * 5 - ANO */
    html += QString(
      "<p align=\"right\">%1</p>"
      "<h2 align=\"center\">REGISTRO PONTO</h2>"
      "<p>Nome: %2 Horário: %3</p>"
      "<table border=\"1\" align=\"center\" width=\"100%\">"
        "<tr><th colspan=\"2\">%4 %5</th>"
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
        "</tr>").arg(QString::number(i + 1),
                     o.m_vEmployee.at(i).m_employee.m_name,
                     o.m_vEmployee.at(i).strHours(),
                     br.toString(idt, "MMMM").toUpper(),
                     idt.toString("yyyy"));
    dt = idt;
    const int daysInMonth = dt.daysInMonth();
    for (int i = 0; i != daysInMonth; ++i)
    {
      /* 1 - Data dd
       * 2 - Data dddd */
      html += QString(
        "<tr>"
         "<td width=\"5%\">%1</td>"
           "<td width=\"15%\">%2</td>"
           "<td width=\"30%\"></td>"
           "<td width=\"5%\"></td>"
           "<td width=\"5%\"></td>"
           "<td width=\"30%\"></td>"
           "<td width=\"5%\"></td>"
           "<td width=\"5%\"></td>"
         "</tr>").arg(dt.toString("dd"),
                      br.toString(dt, "dddd"));
      dt = dt.addDays(1);
    }

    html += QString(
      "</table>"
      "<br>"
      "<table border=\"1\" cellpadding=\"2\" align=\"center\" width=\"100%\" style=\"border-style:solid;%1\">"
      "<tr>"
      "<td width=\"40%\">TOTAL DE HORAS NORMAIS</td>"
      "<td width=\"10%\">(H.N.)</td>"
      "<td width=\"50%\"></td>"
      "</tr>"
      "<tr>"
      "<td width=\"40%\">TOTAL DE HORAS EXTRAS 50%</td>"
      "<td width=\"10%\">(H.E.)</td>"
      "<td width=\"50%\"></td>"
      "</tr>"
      "<tr>"
      "<td width=\"40%\">TOTAL DE HORAS FALTAS</td>"
      "<td width=\"10%\">(H.F.)</td>"
      "<td width=\"50%\"></td>"
      "</tr>"
      "<tr>"
      "<td width=\"40%\">HORAS NOTURNAS</td>"
      "<td width=\"10%\">(H.Not.)</td>"
      "<td width=\"50%\"></td>"
      "</tr>"
      "<tr>"
      "<td width=\"40%\">HORAS ADICIONAIS NOTURNAS</td>"
      "<td width=\"10%\">(H.Ad.Not.)</td>"
      "<td width=\"50%\"></td>"
      "</tr>"
      "<tr>"
      "<td width=\"40%\">DESCANSO SEMANAL REMUNERADO</td>"
      "<td width=\"10%\">(D.R.S)</td>"
      "<td width=\"50%\"></td>"
      "</tr>"
      "</tr>"
      "</table>").arg(i == (o.m_vEmployee.size() - 1) ? "" : "page-break-after:always;");
  }

  html +=
    "</body>"
    "</html>";

  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Salvar livro ponto"),
                                                  "/desktop/livro_" + idt.toString("yyyy") + "_" + idt.toString("MM") + ".pdf",
                                                  tr("PDF (*.pdf)"));

  QProgressDialog progress(tr("Gerando livro ponto..."), QString(), 0, 3, this);
  progress.setWindowModality(Qt::WindowModal);

  QTextDocument doc;
  doc.setHtml(html);

  progress.setValue(1);

  doc.setDocumentMargin(20);
  QPrinter printer(QPrinter::PrinterResolution);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPaperSize(QPrinter::A4);
  printer.setOutputFileName(fileName);
  doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
  progress.setValue(2);
  doc.print(&printer);
  progress.setValue(3);
  accept();
  if (m_cbOpenFile->isChecked())
    QDesktopServices::openUrl(QUrl(fileName, QUrl::TolerantMode));
}

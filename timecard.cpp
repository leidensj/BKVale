#include "timecard.h"
#include <QTextDocument>
#include <QPrinter>
#include <QDate>
#include "jitem.h"

TimeCard::TimeCard(QWidget* parent)
 : QFrame(parent)
{
  QDate dt(QDate::currentDate());
  QString html;
  html = "<html>"
                 "<body>"
                 "<table align=\"center\" width=\"100%\" height=\"100%\" style=\"page-break-after:always;\">"
                   "<tr>"
                     "<td align=\"center\" style=\"padding:80px;font-size:48pt;\">JA</th>"
                   "</tr>"
                   "<tr>"
                     "<td align=\"center\" style=\"font-size:36pt;\">Livro Ponto</th>"
                   "</tr>"
                   "<tr>"
                     "<td align=\"center\" style=\"font-size:36pt;\">";
  dt = dt.addDays(-1 * dt.day() + 1);
  html += dt.toString("dd/MM/yyyy") + "</td></tr>";
  html += "<tr>"
                     "<td align=\"center\" style=\"font-size:36pt;\">A</th>"
                   "</tr>"
                   "<tr>"
                     "<td align=\"center\" style=\"font-size:36pt;\">";
  dt = dt.addDays(dt.daysInMonth() - 1);
  html += dt.toString("dd/MM/yyyy") + "</td></tr>";
  html +=
                   "<tr>"
                     "<td align=\"center\" style=\"padding-top:80px;font-size:18pt;\">JA Delivery e Gastronomia LTDA</th>"
                   "</tr>"
                   "<tr>"
                     "<td align=\"center\" style=\"font-size:18pt;\">Rua Sinimbu, 175 Sala C Bairro Lourdes</th>"
                   "</tr>"
                   "<tr>"
                     "<td align=\"center\" style=\"font-size:18pt;\">CEP 95020-000</th>"
                   "</tr>"
                   "<tr>"
                     "<td align=\"center\" style=\"font-size:18pt;\">Caxias do Sul / RS</th>"
                   "</tr>"
                   "<tr>"
                     "<td align=\"center\" style=\"font-size:18pt;\">CNPJ 27.920.088/0001-16</th>"
                   "</tr>"
                 "</table>"

                 "<h1 align=\"center\">REGISTRO PONTO</h1>"
                 "<p>Nome: Rodrigo     Horário:18h00min 21h:30min 22h00min 01h20min</p>"
                 "<table border=\"1\" align=\"center\" width=\"100%\">"
                   "<tr>"
                     "<th colspan=\"2\">JANEIRO 2019</th>"
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
                   "</tr>";
  dt = dt.addDays(-1 * dt.day() + 1);
  const int daysInMonth = dt.daysInMonth();
  for (int i = 0; i != daysInMonth; ++i)
  {
    html += "<tr>"
              "<td width=\"5%\">";
    html += dt.toString("dd") + "</td>";
    html += "<td width=\"15%\">" + dt.toString("dddd") + "</td>";
    html += "<td width=\"30%\"></td>"
            "<td width=\"5%\"></td>"
            "<td width=\"5%\"></td>"
            "<td width=\"30%\"></td>"
            "<td width=\"5%\"></td>"
            "<td width=\"5%\"></td>"
            "</tr>";
    dt = dt.addDays(1);
  }
  html +=

                 "</table>"
                 "<br>"
                 "<table border=\"1\" cellpadding=\"2\" align=\"center\" width=\"100%\ style=\"border-style:solid;\">"
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
                 "</table>"
                 "</body>"
                 "</html>";
  QTextDocument doc;
  doc.setHtml(html);

  doc.setDocumentMargin(20);
  QPrinter printer(QPrinter::PrinterResolution);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPaperSize(QPrinter::A4);
  printer.setOutputFileName("test.pdf");
  doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
  doc.print(&printer);
}

#include "printutils.h"

#define ESC              "\x1b"
#define ESC_ALIGN_CENTER "\x1b\x61\x31"
#define ESC_ALIGN_LEFT   "\x1b\x61\x30"
#define ESC_FULL_CUT     "\x1b\x77"
#define ESC_PARTIAL_CUT  "\x1b\x6d"
#define ESC_STRESS_ON    "\x1b\x45"
#define ESC_STRESS_OFF   "\x1b\x46"
#define ESC_LF           "\n"
#define ESC_INIT         "\x1b\x40"
#define ESC_DOUBLE_FONT  "\x1b\x0e\x1b\x56"
#define ESC_PORTUGUESE   "\x1b\x74\x08"
#define ESC_REVERSE_ON   "\x1b\x7d\x31"
#define ESC_REVERSE_OFF  "\x1b\x7d\x30"

#define TABLE_WIDTH           48
#define TABLE_MAX_VALUE       10000

bool PrintUtils::print(QSerialPort& printer,
                       const QString& msg,
                       QString& error)
{
  error.clear();
  QByteArray data(msg.toUtf8());
  bool bRet = false;
  auto nBytes = printer.write(data);

  if (nBytes != data.size())
  {
    error = QObject::tr("Erro '%1' ao imprimir: %2").arg(
              QString::number(printer.error()),
              printer.errorString());
  }
  else if (!printer.waitForBytesWritten(5000))
  {
    error = QObject::tr("Falha de timeout.");
  }
  else
  {
    bRet = true;
  }

  return bRet;
}

bool PrintUtils::initPrinter(QSerialPort& printer,
                             QString& error)
{
  error.clear();
  QString msg = QString(ESC_INIT) + ESC_PORTUGUESE;
  return print(printer, msg, error);
}


QString PrintUtils::buildHeader(const QDate& date)
{
  return QString(ESC_ALIGN_CENTER) +
      ESC_DOUBLE_FONT +
      "B.K. RESTAURANTE"+
      ESC_LF +
      ESC_DOUBLE_FONT +
      "E LANCHERIA LTDA" +
      ESC_LF +
      ESC_DOUBLE_FONT +
      "3228-1666" +
      ESC_LF +
      ESC_ALIGN_LEFT +
      ESC_LF +
      ESC_LF +
      "Data de impressão: " + ESC_STRESS_ON +
      QDate::currentDate().toString("dd/MM/yyyy") +
      ESC_STRESS_OFF +
      ESC_LF +
      "Hora de impressão: " +
      ESC_STRESS_ON +
      QTime::currentTime().toString("hh:mm:ss") +
      ESC_LF +
      ESC_LF +
      ESC_ALIGN_CENTER +
      "Data do vale:" +
      ESC_LF
      ESC_STRESS_OFF +
      ESC_DOUBLE_FONT +
      date.toString("dd/MM/yyyy\n(dddd)") +
      ESC_LF +
      ESC_LF;
}


QString PrintUtils::buildFooter(const QString& total)
{
  return QString(ESC_LF) +
      ESC_ALIGN_CENTER +
      ESC_DOUBLE_FONT +
      "TOTAL R$" +
      total +
      ESC_LF +
      ESC_LF +
      ESC_FULL_CUT;
}

QString PrintUtils::buildBody(const TableContent& tableContent)
{
  QString body = tableContent.empty() ? "" : ESC_ALIGN_LEFT;
  for (size_t i = 0; i != tableContent.size(); ++i)
  {
    QString item;
    {
      QString itemPt1 = tableContent.at(i).at((int)Column::Ammount) +
                        tableContent.at(i).at((int)Column::Unity) +
                        " x R$" +
                        tableContent.at(i).at((int)Column::UnitValue);
      QString itemPt2 = "R$" +
                        tableContent.at(i).at((int)Column::SubTotal);
      const int n = TABLE_WIDTH - (itemPt1.length() + itemPt2.length());
      for (int j = 0; j != n; ++j)
        itemPt1 += " ";
      item = itemPt1 + ESC_STRESS_ON + itemPt2 + ESC_STRESS_OFF;
    }
    body += tableContent.at(i).at((int)Column::Description) + ESC_LF +
            item + ESC_LF +
            "────────────────────────────────────────────────" + ESC_LF;
  }
  return body;
}
